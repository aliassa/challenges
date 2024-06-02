#include "serialize_image.h"


void test()
{
    Image img;
    img.name = "Ali and Ilyes TEST";
    img.name_len = strlen(img.name);
    img.dimensions.width = 50;
    img.dimensions.height = 30;
    img.type  = SQUARE;
    size_t s_im1 = sizeof(Image) + img.name_len - sizeof(char*);
    char* buff;
    serialize_image_to_buffer(&img, &buff);
    print_info(img);
    Image out;
    deserialize_image_from_buffer(buff, &out);
    size_t s_im2 = sizeof(Image) + out.name_len - sizeof(char*);
    print_info(out);
    

    if(s_im1 != s_im2)
        printf(" [FAIL] : Size test\n");
    else
        printf(" [OK] : Size test\n");
    
    if(
        strcmp(img.name, out.name) == 0 &&
        img.type == out.type &&
        img.dimensions.height == out.dimensions.height &&
        img.dimensions.width == out.dimensions.width &&
        memcmp(img.header , out.header, 4) == 0
    )
        printf (" [OK] Content test\n");
    else
        printf (" [FAIL] Content test\n");

}



int main()
{
    
    test();
    return 0;
}
