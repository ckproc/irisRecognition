all:
	g++ -std=c++11 -DDLIB_JPEG_SUPPORT -I/home/ckp/dlib-19.4 /home/ckp/dlib-19.4/dlib/all/source.cpp -ljpeg -lpthread -lX11 trainiris.cpp -O3 -o trainiris
	g++ -std=c++11 -DDLIB_JPEG_SUPPORT -I/home/ckp/dlib-19.4 /home/ckp/dlib-19.4/dlib/all/source.cpp -ljpeg -lpthread -lX11 test.cpp -O3 -o test
clean:
	rm trainiris
	rm test