#include "Material.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"


void Material::init(our::ShaderProgram * ShaderPointer, std::vector<Texture2D *> textures, RenderState * renderstate, Sampler *  sampler){
    MatrixShader = ShaderPointer;
//    diffuse = {0.96,0.9,0.64};
//    specular = {0.4,0.4,0.4};
//    ambient = {0.96, 0.9, 0.64};
//    shininess = 10.0;
//    this->texture = texture;
    this->renderstate = renderstate;
    this->sampler = sampler;

    albedo_tint ={1.0f, 1.0f, 1.0f};
    specular_tint ={1.0f, 1.0f, 1.0f};
    roughness_range ={0.0f, 1.0f};
    emissive_tint = {1.0f, 1.0f, 1.0f};
    albedo_map = textures[0];
    specular_map = textures[1];
    roughness_map = textures[2];
    emissive_map = textures[3];
    ambient_occlusion_map = textures[4];
    
}

void Material::Initialize(){
//    MatrixShader->set("tint", tint);
};
// TODO CHECK IF NEEDED
void Material::TransformShader(glm::mat4 transformationMatrix,glm::mat4 cameraMatrix){
    MatrixShader->set("transform",cameraMatrix*transformationMatrix);
};