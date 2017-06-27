//
//  baseclass.hpp
//  animationTest
//
//  Created by Zachary Snow on 27/06/2017.
//
//

#pragma once

#include "ofMain.h"

/*class vec2i
{
public:
    vec2i()
    {
        x = 0;
        y = 0;
    };
    int x;
    int y;
    
};*/

//I'm creating a 2D animation system
class AnimationBase
{
public:
    AnimationBase()
    {
        setPosition(0, 0);
        setRotation(0);
        setDimension(0, 0);
        
        bImagesLoaded = false;
        imageDrawIndex = 0;
        imageNumber = 0;
        /*imageBuffer = NULL;
        imageBufferPosition = NULL;
        imageBufferEnd = NULL;
        imageBufferLength = 0;*/
    }
    virtual void setup(float x, float y, float r)
    {
        setPosition(x, y);
        setRotation(r);
        reset();
    }
    virtual void reset(float x = 0.0, float y = 0.0, float w = 0.0, float h = 0.0, float r = 0.0)
    {
        setPosition(x, y);
        setDimension(w, h);
        setRotation(r);
        releaseImages();
    }
    virtual void setPosition(float x, float y)
    {
        position.x = x;
        position.y = y;
    }
    virtual ofVec2f getPosition()
    {
        return position;
    }
    virtual void setDimension(float w, float h)
    {
        dimension.x = w;
        dimension.y = h;
    }
    virtual ofVec2f getDimension()
    {
        return dimension;
    }
    virtual void setRotation(float r)
    {
        rotation = r;
    }
    virtual void getRotation()
    {
        return rotation;
    }
    virtual void releaseImages()
    {
        for (int i = 0; i < images.size(); i++)
        {
            images.clear();
        }
        imageDrawIndex = 0;
        imageNumber = 0;
        bImagesLoaded = false;
    }
    virtual void loadImagesFromDirectory(string fileDirectory)
    {
        releaseImages();
        
        ofDirectory d(ofToDataPath(fileDirectory));
        d.listDir();
        d.sort();
        int numImgObjBuf = images.size();
        for (int i = 0; i < d.size(); i++)
        {
            if (i < numImgObjBuf)
            {
                images[i]->load(d.getFile(i));
            } else
            {
                ofImage image;
                image.load(d.getFile(i));
                images.push_back(&image);
            }
        }
        bImagesLoaded = true;
    }
    virtual void getNextDrawImage()
    {
        if (++imageDrawIndex > imageNumber)
            imageDrawIndex = 0;
    }
    virtual void setNextDrawImage(int i)
    {
        if (i > imageNumber)
            return;
        imageNumber = i;
    }
    virtual void draw()
    {
        //TODO rotation
        ofPushMatrix();
        {
            ofRotateX(rotation);
            images[imageDrawIndex]->draw(position.x, position.y, dimension.x, dimension.y);
        }
        ofPopMatrix();
    }
    /*virtual void allocateImageBuffer(int bufferSize)
    {
        imageBuffer = new ofImage[bufferSize];
        imageBufferEnd = imageBuffer + imageBufferLength;
    }
    void clearImageBuffer()
    {
        imageBufferPosition = imageBuffer;
        while (imageBufferPosition < imageBufferEnd)
        {
            *imageBufferPosition.clear();
            imageBufferPosition++;
        }
    }*/
protected:
    ofVec2f position;
    ofVec2f dimension;
    float rotation;
    
    bool bImagesLoaded;
    int imageDrawIndex;
    int imageNumber;
    vector<ofImage*> images;
};
