packed=/tmp/$(filename)
sstrip=elfkickers/bin/sstrip

ELFKICKERSDIR = elfkickers

ldlinux=/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
libsdl=/usr/lib/x86_64-linux-gnu/libSDL.so
libgl=/usr/lib/x86_64-linux-gnu/libGL.so
libc=/lib/x86_64-linux-gnu/libc.so.6
libs=$(libsdl) $(libgl) $(libc)
outputs_gen=`for f in src/*.c; do (echo -n "build/$$(basename $${f%.c}) "); done)`

all:
	make clean compress output=$(outputs_gen)

elfkickers:
	$(MAKE) -C $(ELFKICKERSDIR)

compress: $(output) $(sstrip) 
	strip -s -R .comment -R .gnu.version $<
	$(sstrip) $<
	cp unpack.header packed
	lzma -c -9 $< >> packed
	mv packed $<
	chmod +x $<

build/%: %.o
	mkdir -p build
	ld -dynamic-linker $(ldlinux) $< $(libs) -o $@
	rm $<

%.o: src/%.c
	gcc -Os -fomit-frame-pointer -c $<

$(sstrip):
	$(MAKE) -C $(ELFKICKERSDIR)	

clean:
	rm -f build/*

clean-all: clean
	$(MAKE) -C $(ELFKICKERSDIR) clean
