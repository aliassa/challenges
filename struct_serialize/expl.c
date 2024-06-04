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


int main()
{
    Line* line = (Line*) malloc(sizeof(Line));
    line->shape = (Shape*) malloc(sizeof(Shape));
    line->shape->a = 1;
    line->b = 5;


    Shape* sp = (Shape*) malloc(sizeof(Shape));
    sp->a = 0;

    sp = (Shape*)line;
    printf("%d\n", ((Line*)sp)->b);

}
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