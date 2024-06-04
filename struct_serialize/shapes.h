#pragma once
#include <stdint.h>
#define MAX_GRID_SIZE 100000
#define MAX_GRID_NAME 124
#define TEXT_LINE 10
#define GRID_WIDTH 100
#define GRID_HEIGHT 50
#define PIXEL '*'
#define HIDDEN 'h'

// global id, new for each item

typedef enum SHAPE
{
    SHAPE_TYPE_UNKNOWN, // pixel == ' ' initial value
    LINE,
    CIRCLE,
    SQUARE,
    BORDER,
    LABEL,
    COUNT
}SHAPE_TYPE;

typedef struct Label Label;

typedef struct
{
    char pixel;
    SHAPE_TYPE type;
    Label* label;
    int id;
} ShapeHeader;

struct Label
{
    ShapeHeader header;
    char text[16];
};

typedef struct {
    ShapeHeader header;
    uint16_t length;
}Line;

typedef struct {
    ShapeHeader header;
    uint16_t side;
}Square;

typedef struct {
    ShapeHeader header;
    uint16_t radius;
}Circle;

typedef struct 
{
    ShapeHeader* grid[MAX_GRID_SIZE];
    //char name[MAX_GRID_NAME];
    uint16_t height;
    uint16_t width;

}Grid;

void init_grid(Grid* grid);
void draw_grid(Grid* grid);
void draw_text(Grid* grid, char* text, uint16_t len);

void draw_shape(enum SHAPE shape, Grid* grid); // depending on type enum call appropriate draw func

ShapeHeader* draw_line(uint8_t origin_x, uint8_t origin_y, uint8_t length, Grid* grid);
ShapeHeader* draw_circle(uint8_t origin_x, uint8_t origin_y, uint8_t radius, Grid* grid);
ShapeHeader* draw_square(uint8_t origin_x, uint8_t origin_y, uint8_t side, Grid* grid);

void find_shapes(Grid* grid);
void hide_lines(Grid* grid);
void draw_label(ShapeHeader* s, uint16_t index, const char* text, Grid* grid);
void update_shapes(Grid* grid);