#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "shapes.h"

#define HEADER_SIZE 4
#define HEADER 0x05050505
#define KEY 0xA9
#define FILENAME "image.bin"

typedef struct __attribute__((packed))
{
    char header[4];
    uint16_t name_len;
    char* name;
    struct {
        uint16_t width;
        uint16_t height;
    } dimensions;
    SHAPE_TYPE type;
} Image;

void print_info(const Image img);
void serialize_image_to_file(Image * img, char* filename);
int serialize_image_to_buffer(Image * img, char** out);
int deserialize_image_from_file(FILE* file, Image* des_image);
int deserialize_image_from_buffer(char* buffer, Image* des_image);
void init_image(Image* img);