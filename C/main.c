#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>

int main() {
    int w, h, c;

    unsigned char *img = stbi_load("imagem.jpg", &w, &h, &c, 0);

    if (!img) {
        printf("Erro ao carregar imagem\n");
        return 1;
    }

    printf("w=%d h=%d canais=%d\n", w, h, c);

    unsigned char *p = img;

    for (int i = 0; i < w * h; i++) {
        unsigned char r = *p++;
        unsigned char g = *p++;
        unsigned char b = *p++;

        printf("R=%d G=%d B=%d\n", r, g, b);

        if (c == 4) p++; // pula alpha
    }

    stbi_image_free(img);
    return 0;
}