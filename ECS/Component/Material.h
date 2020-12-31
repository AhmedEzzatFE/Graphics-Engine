//
// Created by ahmad on 31/12/2020.
//

#ifndef PH1_MATERIAL_H
#define PH1_MATERIAL_H


#include <application.hpp>
#include <shader.hpp>
#include <imgui-utils/utils.hpp>
#include <texture/texture-utils.h>
#include <unordered_map>
#include <string>
#include <any>

#define _map std::unordered_map<std::string, std::vector<std::any>>

class Material {
public:
    GLuint* shader;
    _map uniformMap;
    Material(GLuint* shader, std::unordered_map<std::string, std::vector<std::any>>);
};


#endif //PH1_MATERIAL_H
