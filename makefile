packed=/tmp/$(filename)
sstrip=elfkickers/bin/sstrip

ELFKICKERSDIR = elfkickers

WARNINGS = -Wall -Wno-implicit-function-declaration
CCFLAGS = -Os -fomit-frame-pointer -nostdlib -nostartfiles $(WARNINGS)

ldlinux=/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
libsdl=/usr/lib/x86_64-linux-gnu/libSDL.so
libgl=/usr/lib/x86_64-linux-gnu/libGL.so
stub=stubstart
libs=$(libsdl) $(libgl)
outputs_gen=`for f in src/*.c; do (echo -n "build/$$(basename $${f%.c}) "); done)`

all:
	make clean compress output=$(outputs_gen)
	ls -la build/*

elfkickers:
	$(MAKE) -C $(ELFKICKERSDIR)

compress: $(output) $(sstrip) 
	strip -s -R .comment -R .gnu.version $<
	$(sstrip) $<
	cp unpack.header packed
	lzma -c -9 $< >> packed
	mv packed $<
	chmod +x $<

build/%: %.o $(stub).o
	mkdir -p build
	ld -dynamic-linker $(ldlinux) $(stub).o $< $(libs) -o $@
	rm $<

$(stub).o:
	gcc $(stub).S -c $<

%.o: src/%.c
	./s2h.py $<
	gcc $(CCFLAGS) -c $<

$(sstrip):
	$(MAKE) -C $(ELFKICKERSDIR)	

clean:
	rm -f build/*
	rm -f src/*_shaders.h
	rm -f $(stub).o

clean-all: clean
	$(MAKE) -C $(ELFKICKERSDIR) clean
