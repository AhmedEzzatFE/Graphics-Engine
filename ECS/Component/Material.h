#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include <shader.hpp>
#include <utility>
#include <imgui-utils/utils.hpp>
#include <string>
class Material{
public:
    our::ShaderProgram * MatrixShader;
    glm::vec4 tint;
    glm::vec3 specularTint, emissive;
    glm::vec3 diffuse, specular, ambient;
    float shininess;
    void init(our::ShaderProgram* ShaderPointer);
    void Initialize();
    void TransformShader(glm::mat4 transformationMatrix,glm::mat4 cameraMatrix);
};

#endif