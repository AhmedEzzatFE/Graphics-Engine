#include <shader.hpp>
#include <utility>
#include <imgui-utils/utils.hpp>
#include <mesh/mesh.hpp>
#include <mesh/mesh-utils.hpp>
#include <string>
#include "Component.h"
class MeshRendererComponent : public Component{
   public:
        
        our::Mesh * meshPtr;
        our::ShaderProgram * shader; // should be ptr
        void init(our::ShaderProgram *x,our::Mesh *y);
        void update();
        void draw(glm::mat4 transformationMatrix,glm::mat4 cameraMatrix);
        void destroy();

};