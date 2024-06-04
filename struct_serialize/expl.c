#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Label Label;

typedef struct
{
    int id;
    Label* label;
} ShapeHeader;

struct Label
{
    ShapeHeader header;
    char text[16];
};


int main()
{
    ShapeHeader* s = (ShapeHeader*) malloc(sizeof(ShapeHeader));
    Label* l = (Label*) malloc((sizeof(Label)));
    strcpy(l->text, "Original text!");
    s->label = l;
    strcpy(s->label->text, "Modified text!");
    printf("%s\n", l->text);
    return 0;

}
