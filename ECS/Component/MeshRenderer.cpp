#include "MeshRenderer.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
void MeshRendererComponent::init(our::ShaderProgram* x,our::Mesh *y){

      shader=x;
      meshPtr=y;
};
void MeshRendererComponent:: update(){};
void MeshRendererComponent::draw(glm::mat4 transformationMatrix,glm::mat4 cameraMatrix){

        
       

       // glm::mat4 transformation_matrix = glm::mat4(1.0f) ;
        //transformation_matrix = glm::rotate(transformation_matrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //std::cout<<glm::to_string(transformationMatrix)<<std::endl;
        shader->set("tint", glm::vec4(1,1,1,1));
        shader->set("transform",cameraMatrix*transformationMatrix);
        meshPtr->draw();
        

        //NOTE: Remember to reset the color mask such that ImGUI can draw
        glColorMask(true, true, true, true);
};
void MeshRendererComponent::destroy(){
   
}