OBJ = uwp.o collision.o
TARGET = collision
CC = g++
OPTION = -O3

$(TARGET): $(OBJ)
	$(CC) $(OPTION) -o $(TARGET) $^

uwp.o:
	$(CC) -c uwp.cpp

test.o:
	$(CC) -c collision.cpp

clean:
	rm collision *.o