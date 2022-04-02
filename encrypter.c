#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "crypt.h"

int main(int argc, char **argv)
{
    char *salt = "aa";
    char *text = argv[1];
    printf("Crypt: %s\n", crypt(text, salt));
}