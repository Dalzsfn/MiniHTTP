CC      = gcc
CFLAGS  = -Wall -Wextra -Iinclude -IComer

SRC = src/main.c \
      src/server.c \
      src/http.c \
      src/files.c \
      src/mime.c \
      Comer/passiveTCP.c \
      Comer/passivesock.c \
      Comer/errexit.c

TARGET = minihttp

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
