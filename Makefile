CC = g++

OBJS = main.o

main.exe : $(OBJS)
	$(CC) $(OBJS) -std=c++0x -o main.exe

main.o : ./src/test.cpp 
	$(CC) -std=c++0x -c src/test.cpp -o main.o

.PHONY clean:

clean:
	rm -f *.d *.o *.exe
