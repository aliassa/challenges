#include "serialize_image.h"
#include "tcp_server.h"


int main()
{
    printf("SIZEOF IMAGE : %ld\n", sizeof(Image));
    Image bird;
    
    bird.name = "Ali and Ilyes Image";
    bird.name_len = strlen(bird.name);
    bird.dimensions.width = 50;
    bird.dimensions.height = 30;
    bird.type  = SQUARE;

    serialize_image_to_file(&bird, FILENAME);

    char* buff;
    serialize_image_to_buffer(&bird, &buff);
    start_server(8080);

    //free(img.name);
    //free(buff);


    return 0;
    
    Image des_bird;
    // To be factorized
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    Grid image;
    init_grid(40, 40, &image);
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
        draw_shape(images[i].type, &image);
        draw_grid(&image);
    }

    fclose(file);
    free(des_bird.name);

    return 0;
}
