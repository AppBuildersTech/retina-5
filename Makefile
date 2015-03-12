CC = g++ 
CFLAGS = -Wall -std=c++11

all: IndependentEntrypoint.o Logger.o cpuRetina.o
	g++ IndependentEntrypoint.o Logger.o cpuRetina.o -o main
IndependentEntrypoint.o: IndependentEntrypoint.cpp
	$(CC) -c $(CFLAGS) IndependentEntrypoint.cpp
Logger.o: Logger.cpp
	$(CC) -c $(CFLAGS) Logger.cpp
cpuRetina.o: cpuRetina.cpp
	$(CC) -c $(CFLAGS) cpuRetina.cpp
clean:
	rm -rf *.o hello