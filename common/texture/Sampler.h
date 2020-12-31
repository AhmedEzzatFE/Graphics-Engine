//
// Created by ahmad on 31/12/2020.
//

#ifndef PH1_SAMPLER_H
#define PH1_SAMPLER_H

#include <application.hpp>
#include <shader.hpp>
#include <imgui-utils/utils.hpp>

#include <texture/texture-utils.h>

#include <unordered_map>

class Sampler {
    GLuint samplerObj;
    GLsizei numOfSamplers;
public:
    Sampler(GLsizei numOfSamplers ,GLuint textOption);
    Sampler(const Sampler &obj);
    ~Sampler();

};


#endif //PH1_SAMPLER_H
