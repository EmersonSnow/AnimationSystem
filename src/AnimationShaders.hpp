//
//  AnimationShaders.hpp
//  animationTest
//
//  Created by Zachary Snow on 29/08/2017.
//
//

#pragma once

#include "AnimationDefinitions.hpp"
#include "AnimationBackend.hpp"

class AnimationShaderBase
{
public:
    AnimationShaderBase()
    {
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex.str());
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment.str());
        
    };
    virtual void setup()
    {
        
    };
    virtual ofShader & getShader()
    {
        return shader;
    }
protected:
    ofShader shader;
    stringstream vertex;
    stringstream fragment;
};

class AnimationShaderFadeTwoImages : public AnimationShaderBase
{
public:
    AnimationShaderFadeTwoImages()
    {
        vertex << "#version 150\n";
        vertex << "";
    };
protected:
    
};
