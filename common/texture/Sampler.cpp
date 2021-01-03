//
// Created by ahmad on 31/12/2020.
//

#include "Sampler.h"

Sampler::Sampler() {samplerObj=0;}
void Sampler::InitializeSampler()
{  glGenSamplers(1, &this->samplerObj);}

void Sampler::DestroySampler()
{  glDeleteSamplers(1, &samplerObj);}

void Sampler::AttachSample()
{   glBindSampler(0, samplerObj);
    glSamplerParameteri(samplerObj, GL_TEXTURE_MAG_FILTER, magnification_filter);
    glSamplerParameteri(samplerObj, GL_TEXTURE_MIN_FILTER, minification_filter);
    glSamplerParameteri(samplerObj, GL_TEXTURE_WRAP_S, wrap_s);
    glSamplerParameteri(samplerObj, GL_TEXTURE_WRAP_T, wrap_t);
    glSamplerParameterfv(samplerObj, GL_TEXTURE_BORDER_COLOR,  glm::value_ptr(border_color));
    glSamplerParameterf(samplerObj, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);
}
void Sampler::GenSampler()
{    glGenSamplers(1, &this->samplerObj); }



