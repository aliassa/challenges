#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "serialize_image.h"

#define BUFFER_SIZE 1024
#define MAX_NAME 1024

int main(int ac, char* av[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    // img to send :
    if(ac <= 1)
    {
        printf("Enter the name of the image\n");
        return -1;
    }

    char img_name[MAX_NAME] = {0};
    char* ptr = &img_name[0];
    size_t size = 0;
    for(int i = 1; i < ac; i++)
    {
        memcpy(ptr, av[i], strlen(av[i]));
        ptr += strlen(av[i]);
        if(i != ac -1)
        {
            memset(ptr, ' ', 1);
            ptr++;
        }
    }

    Image bird;
    bird.name = (char*)malloc(strlen(img_name) + 1);
    strcpy(bird.name, img_name);
    bird.name_len = strlen(bird.name);
    bird.dimensions.width = 50;
    bird.dimensions.height = 30;
    bird.type  = SQUARE;

    char *img;
    int img_size = serialize_image_to_buffer(&bird, &img);
    //
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 and IPv6 addresses from text to binary form
   // if (inet_pton(AF_INET, "192.168.1.6", &serv_addr.sin_addr) <= 0) {
      if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // Send message to server
    send(sock, img, img_size, 0);

    // Receive message from server
    // For now server doesn't respond!
    // TODO
    // get info abouts shapes!
    /*int valread = read(sock, buffer, BUFFER_SIZE);
    Image rv_image;
    deserialize_image_from_buffer(buffer, &rv_image);
    printf("From SERVER : \n");
    print_info(rv_image);*/

    // Close the socket
    close(sock);

    return 0;
}
