#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "serialize_image.h"

#define BUFFER_SIZE 1024

void start_server(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char* pbuffer = &buffer[0];
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Binding the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Waiting for connections on port %d...\n", port);

        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted\n");

        // Read data from the client
        int read_size;
        Image rv_image;
        while ((read_size = read(new_socket, pbuffer, BUFFER_SIZE)) > 0) {
            deserialize_image_from_buffer(pbuffer, &rv_image);
            print_info(rv_image);
            Grid grid;

            /*draw_text(&grid, rv_image.name, rv_image.name_len);
            draw_shape(rv_image.type, &grid);
            draw_grid(&grid);*/

            rv_image.dimensions.width += 12;
            printf("\n Image to send to client : \n");
            print_info(rv_image);
            int img_size = serialize_image_to_buffer(&rv_image, &pbuffer);

            // Echo the message back to the client
            send(new_socket, pbuffer, img_size, 0);
            memset(buffer, 0, BUFFER_SIZE);
            pbuffer = &buffer[0];
        }

        if (read_size == 0) {
            printf("Client disconnected\n");
        } else if (read_size == -1) {
            perror("recv failed");
        }

        close(new_socket);
    }

    close(server_fd);
}
