# PA7 makefile
# Brad Smith

CC=g++
FLAGS=-I. -Wall -g -O2 -std=c++11
FILES=main.o Image.o ImageSmith.o KeyPoints.o
pa7: $(FILES)
	$(CC) $(FLAGS) $(FILES) -o pa7
main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp
Image.o: Image.cpp Image.h
	$(CC) $(FLAGS) -c Image.cpp
ImageSmith.o: ImageSmith.cpp ImageSmith.h
	$(CC) $(FLAGS) -c ImageSmith.cpp
KeyPoints.o: KeyPoints.cpp KeyPoints.h
	$(CC) $(FLAGS) -c KeyPoints.cpp
clean:
	rm -f *.o PA7.tar pa7
tar:
	tar -cf PA7.tar *.cpp *.h Makefile
