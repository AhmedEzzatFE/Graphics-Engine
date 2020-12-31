#include "CameraComponent.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>   
 glm::mat4 CameraComponent:: getcameraMatrix()
    {
        glm::mat4 view_matrix = glm::lookAt(eye, eye + direction, up);
        glm::mat4 projection_matrix;
        if( this->type == CameraType::Perspective){
            projection_matrix = glm::perspective(field_of_view_y,aspect_ratio,near,far);
        } else {
            float half_height = orthographic_height * 0.5f;
            float half_width = aspect_ratio * half_height;
            projection_matrix = glm::ortho(-half_width, half_width, -half_height, half_height,near,far);
        }

        glm::mat4 camera_matrix = projection_matrix * view_matrix;
        return camera_matrix;

    }
    