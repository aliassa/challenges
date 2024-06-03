#include "shapes.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


void init_grid(Grid* grid)
{
    memset(grid->grid, ' ', GRID_HEIGHT * GRID_WIDTH);
    grid->height = GRID_HEIGHT;
    grid->width = GRID_WIDTH;
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (i == 0 || i == GRID_HEIGHT - 1 || j == 0 || j == GRID_WIDTH - 1) {
                grid->grid[i * GRID_WIDTH + j] = '#';
            }
        }
    }
}

void draw_square(uint8_t origin, uint8_t side, Grid* grid)
{
    uint16_t top_left = (grid->height / 4 + 1) * grid->width + 1 + origin;
    char* draw_ptr = &grid->grid[top_left];
    for(uint16_t i = 0; i < side; i++)
    {
        for(uint16_t j = 0; j < grid->width; j++)
        {
            if((i == 0 || i == side -1) && (j <= side) || j==0 || j == side)
                draw_ptr[i * grid->width + j] = '#';
        }
    }
}

void draw_circle(uint8_t origin, uint8_t radius, Grid* grid)
{ }

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
    uint16_t begin = (grid->height / 2 + 1) * grid->width + 1 + origin;
    char* draw_ptr = &grid->grid[begin];
    for(uint16_t i = 0; i < length ; i++)
    {
        *draw_ptr = '-';
        draw_ptr++;
    }
}


void draw_shape(enum SHAPE shape, Grid* grid)
{
    switch (shape)
    {
    case LINE:
        draw_line(5, 10, grid);
        break;
    case SQUARE:
        draw_square(5, 10, grid);
        break;
    case CIRCLE:
        draw_circle(5, 10, grid);
        break;
    default:
        printf("NOT SUPPRTED SHAPE!\n");
        break;
    }
}