filename=example
packed=/tmp/$(filename)
ldlinux=/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
libsdl=/usr/lib/x86_64-linux-gnu/libSDL.so
libgl=/usr/lib/x86_64-linux-gnu/libGL.so
libc=/lib/x86_64-linux-gnu/libc.so.6

all:
	gcc -Os -fomit-frame-pointer -c $(filename).c
	ld -dynamic-linker $(ldlinux) $(filename).o $(libsdl) $(libgl) $(libc) -o $(filename)
	strip -s -R .comment -R .gnu.version $(filename)
	cp unpack.header $(packed)
	gzip -cn9 $(filename) >> $(packed)
	cp $(packed) $(filename)

clean:
	rm *.o
