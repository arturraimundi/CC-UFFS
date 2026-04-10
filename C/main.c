#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>

int main() {
    int w, h, c;

    unsigned char *img = stbi_load("imagem.jpg", &w, &h, &c, 0);

    if (!img) {
        printf("Erro ao carregar imagem\n");
        return 1;
    };
    for(int i = 0, i < int &w, i++ ){
        printf('a');
    };
    printf("w=%d h=%d canais=%d\n", w, h, c);

    stbi_image_free(img);
    return 0;
}