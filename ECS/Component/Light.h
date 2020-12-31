//
// Created by ahmad on 31/12/2020.
//

#ifndef PH1_LIGHT_H
#define PH1_LIGHT_H
#include <application.hpp>

enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

class Light {

        LightType type;
        glm::vec3 diffuse, specular, ambient;
        glm::vec3 position; // Used for Point and Spot Lights only
        glm::vec3 direction; // Used for Directional and Spot Lights only
        struct {
            float constant, linear, quadratic;
        } attenuation; // Used for Point and Spot Lights only
        struct {
            float inner, outer;
        } spot_angle; // Used for Spot Lights only

};


#endif //PH1_LIGHT_H
