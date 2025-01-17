CC = gcc
CFLAGS = -shared -fPIC
LDFLAGS = -ldl

TARGET = socketlogger.so
SRC = socketlogger.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
