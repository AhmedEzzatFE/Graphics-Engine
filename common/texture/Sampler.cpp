//
// Created by ahmad on 31/12/2020.
//

#include "Sampler.h"


Sampler::Sampler(GLsizei numOfSamplers, GLuint textOption) {
    this->numOfSamplers = numOfSamplers;
    glGenSamplers(numOfSamplers, &samplerObj);
    glBindSampler(textOption, samplerObj);
}
Sampler::Sampler(const Sampler &obj)
{
    this->samplerObj = obj.samplerObj;
    this->numOfSamplers = obj.numOfSamplers;
}


Sampler::~Sampler(){
    glDeleteSamplers(numOfSamplers, &samplerObj);
}
