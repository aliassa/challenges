#include "serialize_image.h"



void print_info(const Image img)
{
    printf("\nheader:%08x | name_len:%d | name:%s | width:%d | height:%d | type:%d\n",
            *(uint32_t*)img.header, img.name_len,  img.name, img.dimensions.width, img.dimensions.height
            , img.type);
}

int main()
{
    const uint32_t header = HEADER;
    printf("SIZEOF IMAGE : %ld\n", sizeof(Image));
    Image bird = { 
        .name_len = 19,
        .name = "Ali and Ilyes Image",
        .dimensions.width = 50,
        .dimensions.height = 30,
        .type = LINE
         };
    bird.header[0] = (header >> 24 ) &0xFF;
    bird.header[1] = (header >> 16 ) &0xFF;
    bird.header[2] = (header >> 8 ) &0xFF;
    bird.header[3] = (header >> 0 ) &0xFF;

    serialize_image_to_file(&bird, FILENAME);

    Image des_bird;
    // To be factorized
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    Grid image;
    init_grid(10, 30, &image);
    Image images[10];
    uint32_t seek = 0;
    int res = 0;
    for(int i = 0; i < 10 ; i++)
    {
        res = deserialize_image_from_file(file, images + i);
        if(res == -1) return -1;
        seek += sizeof(Image) - sizeof(char*) + images[i].name_len;
        if(fseek(file, seek, SEEK_SET) != 0)
        {
            printf("cant sek!\n");
            break;
        }

        print_info(images[i]);
        draw_text(&image, images[i].name, images[i].name_len);
        draw_grid(&image);
    }

    fclose(file);
    free(des_bird.name);

    return 0;
}
