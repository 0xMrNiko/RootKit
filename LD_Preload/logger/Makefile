CC = gcc
CFLAGS = -shared -fPIC
LDFLAGS = -ldl

TARGET = logger.so
SRC = logger.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
