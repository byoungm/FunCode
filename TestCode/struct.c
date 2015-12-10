#include <stdio.h>
#include <stdlib.h>

// Normal Types
typedef char INT8;
typedef short INT16;
typedef int INT32;
typedef long INT64;


typedef struct 
{
    char* name;
    INT16 height ;
    INT16 weight ;

}Foo_t;

static Foo_t *fooArray[10];

int main()
{
    Foo_t *foo = malloc(sizeof(Foo_t));
    fooArray[0] = foo;

    foo->name = malloc(15);
    foo->name = "Benjamin";
    foo->height = 68;
    foo->weight = 151;

    printf("\nMy Name is %s, weight = %d, height = %d", fooArray[0]->name, foo->weight, foo->height );

    printf("\n");
    return 0;
}       