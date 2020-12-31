//
// Created by ahmad on 31/12/2020.
//

#ifndef PH1_TEXTURE2D_H
#define PH1_TEXTURE2D_H
#include <application.hpp>
#include <shader.hpp>
#include <imgui-utils/utils.hpp>

#include <texture/texture-utils.h>

#include <unordered_map>

class Texture2D {
    GLuint texture;


public:
    Texture2D(int option);
    void onDestroy();
    ~Texture2D();
};


#endif //PH1_TEXTURE2D_H
