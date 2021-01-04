#include "Material.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

/*void Material::settint(glm::vec4 t){
    this.tint = t ;
};*/

void Material::init(our::ShaderProgram * ShaderPointer){
    MatrixShader = ShaderPointer;
    diffuse = {0.96,0.9,0.64};
    specular = {0.4,0.4,0.4};
    ambient = {0.96, 0.9, 0.64};
    shininess = 10.0;
}

void Material::Initialize(){
    MatrixShader->set("tint", tint);
};

void Material::TransformShader(glm::mat4 transformationMatrix,glm::mat4 cameraMatrix){
    MatrixShader->set("transform",cameraMatrix*transformationMatrix);
};