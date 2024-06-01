#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HEADER_SIZE 4
#define HEADER 0x50505050

typedef struct
{
    char header[4];
    uint16_t name_len;
    char* name;
    struct {
        uint16_t width;
        uint16_t height;
    } dimensions;
} Image;


void serialize_image_to_file(Image * img, char* filename)
{
    size_t size = sizeof(Image) + img->name_len - sizeof(char*);
    // header | name_length | name | width | height |
    int index = 0;
    char* memory = (char*)malloc(size);
    memset(memory, 0x50, 4);
    printf("written header : %08x\n", *(uint32_t*)memory);
    index += HEADER_SIZE;
    memcpy(memory + index, &img->name_len, sizeof(img->name_len));
    index += sizeof(img->name_len);
    memcpy(memory + index, img->name, img->name_len);
    index += img->name_len;
    memcpy(memory + index , &img->dimensions, 4);

    printf("Raw object size is : %d\n", index);

     FILE *file = fopen(filename, "wb");
     if(file == NULL)
     {
        printf("Couldn't open file, clean and out\n");
        free(memory);
        return;
     }
     size_t written = fwrite(memory, size, 1, file);
    if (written != 1) {
        perror("Error writing to file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    free(memory);
}



int deserialize_image_from_file(const char* filename, Image* des_image)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // read header and length
    char h_len[6];
    fread(h_len, 6, 1, file);
    printf("Read header : %08x\n", *(uint32_t*)h_len);
    const uint32_t h = HEADER;
    if(memcmp(&h, h_len, 4) != 0)
    {
        printf("Not an image!\n");
        return -1;
    }
    memcpy(des_image, h_len, 4);
    des_image->name_len = *(uint16_t*)(&h_len[4]);
    des_image->name = (char*)malloc(des_image->name_len + 1);
    des_image->name[des_image->name_len] = '\0';

    fseek(file, 6, SEEK_SET);
    char rest[des_image->name_len + 4]; // name + dims
    fread(rest, des_image->name_len + 4, 1, file);
    memcpy(des_image->name, rest, des_image->name_len);
    memcpy(&des_image->dimensions, rest + des_image->name_len , 4);
    // Close the file
    fclose(file);
}


int main()
{
    const uint32_t header = HEADER;
    
    Image bird = { 
        .name_len = 13,
        .name = "123456789abcd",
        .dimensions.width = 5,
        .dimensions.height = 12,
         };
    bird.header[0] = (header >> 24 ) &0xFF;
    bird.header[1] = (header >> 16 ) &0xFF;
    bird.header[2] = (header >> 8 ) &0xFF;
    bird.header[3] = (header >> 0 ) &0xFF;
    serialize_image_to_file(&bird, "image.bin");
    


    Image des_bird;
    deserialize_image_from_file("image.bin", &des_bird);
    printf("\nheader:%08x | name_len:%d | name:%s | width:%d | height:%d\n",
            *(uint32_t*)des_bird.header, des_bird.name_len,  des_bird.name, des_bird.dimensions.width, des_bird.dimensions.height);

    
    free(des_bird.name);

    return 0;
}

