CC = g++ 
CFLAGS = -c -Wall -std=c++11

all: main.o Logger.o cpuRetina.o
	g++ main.o Logger.o cpuRetina.o -o main
main.o:
	$(CC) $(CFLAGS) IndependentEntrypoint.cpp -o main.o
Logger.o:
	$(CC) $(CFLAGS) Logger.cpp -o Logger.o
cpuRetina.o:
	$(CC) $(CFLAGS) cpuRetina.cpp -o cpuRetina.o