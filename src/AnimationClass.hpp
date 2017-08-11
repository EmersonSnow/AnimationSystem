    //
//  baseclass.hpp
//  animationTest
//
//  Created by Zachary Snow on 27/06/2017.
//
//

#pragma once

#include "ofMain.h"
#include "ofxEasing.h"
#include "AnimationDefinitions.hpp"
#include "AnimationUtil.hpp"
#include "AnimationBackend.hpp"

//TODO seperate loop image and loop moving
//TODO add bool if image dies on finish
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
        bInUse = false;
        bStart = false;
        
        bImageDurationSetManually = false;
        
        bImageMultiply = false;
        
        bLoopImage = false;
        bLoopImageBackward = false;
        //bLoopImageBackwardCycle = false;
        
        loopImageNumber = 0;
        loopImageCount = 0;
        
        setPosition(0, 0);
        setRotation(0);
        setDimension(0, 0);
        
        //durationImageMicros = 0;
        //durationEachImageMicros = 0;
        timeStartPlay = 0;
        timeStartLoopImageCycle = 0;
        
        bImagesLoaded = false;
        imageDrawIndex = 0;
        imageNumber = 0;
        
        bImagesFinished = false;
        
        type = ANIMATION_TYPE_IMAGE_SINGLE_STILL;
        finishedState = ANIMATION_FINISH_STATE_STATIC;
    }
    ~AnimationBase()
    {
        
    }
    virtual void setup(string fileDirectory, AnimationType type, float duration, float x, float y, float w, float h, float r)
    {
        this->type = type;
        bImageMultiply = AnimationUtil::getImageMultiply(type);
        
        setTotalDuration(duration);
        
        loadImagesFromDirectory(fileDirectory);
        
        position.x = x;
        position.y = y;
        setDimension(w, h);
        setRotation(r);
        
        bStart = false;
        bLoopImage = false;
        bImagesFinished = false;
        loopState = ANIMATION_LOOP_STATE_NONE;
        
        finishedState = ANIMATION_FINISH_STATE_STATIC;
        
        bInUse = true;
    }
    virtual void reset(AnimationType type = ANIMATION_TYPE_IMAGE_SINGLE_STILL, float x = 0.0, float y = 0.0, float w = 0.0, float h = 0.0, float r = 0.0)
    {
        this->type = type;
        
        bInUse = false;
        bStart = false;
        
        loopState = ANIMATION_LOOP_STATE_NONE;
        bLoopImage = false;
        
        bImagesFinished = false;
        
        
        finishedState = ANIMATION_FINISH_STATE_STATIC;

        durationImages.clear();
        //durationImageMicros = 0;
        //durationEachImageMicros = 0;
        timeStartPlay = 0;
        timeStartLoopImageCycle = 0;
        
        
        setPosition(x, y);
        setDimension(w, h);
        setRotation(r);
        releaseImages();
    }
    inline void setInUse(bool b)
    {
        bInUse = b;
    }
    inline bool getInUse()
    {
        return bInUse;
    }
    virtual unsigned long long setTotalDuration(float duration)
    {
        unsigned long long durationl = (duration * 1000000);
        durationTotal = durationl;
        
        if (bImagesLoaded)
        {
            if (bImageDurationSetManually)
            {
                unsigned long long durationCount = 0;
                for (int i = 0; i < durationImages.size(); i++)
                {
                    durationCount += durationImages[i];
                }
                
                if (durationCount < durationTotal)
                {
                    durationImages[durationImages.size()-1] = durationTotal - durationCount;
                } else if (durationCount > durationTotal)
                {
                    durationImages[durationImages.size()-1] = durationCount - durationTotal;
                }
            } else
            {
                unsigned long long durationPreImage = durationl/images.size();
                for (int i = 0; i < durationImages.size(); i++)
                {
                    durationImages[i] = durationPreImage;
                }
            }
        }
    }
    virtual unsigned long long getTotalDuration()
    {
        return durationTotal;
    }
    virtual void setDurationPreImage(int imageIndex, float duration)
    {
        if (!bImagesLoaded || (imageIndex >= images.size()))
            return;
        
        unsigned long long durationl = (duration * 1000000);
        
        unsigned long long durationCount = 0;
        if (bImagesLoaded)
        {
            for (int i = 0; i < durationImages.size(); i++)
            {
                if (i == imageIndex)
                {
                    durationCount += duration;
                } else
                {
                    durationCount += durationImages[i];
                }
            }
            
            if (durationCount > durationTotal)
            {
                cout << "Can't change the frame duration, it exceed the total duration\n";
                return;
            } else
            {
                durationImages[imageIndex] = durationl;
                bImageDurationSetManually = true;
            }

        }
    }
    inline void setPosition(float x, float y)
    {
        position.x = x;
        position.y = y;
    }
    inline ofVec2f getPosition()
    {
        return position;
    }
    inline void setDimension(float w, float h)
    {
        dimension.x = w;
        dimension.y = h;
    }
    virtual ofVec2f getDimension()
    {
        return dimension;
    }
    inline void setRotation(float r)
    {
        rotation = r;
    }
    inline void getRotation()
    {
        return rotation;
    }
    inline void setLoopImage(bool b)
    {
        bLoopImage = b;
        loopState = ANIMATION_LOOP_STATE_FORWARD;
    }
    inline void getLoop()
    {
        return bLoopImage;
    }
    inline void setLoopImageBackward(bool b)
    {
        bLoopImageBackward = b;
        loopState = ANIMATION_LOOP_STATE_FORWARD;
    }
    inline bool getLoopImageBackward()
    {
        return bLoopImageBackward;
    }
    inline AnimationLoopState getAnimationLoopState()
    {
        return loopState;
    }
    ///Set the number times in an image loops, if 0 it loops infinite
    inline void setLoopImageNumber(int i)
    {
        loopImageNumber = i;
    }
    inline int getLoopImageNumber()
    {
        return loopImageNumber;
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
        //TEMP Clear image durations if new images are loaded
        durationImages.clear();
        bImageDurationSetManually = false;
        
        ofDirectory d(ofToDataPath(fileDirectory));
        d.listDir();
        d.sort();
        
        int numberImagesBuffered = images.size();
        if (!bImageMultiply)
        {
            if (numberImagesBuffered > 0)
            {
                images[0].load(d.getFile(0));
            } else
            {
                images.push_back(*new ofImage);
                images[images.size()-1].load(d.getFile(0));
                imageNumber++;
            }
            durationImages.push_back(durationTotal);
        } else
        {
            unsigned long long durationPreImage = durationTotal/d.size();
            for (int i = 0; i < d.size(); i++)
            {
                if (i < numberImagesBuffered)
                {
                    images[i].load(d.getFile(i));
                    imageNumber++;
                } else
                {
                    //ofImage image;
                    //image.load(d.getFile(i));
                    //images.push_back(image);
                    images.push_back(*new ofImage);
                    images[images.size()-1].load(d.getFile(i));
                    imageNumber++;
                }
                durationImages.push_back(durationPreImage);
            }
        }
        bImagesLoaded = true;
    }
    virtual void setNextDrawImage(int i)
    {
        if (i > imageNumber)
            return;
        imageNumber = i;
    }
    virtual void setFinished()
    {
        switch(finishedState)
        {
            case ANIMATION_FINISH_STATE_STATIC:
            {
                
            }
            case ANIMATION_FINISH_STATE_DELETE:
            {
                reset();
            }
            case ANIMATION_FINISH_STATE_DELETE_FADE:
            {
                //TODO Work out a shader system, with a base shader for operating this sort of thing
            }
        }
    }
    virtual bool isFinished()
    {
        if (bLoopImage)
        {
            if (bStart)
            {
                return false;
            }
            return true;
        } else
        {
            /*if (bInUse && (imageDrawIndex == imageNumber-1) & (ofGetElapsedTimeMicros() >= (timeStartPlay+durationImageMicros)))
            {
                return true;
            }
            return false;*/
        }
    }
    virtual bool isDying()
    {
        if (!isFinished())
            return;
        
        
    }
    virtual void start(unsigned long long time)
    {
        bStart = true;
        timeStartPlay = time;
        timeStartLoopImageCycle = timeStartPlay;
    }
    virtual void restart(unsigned long long time)
    {
        timeStartLoopImageCycle = 0;
        imageDrawIndex = 0;
        timeStartPlay = time;
    }
    virtual bool getPlaying()
    {
        return bStart;
    }
    virtual void stop()
    {
        bStart = false;
    }
    virtual unsigned long long getPreviousDrawnImagesDuration(bool bLoopForward)
    {
        if (bLoopForward)
        {
            if (imageDrawIndex == 0)
            {
                return 0;
            } else if (imageDrawIndex == 1)
            {
                return durationImages[0];
            }
            unsigned long long duration = 0;
            for (int i = 0; i <= imageDrawIndex-1; i++)
            {
                duration += durationImages[i];
            }
            return duration;
        } else
        {
            int imageIndex = images.size()-1;
            if (imageDrawIndex == imageIndex)
            {
                return 0;
            } else if (imageDrawIndex == imageIndex-1)
            {
                return durationImages[imageIndex-1];
            }
            unsigned long long duration = 0;
            for (int i = imageIndex; i > imageDrawIndex; i--)
            {
                duration += durationImages[i];
            }
            return duration;
        }
        
    }
    /*virtual void getNextDrawImage(unsigned long long time)
    {
        if (!bLoopImage)
        {
            
        } else
        {
            if (bLoopImageBackward)
            {
                if (bLoopImageBackwardCycle)
                {
                    
                } else
                {
                    
                        bLoopImageBackwardCycle = true;
                    }
                }
            } else
            {
                if (++imageDrawIndex >= imageNumber)
                {
                    timeStartLoopImageCycle = ofGetElapsedTimeMicros();
                    imageDrawIndex = 0;
                    loopImageCount++;
                }
            }
        }
    }*/
    virtual void update(unsigned long long time)
    {
        if (!bStart & !isFinished())
            return;
        
        //Check all types in base update, as inherited classes will call the base
        if (bImageMultiply)
        {
            switch(loopState)
            {
                case ANIMATION_LOOP_STATE_NONE:
                {
                    if (time >= (timeStartLoopImageCycle+durationImages[imageDrawIndex]+getPreviousDrawnImagesDuration(true)))
                    {
                        if (++imageDrawIndex >= imageNumber)
                        {
                            timeStartLoopImageCycle = time;
                            imageDrawIndex = imageNumber-1;
                            bImagesFinished = true;
                            break;
                        }
                    }
                    break;
                }
                case ANIMATION_LOOP_STATE_FORWARD:
                {
                    if ((loopImageNumber == 0) || (loopImageCount < loopImageNumber))
                    {
                        if (time >= (timeStartLoopImageCycle+durationImages[imageDrawIndex]+getPreviousDrawnImagesDuration(true)))
                        {
                            if (++imageDrawIndex >= imageNumber)
                            {
                                timeStartLoopImageCycle = time;
                                imageDrawIndex = imageNumber-1;
                                loopImageCount++;
                                if (bLoopImageBackward)
                                {
                                    loopState = ANIMATION_LOOP_STATE_BACKWARD;
                                } else
                                {
                                    imageDrawIndex = 0;
                                }
                                break;
                            }
                        }
                    } else
                    {
                        bImagesFinished = true;
                    }
                    break;
                }
                case ANIMATION_LOOP_STATE_BACKWARD:
                {
                    if ((loopImageNumber == 0) || (loopImageCount < loopImageNumber))
                    {
                        if (time >= (timeStartLoopImageCycle+durationImages[imageDrawIndex]+getPreviousDrawnImagesDuration  (false)))
                        {
                            if (--imageDrawIndex <= 0)
                            {
                                timeStartLoopImageCycle = time;
                                imageDrawIndex = 0;
                                loopImageCount++;
                                if (bLoopImageBackward)
                                {
                                    loopState = ANIMATION_LOOP_STATE_FORWARD;
                                }
                                break;
                            }
                        }
                    } else
                    {
                        bImagesFinished = true;
                    }
                    break;
                }
            }
        }
    }
    virtual void draw()
    {
        if (!bStart & !isFinished())
            return;
        //TODO rotation
        ofPushMatrix();
        {
            ofTranslate(position.x+(dimension.x/2), position.y+(dimension.y/2), 0);
            ofRotate(rotation, 0, 0, 1);
            ofPushMatrix();
            {
                ofTranslate(-(position.x+(dimension.x/2)), -(position.y+(dimension.y/2)), 0);
                images[imageDrawIndex].draw(position.x, position.y, dimension.x, dimension.y);
            }
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
    bool bInUse;
    bool bStart;
    
    AnimtionFinishState finishedState;
               
    bool bLoopImage;
    bool bLoopImageBackward;
    AnimationLoopState loopState;
    int loopImageNumber;
    int loopImageCount;
    
    bool bImagesFinished;
    
    AnimationType type;
    ofVec2f position;
    ofVec2f dimension;
    float rotation;
    
    float durationTotal;
    float durationMovement;
    unsigned long long timeStart;
    //unsigned long durationImageMicros;
    //unsigned long durationEachImageMicros;
    unsigned long timeStartPlay;
    unsigned long timeStartLoopImageCycle;
    
    bool bImageDurationSetManually;
    bool bImageMultiply;
    bool bImagesLoaded;
    int imageDrawIndex;
    int imageNumber;
    vector<unsigned long> durationImages;
    vector<ofImage> images;
};




class AnimationMoving : public AnimationBase
{
public:
    AnimationMoving()
    {
        AnimationBase::AnimationBase();
        type = ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING;
        bLoopMovement = false;
        bLoopMovementBackward = false;
        bLoopMovementBackwardCycle = false;
        
        loopMovementNumber = 0;
        loopMovementCount = 0;
        
        timeStartLoopMovementCycle = 0;
        
        setEndDrawPosition(0, 0);
        setCurrentDrawPosition(0, 0);
        setEndDrawDimension(0, 0);
        setCurrentDrawDimension(0, 0);
        setEndDrawRotation(0.0);
        setCurrentDrawRotation(0.0);
    }
    virtual void reset(AnimationType type, float x = 0.0, float y = 0.0, float w = 0.0, float h = 0.0, float r = 0.0, float xDraw = 0.0, float yDraw = 0.0, float wDraw = 0.0, float hDraw = 0.0, float rDraw = 0.0)
    {
        bLoopMovement = false;
        bLoopMovementBackward = false;
        bLoopMovementBackwardCycle = false;
        timeStartLoopMovementCycle = 0;
        setEndDrawPosition(xDraw, yDraw);
        setCurrentDrawPosition(0, 0);
        setEndDrawDimension(wDraw, hDraw);
        setCurrentDrawDimension(0, 0);
        setEndDrawRotation(rDraw);
        setCurrentDrawRotation(0.0);
        AnimationBase::reset(type, x, y, w, h, r);
    }
    virtual void setEndDrawPosition(float x, float y)
    {
        endDrawPosition.x = x;
        endDrawPosition.y = y;
    }
    virtual ofVec2f getEndDrawPosition()
    {
        return endDrawPosition;
    }
    virtual void setCurrentDrawPosition(float x, float y)
    {
        currentDrawPosition.x = x;
        currentDrawPosition.y = y;
    }
    virtual ofVec2f getCurrentDrawPosition()
    {
        return currentDrawPosition;
    }
    virtual void setEndDrawDimension(float w, float h)
    {
        endDrawDimension.x = w;
        endDrawDimension.y = h;
    }
    virtual ofVec2f getEndDrawDimension()
    {
        return endDrawDimension;
    }
    virtual void setCurrentDrawDimension(float w, float h)
    {
        currentDrawDimension.x = h;
        currentDrawDimension.y = h;
    }
    virtual ofVec2f getCurrentDrawDimension()
    {
        return currentDrawDimension;
    }
    virtual void setEndDrawRotation(float r)
    {
        endDrawRotation = r;
    }
    virtual float getEndDrawRotation()
    {
        return endDrawRotation;
    }
    virtual void setCurrentDrawRotation(float r)
    {
        currentDrawRotation = r;
    }
    virtual float getCurrentDrawRotation()
    {
        return currentDrawRotation;
    }
    inline void setLoopMovement(bool b)
    {
        bLoopMovement = b;
    }
    inline bool getLoopMovement(bool b)
    {
        return bLoopMovement;
    }
    ///0 is infinite
    inline void setLoopMovementNumber(int i)
    {
        loopMovementNumber = i;
    }
    inline void setLoopMovementBackward(bool b)
    {
        bLoopMovementBackward = b;
    }
    inline void getLoopMovementBackward()
    {
        return bLoopMovementBackward;
    }
    inline void setLoopMovementDuration(float d)
    {
        timeLoopMovementDuration = (unsigned long long)d*1000000;
        timeLoopMovementDurationf = d;
    }
    inline void getLoopMovementDuration()
    {
        return timeLoopMovementDuration;
    }
    virtual void start(unsigned long long time)
    {
        AnimationBase::start(time);
        timeStartLoopMovementCycle = time;
        timeStartLoopMovementCyclef = ofGetElapsedTimef();
        setCurrentDrawPosition(position.x, position.y);
        setCurrentDrawDimension(dimension.x, dimension.y);
        setCurrentDrawRotation(rotation);
    }
    virtual void restart(unsigned long long time)
    {
        AnimationBase::restart(time);
        timeStartLoopMovementCycle = 0;
        imageDrawIndex = 0;
        timeStartPlay = ofGetElapsedTimeMicros();
    }
    virtual void getNextLoopMovementCycle(unsigned long long time)
    {
        if (bLoopMovement)
        {
            
            cout << "getNextLoopMovementCycle()\n";
        }
    }
    /*virtual void getNextDrawImage(unsigned long long time)
    {
        if (!bLoopImage)
        {
            if (++imageDrawIndex >= imageNumber)
                imageDrawIndex = imageNumber-1;
        } else
        {
            if (bLoopImageBackward)
            {
                if (bLoopImageBackwardCycle)
                {
                    if (--imageDrawIndex <= 0)
                    {
                        timeStartPlayCycle = time;
                        imageDrawIndex = 0;
                        bLoopImageBackwardCycle = false;
                    }
                } else
                {
                    if (++imageDrawIndex >= imageNumber)
                    {
                        timeStartPlayCycle = time;
                        imageDrawIndex = imageNumber-1;
                        bLoopImageBackwardCycle = true;
                    }
                }
            } else
            {
                if (++imageDrawIndex >= imageNumber)
                {
                    timeStartPlayCycle = ofGetElapsedTimeMicros();
                    imageDrawIndex = 0;
                }
            }
        }
    }*/
    virtual void update(unsigned long long time)
    {
        //if (!bStart & !isFinished())
        //    return;
        
        if (bLoopMovement)
        {
            if (time > (timeStartLoopMovementCycle+timeLoopMovementDuration))
            {
                timeStartLoopMovementCycle = time;
                timeStartLoopMovementCyclef = ofGetElapsedTimef();
                if (bLoopMovementBackward)
                    bLoopMovementBackwardCycle = !bLoopMovementBackwardCycle;
            }
        }
        AnimationBase::update(time);
    }
    virtual void calcMovement()
    {
        //Moved to draw because we only need call this on draw
        float timef = ofGetElapsedTimef();
        //float timeStartLoopMovementCyclef = float(timeStartLoopMovementCycle/1000000.0);
        //float timeLoopMovementDurationf = (float(timeLoopMovementDuration)/1000000.0);
        if (bLoopMovementBackward)
        {
            if (bLoopMovementBackwardCycle)
            {
                currentDrawPosition.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, endDrawPosition.x, position.x, &ofxeasing::linear::easeIn);
                currentDrawPosition.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, endDrawPosition.y, position.y, &ofxeasing::linear::easeIn);
                
                currentDrawDimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, endDrawDimension.x, dimension.x, &ofxeasing::linear::easeIn);
                currentDrawDimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, endDrawDimension.y, dimension.y, &ofxeasing::linear::easeIn);
                
                currentDrawRotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, endDrawRotation, rotation, &ofxeasing::linear::easeIn);
            } else
            {
                currentDrawPosition.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, position.x, endDrawPosition.x, &ofxeasing::linear::easeIn);
                currentDrawPosition.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, position.y, endDrawPosition.y, &ofxeasing::linear::easeIn);
                
                currentDrawDimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.x, endDrawDimension.x, &ofxeasing::linear::easeIn);
                currentDrawDimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.y, endDrawDimension.y, &ofxeasing::linear::easeIn);
                
                currentDrawRotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, rotation, endDrawRotation, &ofxeasing::linear::easeIn);
                
            }
        } else
        {
            currentDrawPosition.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, position.x, endDrawPosition.x, &ofxeasing::linear::easeIn);
            currentDrawPosition.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, position.y, endDrawPosition.y, &ofxeasing::linear::easeIn);
            
            currentDrawDimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.x, endDrawDimension.x, &ofxeasing::linear::easeIn);
            currentDrawDimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.y, endDrawDimension.y, &ofxeasing::linear::easeIn);
            
            currentDrawRotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, rotation, endDrawRotation, &ofxeasing::linear::easeIn);
        }

    }
    virtual void draw()
    {
        //cout << this << " Call draw\n";
        switch(AnimationBackend::getMode())
        {
            case ANIMATION_MODE_EDIT:
            {
                ofPushMatrix();
                {
                    ofTranslate(position.x+(dimension.x/2), position.y+(dimension.y/2), 0);
                    ofRotate(rotation, 0, 0, 1);
                    ofPushMatrix();
                    {
                        ofTranslate(-(position.x+(dimension.x/2)), -(position.y+(dimension.y/2)), 0);
                        images[imageDrawIndex].draw(position.x, position.y, dimension.x, dimension.y);
                    }
                }
                ofPopMatrix();
                return;
            }
            case ANIMATION_MODE_VIEW:
            {
                ofPushMatrix();
                {
                    ofTranslate(position.x+(dimension.x/2), position.y+(dimension.y/2), 0);
                    ofRotate(rotation, 0, 0, 1);
                    ofPushMatrix();
                    {
                        ofTranslate(-(position.x+(dimension.x/2)), -(position.y+(dimension.y/2)), 0);
                        images[imageDrawIndex].draw(position.x, position.y, dimension.x, dimension.y);
                    }
                }
                ofPopMatrix();
                return;
            }
            case ANIMATION_MODE_PLAY:
            {
                if (!bStart & !isFinished())
                    return;
                
                calcMovement();
                
                ofPushMatrix();
                {
                    ofTranslate(currentDrawPosition.x+(currentDrawDimension.x/2), currentDrawPosition.y+(currentDrawDimension.y/2), 0);
                    ofRotate(currentDrawRotation, 0, 0, 1);
                    ofPushMatrix();
                    {
                        ofTranslate(-(currentDrawPosition.x+(currentDrawDimension.x/2)), -(currentDrawPosition.y+(currentDrawDimension.y/2)), 0);
                        images[imageDrawIndex].draw(currentDrawPosition.x, currentDrawPosition.y, currentDrawDimension.x, currentDrawDimension.y);
                    }
                    ofPopMatrix();
                }
                ofPopMatrix();
                
                return;
            }
        }
    }
protected:
    ofVec2f currentDrawPosition;
    ofVec2f currentDrawDimension;
    float currentDrawRotation;
    
    bool bLoopMovement;
    bool bLoopMovementBackward;
    bool bLoopMovementBackwardCycle;
    
    int loopMovementNumber;
    int loopMovementCount;
    
    unsigned long long timeStartLoopMovementCycle;
    unsigned long long timeLoopMovementDuration;
    float timeStartLoopMovementCyclef;
    float timeLoopMovementDurationf;
    
    ofVec2f endDrawPosition;
    ofVec2f endDrawDimension;
    float endDrawRotation;
private:
    
};



class AnimationMovingBezier : public AnimationMoving
{
public:
    AnimationMovingBezier()
    {
        AnimationMoving::AnimationMoving();
        bezierIndex = 0;
        bezierPointNumber = 100;
        setBezierPoints(200, 400, 210, 110, 390, 210, 400, 400);
        calcBezierPoints();
        
        type = ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING_BEZIER;
        point.load(ofToDataPath("ui/point.png"));
    }
    virtual void setBezierPoint1(float x, float y)
    {
        bezierPoint1.x = x;
        bezierPoint1.y = y;
        calcBezierPoints();
    }
    virtual void setBezierPoint2(float x, float y)
    {
        bezierPoint2.x = x;
        bezierPoint2.y = y;
        calcBezierPoints();
    }
    virtual void setBezierPoint3(float x, float y)
    {
        bezierPoint3.x = x;
        bezierPoint3.y = y;
        calcBezierPoints();
    }
    virtual void setBezierPoint4(float x, float y)
    {
        bezierPoint4.x = x;
        bezierPoint4.y = y;
        calcBezierPoints();
    }
    void setBezierPoints(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
    {
        bezierPoint1.x = x1;
        bezierPoint1.y = y1;
        bezierPoint2.x = x2;
        bezierPoint2.y = y2;
        bezierPoint3.x = x3;
        bezierPoint3.y = y3;
        bezierPoint4.x = x4;
        bezierPoint4.y = y4;
        calcBezierPoints();
    }
    virtual float calcBezierPoint( float n1 , float n2 , float perc )
    {
        float diff = n2 - n1;
        
        return n1 + ( diff * perc );
    }
    virtual void calcBezierPoints()
    {
        bezierPoints.clear();
        float numberCalc = (1.0/float(bezierPointNumber));
        for( float i = 0 ; i < 1 ; i += numberCalc)
        {
            int xa = calcBezierPoint( bezierPoint1.x , bezierPoint2.x , i );
            int ya = calcBezierPoint( bezierPoint1.y , bezierPoint2.y , i );
            int xb = calcBezierPoint( bezierPoint2.x , bezierPoint3.x , i );
            int yb = calcBezierPoint( bezierPoint2.y , bezierPoint3.y , i );
            int xc = calcBezierPoint( bezierPoint3.x , bezierPoint4.x , i );
            int yc = calcBezierPoint( bezierPoint3.y , bezierPoint4.y , i );
            
            // The Blue Line
            int xm = calcBezierPoint( xa , xb , i );
            int ym = calcBezierPoint( ya , yb , i );
            int xn = calcBezierPoint( xb , xc , i );
            int yn = calcBezierPoint( yb , yc , i );
            
            
            // The Black Dot
            ofVec2f point;
            point.x = calcBezierPoint( xm , xn , i );
            point.y = calcBezierPoint( ym , yn , i );
            bezierPoints.push_back(point);
        }
    }
    virtual void setCalcBezierPointNumber(int n)
    {
        bezierPointNumber = n;
        calcBezierPoints();
    }
    virtual ofVec2f getCalcBezierPoint(int i)
    {
        return bezierPoints[i];
    }
    virtual void calcMovement()
    {
        float timef = ofGetElapsedTimef();
        if (bLoopMovementBackward)
        {
            if (bLoopMovementBackwardCycle)
            {
                bezierIndex = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, bezierPointNumber, 0, &ofxeasing::linear::easeIn);
                
                currentDrawDimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, endDrawDimension.x, dimension.x, &ofxeasing::linear::easeIn);
                currentDrawDimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, endDrawDimension.y, dimension.y, &ofxeasing::linear::easeIn);
                
                currentDrawRotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf,endDrawRotation, rotation, &ofxeasing::linear::easeIn);
                
            } else
            {
                bezierIndex = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+durationTotal, 0, bezierPointNumber, &ofxeasing::linear::easeIn);
                
                currentDrawDimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.x, endDrawDimension.x, &ofxeasing::linear::easeIn);
                currentDrawDimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.y, endDrawDimension.y, &ofxeasing::linear::easeIn);
                
                currentDrawRotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, rotation, endDrawRotation, &ofxeasing::linear::easeIn);
            }
        } else
        {
            bezierIndex = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, 0, bezierPointNumber, &ofxeasing::linear::easeIn);
            
            currentDrawDimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.x, endDrawDimension.x, &ofxeasing::linear::easeIn);
            currentDrawDimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.y, endDrawDimension.y, &ofxeasing::linear::easeIn);
            
            currentDrawRotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, rotation, endDrawRotation, &ofxeasing::linear::easeIn);
        }
    }
    virtual void draw()
    {
        if (!bStart & !isFinished())
            return;
        
        calcMovement();
        
        ofPushMatrix();
        {
            ofTranslate(bezierPoints[bezierIndex].x+(currentDrawDimension.x/2), bezierPoints[bezierIndex].y+(currentDrawDimension.y/2), 0);
            ofRotate(currentDrawRotation, 0, 0, 1);
            ofPushMatrix();
            {
                ofTranslate(-(bezierPoints[bezierIndex].x+(currentDrawDimension.x/2)), -(bezierPoints[bezierIndex].y+(currentDrawDimension.y/2)), 0);
                images[imageDrawIndex].draw(bezierPoints[bezierIndex].x, bezierPoints[bezierIndex].y, currentDrawDimension.x, currentDrawDimension.y);
            }
        }
        ofPopMatrix();
        
        point.draw(bezierPoint1.x-5, bezierPoint1.y-5, 10, 10);
        point.draw(bezierPoint2.x-5, bezierPoint2.y-5, 10, 10);
        point.draw(bezierPoint3.x-5, bezierPoint3.y-5, 10, 10);
        point.draw(bezierPoint4.x-5, bezierPoint4.y-5, 10, 10);
    }
protected:
    int bezierIndex;
    int bezierPointNumber;
    
    vector<ofVec2f> bezierPoints;
    ofVec2f bezierPoint1;
    ofVec2f bezierPoint2;
    ofVec2f bezierPoint3;
    ofVec2f bezierPoint4;
    
    ofImage point;
    
};
