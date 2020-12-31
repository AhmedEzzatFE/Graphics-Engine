//
// Created by ahmad on 31/12/2020.
//

#include "Texture2D.h"


Texture2D::Texture2D(int option){
    glGenTextures(1, &texture);
    uint8_t pixel_data[] = {
            255,   0,   0, 255,
            0, 255,   0, 255,
            0,   0, 255, 255,
            255, 255,   0, 255,
    };
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

 Texture2D::~Texture2D()
{
    glDeleteTextures(1, &texture);
}

