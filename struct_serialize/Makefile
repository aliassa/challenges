CC = gcc
CFLAGS = -DCLEAR

all: client server test

client: tcp_client.c serialize_image.c
	$(CC) -g  $(CFLAGS) $^ -o $@

server: main.c shapes.c tcp_server.c serialize_image.c
	$(CC) -g $(CFLAGS) $^ -o $@

test: test_serialize.c serialize_image.c
	$(CC) -g $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f client server test
