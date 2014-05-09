rwfs : rwfs.o 
	gcc -g `pkg-config fuse --libs` -o rwfs rwfs.o

rwfs.o : rwfs.c 
	gcc -g -Wall `pkg-config fuse --cflags` -c rwfs.c

clean:
	rm -f rwfs *.o

