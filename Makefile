TARGET = BountyTrails

CC = gcc

SRC = main.c 

LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all:
	$(CC) $(SRC) -o $(TARGET) $(LDFLAGS)
	@echo "Compiled successfully!"

clean:rm -f $(TARGET)