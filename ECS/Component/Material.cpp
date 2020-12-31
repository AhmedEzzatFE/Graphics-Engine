//
// Created by ahmad on 31/12/2020.
//

#include "Material.h"

Material::Material(GLuint* shader, _map uniformMap) {
    this->shader = shader;
    this->uniformMap = uniformMap;
}