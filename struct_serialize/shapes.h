#include <stdint.h>
#define MAX_GRID_SIZE 100000
#define MAX_GRID_NAME 124
#define TEXT_LINE 10
#define GRID_WIDTH 100
#define GRID_HEIGHT 50
#define PIXEL '*'
typedef enum SHAPE
{
    SHAPE_TYPE_UNKNOWN, // pixel == ' ' initial value
    LINE,
    CIRCLE,
    SQUARE,
    BORDER,
    COUNT
}SHAPE_TYPE;

typedef struct
{
    char pixel;
    SHAPE_TYPE type;
} ShapeHeader;



typedef struct {
    ShapeHeader* header;
    uint16_t length;
}Line;

typedef struct {
    ShapeHeader* header;
    uint16_t side;
}Square;

typedef struct {
    ShapeHeader* header;
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
void draw_line(uint8_t origin_x, uint8_t origin_y, uint8_t length, Grid* grid);
void draw_circle(uint8_t origin_x, uint8_t origin_y, uint8_t radius, Grid* grid);
void draw_square(uint8_t origin_x, uint8_t origin_y, uint8_t side, Grid* grid);
void find_line(Grid* grid);