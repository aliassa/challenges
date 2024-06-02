#include "serialize_image.h"


void print_info(const Image img)
{
    printf("\nheader:[%08x] | name_len:[%d] | name:[%s] | width:[%d] | height:[%d] | type:[%d]\n",
            *(uint32_t*)img.header, img.name_len,  img.name, img.dimensions.width, img.dimensions.height
            , img.type);
}


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

void init_image(Image* img)
{
    const uint32_t header = HEADER;
    img->header[0] = (header >> 24 ) &0xFF;
    img->header[1] = (header >> 16 ) &0xFF;
    img->header[2] = (header >> 8 ) &0xFF;
    img->header[3] = (header >> 0 ) &0xFF;
}

void serialize_image_to_file(Image * img, char* filename)
{
    init_image(img);
    size_t size = sizeof(Image) + img->name_len - sizeof(char*);
    // header | name_length | name | width | height | type
    int index = 0;
    char* memory = (char*)malloc(size);
    memset(memory, img->header[0], 4);
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

int deserialize_image_from_buffer(char* buffer, Image* out)
{
    // read buffer into Image*
    char* read_ptr = buffer;
    const uint32_t h = HEADER;
    if(memcmp(read_ptr, &h, 4) != 0)
    {
        printf("Not an image , header : %08x!\n", *(uint32_t*)read_ptr);
        return -1;
    }
    memcpy(out->header, read_ptr, 4);
    out->name_len = *(uint16_t*)(&read_ptr[4]);
    read_ptr += 6; // header char[4] (4) + uint16_t (2)

    out->name = (char*)malloc(out->name_len + 1);
    out->name[out->name_len] = '\0';
    // header | name_length | name | width | height | type
    memcpy(out->name, read_ptr, out->name_len);
    read_ptr += out->name_len;
    memcpy(&out->dimensions, read_ptr, 4 + 4);
}
// input : char* img;
// the func is reponsible for allocation
int serialize_image_to_buffer(Image * img, char** out)
{
    init_image(img);
    size_t size = sizeof(Image) + img->name_len - sizeof(char*);
    *out = (char*)malloc(size);
    if(*out == NULL) return 0;
    char* write_ptr = *out;
    memset(write_ptr, img->header[0], HEADER_SIZE);
    write_ptr += HEADER_SIZE;
    memcpy(write_ptr, &img->name_len, 2);
    write_ptr += 2; // name_len uint16_t
    memcpy(write_ptr, img->name, img->name_len);
    write_ptr += img->name_len;
    memcpy(write_ptr, &img->dimensions, 4);
    write_ptr += 4;
    memcpy(write_ptr, &img->type, 4);
    write_ptr += 4;
    return size;
}