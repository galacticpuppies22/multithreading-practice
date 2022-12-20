CC=g++
STD=-std=c++17
PTHREAD=-pthread 
# OCOLORS=-ocolors 

mandelbrot:
	${CC} ${STD} ${PTHREAD} -o ./bin/mandelbrot.o ./src/mandelbrot.cpp

run:
	./bin/mandelbrot.o -2 1 -1.5 1.5 e0.bmp
	./bin/mandelbrot.o -0.5 0 0.3 1.2 e1.bmp
	./bin/mandelbrot.o 0.3 0.4 0.6 0.7 e2.bmp
	./bin/mandelbrot.o -0.2 0.0 -1.0 -0.9 e3.bmp
	./bin/mandelbrot.o -0.05 -0.01 -1.01 -0.97 e4.bmp

clean:
	# rm -f colors
	rm -f ./bin/mandelbrot.o
	rm -f e0.bmp
	rm -f e1.bmp
	rm -f e2.bmp
	rm -f e3.bmp
	rm -f e4.bmp