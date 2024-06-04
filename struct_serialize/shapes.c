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
    uint16_t index = origin_y * grid->width + origin_x;
    free(grid->grid[index]);
    Square* square = (Square*)malloc(sizeof(Square));
    square->side = side;
    square->header.type = SQUARE;
    grid->grid[index] = (ShapeHeader*) square;

    for (int y = 0; y < side; y++) {
        for (int x = 0; x < side; x++) {
            int adjusted_y = origin_y + (int)(y / aspect_ratio);
            if (adjusted_y < grid->height && (origin_x + x) < grid->width) {
                grid->grid[adjusted_y * grid->width + origin_x + x]->pixel = PIXEL;
            }
        }
    }
    int label_index = index + (grid->width * side)/2;
    draw_label(label_index, "-Label : Square ", grid);
}

void find_shapes(Grid *grid)
{
    for(int i = 0; i < grid->height ; i++)
    {
        for(int j = 0; j < grid->width; j++)
        {
            ShapeHeader* shape = grid->grid[i * grid->width + j];
            if(shape->type == LINE)
            {
                printf("[Line] Length : %d - Coordenates : (x=%d , y=%d)\n", ((Line*)shape)->length, j, i);
                j += ((Line*)shape)->length;
            }
            else if(shape->type == CIRCLE)
            {
                printf("[Circle] Radius : %d - Coordenates : (x=%d , y=%d)\n", ((Circle*)shape)->radius, j, i);
                j += ((Circle*)shape)->radius;
            }
            else if(shape->type == SQUARE)
            {
                printf("[Square] Side : %d - Coordenates : (x=%d , y=%d)\n", ((Square*)shape)->side, j, i);
                j += ((Square*)shape)->side;
            }
        }
    }

}

void hide_lines(Grid *grid)
{
    for(int i = 0; i < grid->height ; i++)
    {
        for(int j = 0; j < grid->width; j++)
        {
            ShapeHeader* shape = grid->grid[i * grid->width + j];
            if(shape->type == LINE)
            {
                shape->pixel = 'h';
            }
        }
    }
}

void draw_label(uint16_t index, const char *text, Grid *grid)
{
    if(index >= grid->height * grid->width)
    {
        printf("Label outside grid\n");
        return;
    }
    grid->grid[index]->label = (Label*) malloc(sizeof(Label));
    strcpy(grid->grid[index]->label->text, text);
    grid->grid[index]->type = LABEL;
    for(int i = 0; i < strlen(grid->grid[index]->label->text); i++) {
        if (index + i < MAX_GRID_SIZE && (index % grid->width) + i < grid->width) {
            grid->grid[index + i]->pixel = grid->grid[index]->label->text[i];
        }
    }
}

void draw_circle(uint8_t origin_x, uint8_t origin_y, uint8_t radius, Grid* grid) {
    float aspect_ratio = 2.0;  // Adjust this value based on your terminal's aspect ratio
    
    // Create a circle
    uint16_t index = origin_y * grid->width + origin_x;
    free(grid->grid[index]);
    Circle* circle = (Circle*) malloc(sizeof(Circle));
    circle->header.type = CIRCLE;
    circle->radius = radius;
    grid->grid[index] = (ShapeHeader*)circle;
    uint16_t in = 0;
    int x = 0, y = 0;
    for (y = 0; y < grid->height; y++) {
        for (x = 0; x < grid->width; x++) {
            float dx = x - origin_x;
            float dy = (y - origin_y) * aspect_ratio;
            if (dx * dx + dy * dy <= radius * radius) {
                grid->grid[y * grid->width + x]->pixel = PIXEL;
            }
        }
    }

    draw_label(index + grid->width + (radius * grid->width/aspect_ratio), "-Label : Circle", grid);
}

void draw_grid(Grid* grid)
{
    for(int i = 0; i < grid->height; i++)
    {
        for(int j = 0; j < grid->width; j++)
        {
            if(HIDDEN != grid->grid[i * grid->width + j]->pixel)
                printf("%c",grid->grid[i * grid->width + j]->pixel);
            else
                printf(" ");
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
    // Polymorphisme here
    grid->grid[index] = (ShapeHeader*)line;
    for(uint16_t i = 0; i < length ; i++) {
        if ((origin_x + i) < grid->width) {
            grid->grid[index + i]->pixel = PIXEL;
            grid->grid[index + i]->type = LINE;
        }
    }
    draw_label(index + grid->width,"-Label : Line", grid);
}

void draw_shape(enum SHAPE shape, Grid* grid)
{
    switch (shape)
    {
    case LINE:
        draw_line(30, 5, 30, grid);
        draw_square(10, 25, 10, grid);
        draw_circle(65, 25, 10, grid);
        break;
    case SQUARE:
        draw_square(10, 30, 10, grid);
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