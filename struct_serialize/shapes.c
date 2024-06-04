#include "shapes.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <stddef.h>

#define MAX_SHAPES 125

extern ShapeHeader* shapes[MAX_SHAPES];
extern uint16_t shape_count;

void init_grid(Grid* grid)
{
    for (int i = 0; i < GRID_HEIGHT * GRID_WIDTH; i++) {
        grid->grid[i] = (ShapeHeader*)malloc(sizeof(ShapeHeader));
        grid->grid[i]->pixel = ' ';
        grid->grid[i]->type = SHAPE_TYPE_UNKNOWN;
    }

    grid->height = GRID_HEIGHT;
    grid->width = GRID_WIDTH;
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (i == 0 || i == GRID_HEIGHT - 1 || j == 0 || j == GRID_WIDTH - 1) {
                grid->grid[i * GRID_WIDTH + j]->pixel = '#';
                grid->grid[i * GRID_WIDTH + j]->type = BORDER;
            }
        }
    }
}

static void add_shape(ShapeHeader* shape)
{
    for(int i = 0; i < MAX_SHAPES; i++)
    {
        if(!shapes[i])
        {
            shapes[i] = shape;
            shape_count++;
        }
    }
    printf("Added a ");
    switch (shape->type)
    {
    case LINE:
        printf("Line to the grid\n");
        break;
    case SQUARE:
        printf("Square to the grid\n");
        break;
    case CIRCLE:
        printf("Circle to the grid\n");
        break;
    default:
        break;
    }
}

void draw_square(uint8_t origin_x, uint8_t origin_y, uint8_t side, Grid* grid) {
    float aspect_ratio = 1.6;  // Adjust this value based on your terminal's aspect ratio

    for (int y = 0; y < side; y++) {
        for (int x = 0; x < side; x++) {
            int adjusted_y = origin_y + (int)(y / aspect_ratio);
            if (adjusted_y < grid->height && (origin_x + x) < grid->width) {
                grid->grid[adjusted_y * grid->width + origin_x + x]->pixel = PIXEL;
            }
        }
    }
}

void find_line(Grid *grid)
{
    // search for shapes
    // Now we support lines only
    uint16_t count = 0;
    for(int i = 0; i < grid->height ; i++)
    {
        for(int j = 0; j < grid->width; j++)
        {
            ShapeHeader* shape = grid->grid[i * grid->width + j];
            if(shape->type == LINE)
            {
                //Line* line = (Line*)((char*)shape - offsetof(Line, header));
                //Line* line = (Line*) shape;
                printf("===========================================\n");
                printf("Found a line with width : %d At : %d, %d\n", ((Line*)shape)->length, j, i);
                printf("===========================================\n");
                break; // TODO see if we can search multiple lines in same grid line
                // use legth for that!
            }
        }
    }

}

//void draw_circle(uint8_t origin, uint8_t radius, Grid* grid)
void draw_circle(uint8_t origin_x, uint8_t origin_y, uint8_t radius, Grid* grid) {
    float aspect_ratio = 2.0;  // Adjust this value based on your terminal's aspect ratio

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            float dx = x - origin_x;
            float dy = (y - origin_y) * aspect_ratio;
            if (dx * dx + dy * dy <= radius * radius) {
                grid->grid[y * grid->width + x]->pixel = PIXEL;
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
            printf("%c",grid->grid[i * grid->width + j]->pixel);
        }
        printf("\n");
    }
}

void draw_text(Grid* grid, char* text, uint16_t len)
{
    uint16_t text_line = 3;
    uint16_t text_start = (grid->width - len)/2;

    uint16_t begin = text_line * grid->width + text_start;
    ShapeHeader* draw_ptr = grid->grid[begin];
    for(uint16_t i = 0; i < len ; i++)
    {
        draw_ptr->pixel = text[i];
        draw_ptr++;
    }
}

void draw_line(uint8_t origin_x, uint8_t origin_y, uint8_t length, Grid* grid) {
    uint16_t index = (origin_y * grid->width) + origin_x;
    // Create a line
    free(grid->grid[index]);
    Line* line = (Line*) malloc(sizeof(Line));
    line->header.type = LINE;
    line->header.pixel = PIXEL;
    line->length = length;
    grid->grid[index] = (ShapeHeader*)line;
    for(uint16_t i = 0; i < length ; i++) {
        if ((origin_x + i) < grid->width) {
            grid->grid[index + i]->pixel = PIXEL;
        }
    }
    add_shape(grid->grid[index]);
}

void draw_shape(enum SHAPE shape, Grid* grid)
{
    switch (shape)
    {
    case LINE:
        draw_line(30, 5, 30, grid);
        draw_line(35, 10, 50, grid);
        draw_circle(65, 25, 20, grid);
        draw_square(10, 25, 20, grid);
        break;
    case SQUARE:
        draw_square(10, 25, 10, grid);
        break;
    case CIRCLE:
        draw_circle(65, 25, 10, grid);
        break;
    case SHAPE_TYPE_UNKNOWN:
        printf("UNKNOWN SHAPE!\n");
        break;
    default:
        printf("NOT SUPPRTED SHAPE!\n");
        break;
    }
}