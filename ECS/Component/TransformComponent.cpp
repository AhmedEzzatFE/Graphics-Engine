#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
    position = {1, 0, 0};
    rotation = {0, 1, 1};
    scale = {1, 1, 5};
}

[[nodiscard]] glm::mat4 TransformComponent::to_mat4() const
{
    return glm::translate(glm::mat4(1.0f), position) *
           glm::yawPitchRoll(rotation.y, rotation.x, rotation.z) *
           glm::scale(glm::mat4(1.0f), scale);
}

void TransformComponent::init(our::Application *application, glm::vec3 pos, glm::vec3 rot, glm::vec3 sc)
{
    this->app = application;
    position = pos;
    rotation = rot;
    scale = sc;
}
void TransformComponent::update(double delta_time)
{
    
    double factor = 10 * delta_time;
   
    if (app->getKeyboard().isPressed(GLFW_KEY_UP))
    {
        position[1] = position[1] + factor;
        rotation[0] = rotation[0] + factor;
    
    }
    if (app->getKeyboard().isPressed(GLFW_KEY_DOWN))
    {
        position[1] = position[1] - factor;
        rotation[0] = rotation[0] -factor;
    }
    if (app->getKeyboard().isPressed(GLFW_KEY_RIGHT))
    {
        position[0] = position[0] + factor;
        rotation[2] = rotation[2] + factor;
   
    }
    if (app->getKeyboard().isPressed(GLFW_KEY_LEFT))
    {
        position[0] = position[0] - factor;
           rotation[2] = rotation[2] - factor;
   
    }
    if (app->getKeyboard().isPressed(GLFW_KEY_H))
    {
        position[2] = position[2] + factor;
           rotation[1] = rotation[1] + factor;
   
    }
    if (app->getKeyboard().isPressed(GLFW_KEY_G))
    {
        position[2] = position[2] - factor;
        rotation[1] = rotation[1] - factor;
   
    }
    //   if(app->getKeyboard().isPressed(GLFW_KEY_S)) position -= front * ((float)delta_time * current_sensitivity.z);
    // if(app->getKeyboard().isPressed(GLFW_KEY_Q)) position += up * ((float)delta_time * current_sensitivity.y);
    // if(app->getKeyboard().isPressed(GLFW_KEY_E)) position -= up * ((float)delta_time * current_sensitivity.y);
    // if(app->getKeyboard().isPressed(GLFW_KEY_D)) position += right * ((float)delta_time * current_sensitivity.x);
    // if(app->getKeyboard().isPressed(GLFW_KEY_A)) position -= right * ((float)delta_time * current_sensitivity.x);

}
void TransformComponent::fall(double delta_time, int speed)
{
    double factor = speed * delta_time;
   
     position[1] = position[1] - factor;
        rotation[0] = rotation[0] -factor;
        
}

