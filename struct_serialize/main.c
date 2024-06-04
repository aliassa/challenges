#include "serialize_image.h"
#include "tcp_server.h"
#include <unistd.h> // For sleep
#include <pthread.h>
#include <string.h>

#define MAX_SHAPES 125
#define FRAME_RATE 50

Grid main_grid;
extern void clear_screen();

ShapeHeader* shapes[MAX_SHAPES]; // initialized to null!
uint16_t shape_count;
int shape_id;

void draw_all_file()
{
    Image des_bird;
    // To be factorized
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    Grid image;
    init_grid(&image);
    Image images[10];
    uint32_t seek = 0;
    int res = 0;
    for(int i = 0; i < 10 ; i++)
    {
        res = deserialize_image_from_file(file, images + i);
        if(res == -1) return;
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
}





int main()
{
    pthread_t server_thread;
    init_grid(&main_grid);
    draw_text(&main_grid, "Main Grid Window", 16);
    ShapeHeader* sq1 = draw_square(5, 5, 12, &main_grid);
    ShapeHeader* sq2 = draw_square(25, 5, 12, &main_grid);
    ShapeHeader* sq3 = draw_square(45, 5, 12, &main_grid);
    ShapeHeader* sq4 = draw_square(65, 5, 12, &main_grid);

    ShapeHeader* l1 = draw_line(5, 15, 15, &main_grid);
    ShapeHeader* l2 = draw_line(25, 15, 15, &main_grid);
    ShapeHeader* l3 = draw_line(45, 15, 15, &main_grid);
    ShapeHeader* l4 = draw_line(65, 15, 15, &main_grid);

    ShapeHeader* c1 = draw_circle(12, 25, 10, &main_grid);
    ShapeHeader* c2 = draw_circle(32, 25, 10, &main_grid);
    ShapeHeader* c3 = draw_circle(52, 25, 10, &main_grid);
    ShapeHeader* c4 = draw_circle(72, 25, 10, &main_grid);
    pthread_create(&server_thread, NULL, start_server, NULL); 
     while (1) {
        clear_screen();
        update_shapes(&main_grid);
        draw_grid(&main_grid);
        usleep(1000000 / FRAME_RATE); // Sleep to maintain frame rate
    }
    
    pthread_join(server_thread, NULL);
    return 0;
}
