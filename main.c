#include "serialize_image.h"

int main()
{
    const uint32_t header = HEADER;
    printf("SIZEOF IMAGE : %ld\n", sizeof(Image));
    Image bird = { 
        .name_len = 10,
        .name = "Bird Image",
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
    deserialize_image_from_file(FILENAME, &des_bird);
    printf("\nheader:%08x | name_len:%d | name:%s | width:%d | height:%d | type:%d\n",
            *(uint32_t*)des_bird.header, des_bird.name_len,  des_bird.name, des_bird.dimensions.width, des_bird.dimensions.height
            , des_bird.type);
    #ifndef NODRAW
        Grid image;
        init_grid(des_bird.dimensions.height,des_bird.dimensions.width,&image);
        draw_text(&image, des_bird.name, des_bird.name_len);
        draw_square(35, 8, &image);
        draw_square(22, 8, &image);
        draw_square(10, 8, &image);
        draw_grid(&image);
    #endif // NODRAW    

    free(des_bird.name);

    return 0;
}
