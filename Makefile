CC = gcc
CFLAGS = -DCLEAR

all: client server test

client: tcp_client.c serialize_image.c
	$(CC) $(CFLAGS) $^ -o $@

server: main.c shapes.c tcp_server.c serialize_image.c
	$(CC) $(CFLAGS) $^ -o $@

test: test_serialize.c serialize_image.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f client server test
