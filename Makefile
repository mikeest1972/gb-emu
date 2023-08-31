CC = gcc
IDIR = include/
TARGET = gbemulator# executable file name
LIBS = -lSDL2main -lSDL2 -lSDL2_ttf 
CFLASGS = -I$(IDIR) -o $(TARGET) $(LIBS)

all:
	$(CC) src/*.c $(CFLASGS)

clean:
	rm $(TARGET)

