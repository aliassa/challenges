#include "shapes.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define LINE_TO_DRAW 10



void init_grid(uint16_t height, uint16_t width, Grid* grid)
{
    memset(grid->grid, ' ', height * width);
    grid->height = height;
    grid->width = width;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                grid->grid[i * width + j] = '#';
            }
        }
    }
}

void draw_grid(Grid* grid)
{
    for(int i = 0; i < grid->height; i++)
    {
        for(int j = 0; j < grid->width; j++)
        {
            printf("%c",grid->grid[i * grid->width + j]);
        }
        printf("\n");
    }
}

void draw_text(Grid* grid, char* text, uint16_t len)
{
    uint16_t text_line = 3;
    uint16_t text_start = (grid->width - len)/2;

    uint16_t begin = text_line * grid->width + text_start;
    char* draw_ptr = &grid->grid[begin];
    for(uint16_t i = 0; i < len ; i++)
    {
        *draw_ptr = text[i];
        draw_ptr++;
    }
}

void draw_line(uint8_t origin, uint8_t length,Grid* grid)
{
    uint16_t begin = LINE_TO_DRAW * grid->width + 1;
    char* draw_ptr = &grid->grid[begin];
    for(uint16_t i = 0; i < length ; i++)
    {
        *draw_ptr = '-';
        draw_ptr++;
    }
}