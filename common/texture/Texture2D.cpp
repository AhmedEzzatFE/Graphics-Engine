//
// Created by ahmad on 31/12/2020.
//

#include "Texture2D.h"
Texture2D::Texture2D()
{
    texture=0;
}

Texture2D::Texture2D(const char* file){
    glGenTextures(1, &this->texture);
    our::texture_utils::loadImage(this->texture, file);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}
void Texture2D::ActivateTexture(const char * filename, bool generate_mipmap) {

            if (this->texture)
        {
            glDeleteTextures(1, &this->texture);
        }
        //Generate the Texture and bind them
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        //load new Texture
        our::texture_utils::loadImage(this->texture, filename);
        //Activate and bind the new texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        // for enabling the texture
        glEnable(GL_TEXTURE_2D);

}
void Texture2D::LinkTexture() {

    glActiveTexture(GL_TEXTURE0 );
    glBindTexture(GL_TEXTURE_2D, this->texture);
}
