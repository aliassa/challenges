#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "shapes.h"

#define HEADER_SIZE 4
#define HEADER 0x50505050
#define KEY 0xA9
#define FILENAME "image.bin"

typedef struct
{
    char header[4];
    uint16_t name_len;
    char* name;
    struct {
        uint16_t width;
        uint16_t height;
    } dimensions;
    //SHAPE_TYPE type;
} Image;

void serialize_image_to_file(Image * img, char* filename);
int deserialize_image_from_file(const char* filename, Image* des_image);
void  darw_image(const Image* img);