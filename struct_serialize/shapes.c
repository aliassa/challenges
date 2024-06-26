#include "shapes.h"
#include "colors.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <stddef.h>

#define MAX_SHAPES 125

extern ShapeHeader* shapes[MAX_SHAPES];
extern uint16_t shape_count;
extern int shape_id;

void print_color(SHAPE_TYPE type)
{
    if(type == BORDER)
    {
        printf(ANSI_COLOR_RED);
    }
    else if(type == LABEL)
    {
        printf(ANSI_COLOR_YELLOW);
    }
    else if(type == LINE)
    {
        printf(ANSI_COLOR_CYAN);
    }
    else if(type == SQUARE)
    {
        printf(ANSI_COLOR_GREEN);
    }
    else if(type == CIRCLE)
    {
        printf(ANSI_COLOR_MAGENTA);
    }
    else
    {
        printf(ANSI_COLOR_RESET);
    }
}


static void reset_shape(Grid* grid, ShapeHeader* shape)
{
        shape->pixel = ' ';
        shape->type = SHAPE_TYPE_UNKNOWN;
        shape->id = -1;    
}
void init_grid(Grid* grid)
{
    for (int i = 0; i < GRID_HEIGHT * GRID_WIDTH; i++) {
        grid->grid[i] = (ShapeHeader*)malloc(sizeof(ShapeHeader));
        grid->grid[i]->pixel = ' ';
        grid->grid[i]->type = SHAPE_TYPE_UNKNOWN;
        grid->grid[i]->id = -1;
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

ShapeHeader* draw_square(uint8_t origin_x, uint8_t origin_y, uint8_t side, Grid* grid) {
    float aspect_ratio = 1.6;  // Adjust this value based on your terminal's aspect ratio
    uint16_t index = origin_y * grid->width + origin_x;
    free(grid->grid[index]);
    Square* square = (Square*)malloc(sizeof(Square));
    square->side = side;
    square->header.id = ++shape_id;
    square->header.type = SQUARE;
    grid->grid[index] = (ShapeHeader*) square;

    for (int y = 0; y < side; y++) {
        for (int x = 0; x < side; x++) {
            int adjusted_y = origin_y + (int)(y / aspect_ratio);
            if (adjusted_y < grid->height && (origin_x + x) < grid->width) {
                grid->grid[adjusted_y * grid->width + origin_x + x]->pixel = PIXEL;
                grid->grid[adjusted_y * grid->width + origin_x + x]->type = SQUARE;
                grid->grid[adjusted_y * grid->width + origin_x + x]->id = shape_id;
                grid->grid[adjusted_y * grid->width + origin_x + x]->is_head = 0;
            }
        }
    }
    square->header.is_head = 1;
    int label_index = index + (grid->width * side)/2;
    draw_label((ShapeHeader*) square, label_index, "-Label : Square ", grid);
    return (ShapeHeader*) square;
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

void draw_label(ShapeHeader* s, uint16_t index, const char *text, Grid *grid)
{

    if(index >= grid->height * grid->width)
    {
        printf("Label outside grid\n");
        return;
    }
    free(grid->grid[index]);
    Label* label = (Label*) malloc(sizeof(Label));
    strcpy(label->text, text);
    label->header.type = LABEL;
    label->header.id = ++shape_id;
    grid->grid[index] = (ShapeHeader*)label;
    for(int i = 0; i < strlen(label->text); i++) {
        if (index + i < MAX_GRID_SIZE && (index % grid->width) + i < grid->width) {
            grid->grid[index + i]->pixel = label->text[i];
            grid->grid[index + i]->type = LABEL;
            grid->grid[index + i]->id = shape_id;
            grid->grid[index + i]->is_head = 0;
        }
    }
    label->header.is_head = 1;
    s->label = label; // Affect label to shape
}

ShapeHeader* draw_circle(uint8_t origin_x, uint8_t origin_y, uint8_t radius, Grid* grid) {
    float aspect_ratio = 2.0;  // Adjust this value based on your terminal's aspect ratio
    
    // Create a circle
    uint16_t index = origin_y * grid->width + origin_x;
    free(grid->grid[index]);
    Circle* circle = (Circle*) malloc(sizeof(Circle));
    circle->header.type = CIRCLE;
    circle->radius = radius;
    circle->header.id = ++shape_id;
    grid->grid[index] = (ShapeHeader*)circle;
    uint16_t in = 0;
    int x = 0, y = 0;
    for (y = 0; y < grid->height; y++) {
        for (x = 0; x < grid->width; x++) {
            float dx = x - origin_x;
            float dy = (y - origin_y) * aspect_ratio;
            if (dx * dx + dy * dy <= radius * radius) {
                grid->grid[y * grid->width + x]->pixel = PIXEL;
                grid->grid[y * grid->width + x]->type = CIRCLE;
                grid->grid[y * grid->width + x]->id = shape_id;
                grid->grid[y * grid->width + x]->is_head = 0;
            }
        }
    }
    circle->header.is_head = 1;
    draw_label((ShapeHeader*)circle, index + grid->width + (radius * grid->width/aspect_ratio), "-Label : Circle", grid);
    return (ShapeHeader*)circle;
}


void draw_grid(Grid* grid)
{
    for(int i = 0; i < grid->height; i++)
    {
        for(int j = 0; j < grid->width; j++)
        {
            ShapeHeader* shape = grid->grid[i * grid->width + j]; 
            print_color(shape->type);
            printf("%c",shape->pixel);
        }
        printf("\n");
    }
}

void draw_text(Grid* grid, char* text, uint16_t len)
{
    uint16_t text_line = 3;
    uint16_t text_start = (grid->width - len)/2;
    // clean old text
    for(int i = text_line * grid->width + 1; i < text_line * grid->width + grid->width - 1; i++)
    {
        grid->grid[i]->pixel = ' ';
    }
    uint16_t begin = text_line * grid->width + text_start;
    for(uint16_t i = 0; i < len ; i++)
    {
        uint16_t index = begin + i;
        if(index < grid->height * grid->width)
        {
            grid->grid[index]->pixel = text[i];
        }
    }
}

ShapeHeader* draw_line(uint8_t origin_x, uint8_t origin_y, uint8_t length, Grid* grid) {
    uint16_t index = (origin_y * grid->width) + origin_x;
    // Create a line
    free(grid->grid[index]);
    Line* line = (Line*) malloc(sizeof(Line));
    line->header.type = LINE;
    line->header.id = ++shape_id;
    line->header.pixel = PIXEL;
    line->length = length;

    // Polymorphisme here
    grid->grid[index] = (ShapeHeader*)line;
    for(uint16_t i = 0; i < length ; i++) {
        if ((origin_x + i) < grid->width) {
            grid->grid[index + i]->pixel = PIXEL;
            grid->grid[index + i]->type = LINE;
            grid->grid[index + i]->id = shape_id;
            grid->grid[index + i]->is_head = 0;
        }
    }
    line->header.is_head = 1;
    draw_label((ShapeHeader*)line, index + grid->width,"-Label : Line", grid);
    return (ShapeHeader*)line;
}

void draw_shape(enum SHAPE shape, Grid* grid)
{
    ShapeHeader* s = NULL;
    switch (shape)
    {
    case LINE:
        s = draw_line(30, 5, 30, grid);
        break;
    case SQUARE:
        s = draw_square(10, 30, 10, grid);
        break;
    case CIRCLE:
        s = draw_circle(65, 25, 10, grid);
        break;
    case SHAPE_TYPE_UNKNOWN:
        printf("UNKNOWN SHAPE!\n");
        break;
    default:
        printf("NOT SUPPRTED SHAPE!\n");
        break;
    }
    if(s)
    {
        s->id = ++shape_id;
    }
}

void update_shapes(Grid* grid)
{
    static int id = 7;
    int current_id = -1;
    SHAPE_TYPE type = SQUARE;

    ShapeHeader* new_grid[MAX_GRID_SIZE];
    for (int i = 0; i < grid->height * grid->width; i++) {
        new_grid[i] = NULL;
    }
    for(int i = 0 ; i < grid->height; i++)
    {
        for(int j = 0; j < grid->width ; j++)
        {
            ShapeHeader* shape = grid->grid[i * grid->width + j];
            if(shape->is_head && shape->id != -1)
            {
                char tmp[3];
                sprintf(tmp, "%d", shape->id);
                if(strlen(tmp) == 1)
                    shape->pixel = tmp[0];
                else
                {
                    shape->pixel = tmp[0];
                    grid->grid[i * grid->width + j + 1]->pixel = tmp[1];
                }
            }
            if(id == -1 && shape->type == type)
            {
                id = shape->id;
                printf("We delete square id = %d\n", id);
            }
            current_id = shape->id;
            if(id != -1 && shape->type == type && current_id == id)
            {
                reset_shape(grid, shape);                
            }
        }
    }
}

void delete_shape_by_id(Grid* grid, int id)
{
    for(int i = 0 ; i < grid->height; i++)
    {
        for(int j = 0; j < grid->width ; j++)
        {
            ShapeHeader* shape = grid->grid[i * grid->width + j];
            if(id == shape->id)
            {
                reset_shape(grid, shape);
            }
        }
    }
}
