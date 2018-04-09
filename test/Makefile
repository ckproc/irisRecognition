CC = g++
CFLAGS = -fPIC -std=c++11 -O2 -DDLIB_JPEG_SUPPORT
INCLUDES =-I. -I/usr/include -I/usr/include/opencv -I/usr/include/opencv2 -I/home/ckp/dlib-19.4
LIBS = -L. `pkg-config --libs opencv` -ljpeg -lpthread -lX11
SRC = ./*.cpp /home/ckp/dlib-19.4/dlib/all/source.cpp
EXEC = iris

$(EXEC):$(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) $(INCLUDES) $(LIBS)

clean:
	rm iris 

