#include "material.h"
#include "stb_image.h"

Material::Material(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha); // data is an array of unsigned bytes

    // make the texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // upload data (to the graphics card)
    glTexImage2D(GL_TEXTURE_2D, 
        0, GL_RGBA, width, height, 0, 
        GL_RGBA, GL_UNSIGNED_BYTE, data); 

    // free data (b/c it's now there on the GPU)
    stbi_image_free(data);

    // configure the sampler
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // minifying filter describes how to sample an image when it is shrunk down
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnifying filter 
}

Material::~Material() { // (destructor)
    glDeleteTextures(1, &texture); // 1 = num textures we're deleting
}

void Material::use(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture); // binding texture #2, texture #3, etc
}