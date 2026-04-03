all: main

CC=gcc
C_FLAGS="-g"
TARGET=yash
SRC_DIR=.

main: main.c
	$(CC) $(SRC_DIR)/*.c $(C_FLAGS) -o $(TARGET)  

clean:
	rm -f $(TARGET) $(TARGET).s $(TARGET).o