CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lX11 -lXrandr
TARGET = x-vertical-tiling

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LDFLAGS)

clean:
	rm -f $(TARGET)
