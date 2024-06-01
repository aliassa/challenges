#include "serialize_image.h"

static void encrypt(char* data, size_t size)
{
#ifndef CLEAR
    for(size_t i = 0; i < size; i++)
        data[i] ^= KEY;
#endif // CLEAR
}

static void decrypt(char* data, size_t size)
{
    encrypt(data, size);
}


void serialize_image_to_file(Image * img, char* filename)
{
    size_t size = sizeof(Image) + img->name_len - sizeof(char*);
    // header | name_length | name | width | height | type
    int index = 0;
    char* memory = (char*)malloc(size);
    memset(memory, 0x50, 4);
    index += HEADER_SIZE;
    memcpy(memory + index, &img->name_len, sizeof(img->name_len));
    index += sizeof(img->name_len);
    memcpy(memory + index, img->name, img->name_len);
    index += img->name_len;
    memcpy(memory + index , &img->dimensions, 4);
    index += 4;
    memcpy(memory + index, &img->type, sizeof(SHAPE_TYPE));
    index += sizeof(SHAPE_TYPE);

    encrypt(memory, index);

    FILE *file = fopen(filename, "ab"); // wb for not append
    if(file == NULL)
    {
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


// allocates memory for image name
int deserialize_image_from_file(FILE* file, Image* des_image)
{
    // read header and length
    long curr_seek = ftell(file);
    char h_len[6];
    fread(h_len, 6, 1, file);
    decrypt(h_len, 6);
    const uint32_t h = HEADER;
    if(memcmp(&h, h_len, 4) != 0)
    {
        printf("Not an image , header : %08x!\n", *(uint32_t*)h_len);
        return -1;
    }
    memcpy(des_image, h_len, 4);
    des_image->name_len = *(uint16_t*)(&h_len[4]);
    des_image->name = (char*)malloc(des_image->name_len + 1);
    des_image->name[des_image->name_len] = '\0';

    fseek(file, 6 + curr_seek, SEEK_SET);
    printf(" 2 Current seek : %ld\n", ftell(file));
    char rest[des_image->name_len + 4 + 4]; // name + dims + shape_type
    char* p = rest;
    fread(rest, des_image->name_len + 4 + 4, 1, file);

    decrypt(rest, des_image->name_len + 4 + 4);
    memcpy(des_image->name, p, des_image->name_len);
    p += des_image->name_len;
    memcpy(&des_image->dimensions, p, 4);
    p += 4;
    memcpy(&des_image->type, p, 4);
}