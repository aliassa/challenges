#include <stdlib.h>
#include <stdio.h>


typedef struct 
{
    int a;
}Shape;


typedef struct 
{
    Shape* shape;
    int b;
}Line;

void print_shape(Shape* s)
{
    printf("[SHAPE] : a=%d\n", s->a);
}
void print_line(Line* l)
{
    printf("[LINE] : b=%d ", l->b);
    printf("Has shape : ");
    print_shape(l->shape);
}

int main()
{
    Line* line = (Line*) malloc(sizeof(Line));
    line->shape = (Shape*) malloc(sizeof(Shape));
    line->shape->a = 1;
    line->b = 5;


    Shape* sp = (Shape*) malloc(sizeof(Shape));
    sp->a = 0;

    //print_line(line);
    //print_shape(sp);
    // make sp a Line!

    sp = (Shape*)line;
    print_line((Line*)sp);

}