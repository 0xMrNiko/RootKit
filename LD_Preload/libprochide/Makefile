CC = gcc
CFLAGS = -shared -fPIC
LDFLAGS = -ldl

TARGET = libprochide.so
SRC = libprochide.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
