rwfs : rwfs.o arc4.o
	gcc -g `pkg-config fuse --libs` -o rwfs rwfs.o arc4.o

rwfs.o : rwfs.c 
	gcc -g -Wall `pkg-config fuse --cflags` -c rwfs.c

arc4.o : arc4.c 
	gcc -g -Wall -c arc4.c

clean:
	rm -f rwfs *.o

