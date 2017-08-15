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

//TODO Add movement finsihed bool
//TODO add image animating finished bool
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
class AnimationObjectBase
{
public:
    AnimationObjectBase()
    {
        bInUse = false;
        bStart = false;
        
        bImageDurationSetManually = false;
        
        bImageMultiple = false;
        
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
        
        //bImagesLoaded = false;
        imageDrawIndex = 0;
        //imageNumber = 0;
        
        bImagesFinished = false;
        
        type = ANIMATION_TYPE_IMAGE_SINGLE_STILL;
        finishedState = ANIMATION_FINISH_STATE_STATIC;
    }
    ~AnimationObjectBase()
    {
        
    }
    virtual void setup(string fileDirectory, AnimationType type, float duration, float x, float y, float w, float h, float r)
    {
        this->type = type;
        bImageMultiple = AnimationUtil::getImageMultiple(type);
        
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
        
        /*if (bImagesLoaded)
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
        }*/
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
    inline bool getLoopImage()
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
        
        imagesContainerIndex = AnimationBackend::loadImages(fileDirectory)
       /*ofDirectory d(ofToDataPath(fileDirectory));
        d.listDir();
        d.sort();
        
        int numberImagesBuffered = images.size();
        if (!bImageMultiple)
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
        bImagesLoaded = true;*/
    }
    virtual void setNextDrawImage(int i)
    {
        if (i > imageNumber)
            return;
        imageNumber = i;
    }
    virtual bool isPlaying()
    {
        return (bStart && isFinished());
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
        //TODO Revise how finish is calucated
        return false;
        //        if (bLoopImage)
        //        {
        //            if (bStart)
        //            {
        //                return false;
        //            }
        //            return true;
        //        } else
        //        {
        //            /*if (bInUse && (imageDrawIndex == imageNumber-1) & (ofGetElapsedTimeMicros() >= (timeStartPlay+durationImageMicros)))
        //            {
        //                return true;
        //            }
        //            return false;*/
        //        }
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
    virtual void calcuateLoopImageState(unsigned long long time)
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
                    if (time >= (timeStartLoopImageCycle+durationImages[imageDrawIndex]+getPreviousDrawnImagesDuration(false)))
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
    virtual void update(unsigned long long time)
    {
        //Check all types in base update, as inherited classes will call the base
        switch(AnimationBackend::getMode())
        {
            case ANIMATION_MODE_EDIT:
            {
                if (bStart && !isFinished())
                {
                    if (bImageMultiple)
                    {
                        calcuateLoopImageState(time);
                    }
                }
            }
            case ANIMATION_MODE_VIEW:
            {
                if (bStart && !isFinished())
                {
                    if (bImageMultiple)
                    {
                        //cout << bStart << " Start, " << isFinished() << " , finsihed" << "\n";
                        calcuateLoopImageState(time);
                    }
                }
            }
            case ANIMATION_MODE_PLAY:
            {
                if (bStart && !isFinished())
                {
                    if (bImageMultiple)
                    {
                        calcuateLoopImageState(time);
                    }
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
                Animationimages[imageDrawIndex].draw(position.x, position.y, dimension.x, dimension.y);
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
    unsigned long long timeStartPlay;
    unsigned long long timeStartLoopImageCycle;
    
    
    bool bImageDurationSetManually;
    bool bImageMultiple;
    //bool bImagesLoaded;
    int imageDrawIndex;
    int imagesContainerIndex;
    //int imageNumber;
    vector<unsigned long> durationImages;
    //vector<ofImage> images;
    
};




class AnimationObjectMoving : public AnimationObjectBase
{
public:
    AnimationObjectMoving()
    {
        AnimationObjectBase::AnimationObjectBase();
        type = ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING;
        bLoopMovement = false;
        bLoopMovementBackward = false;
        bLoopMovementBackwardCycle = false;
        
        loopMovementNumber = 0;
        loopMovementCount = 0;
        
        timeLoopMovementDuration = 0;
        timeLastMovementUpdate = 0;
        timeStartLoopMovementCyclef = 0.0;
        timeLoopMovementDurationf = 0.0;
        timeStartLoopMovementCycle = 0;
        timeLastMovementUpdate = 0;
        
        setMovementContainerPosition(animationMovingContainer, 0, 0);
        setCurrentDrawPosition(0, 0);
        setMovementContainerDimension(animationMovingContainer, 0, 0);
        setCurrentDrawDimension(0, 0);
        setMovementContainerRotation(animationMovingContainer, 0.0);
        setCurrentDrawContainer(0.0);
    }
    virtual void reset(AnimationType type, float x = 0.0, float y = 0.0, float w = 0.0, float h = 0.0, float r = 0.0, float xDraw = 0.0, float yDraw = 0.0, float wDraw = 0.0, float hDraw = 0.0, float rDraw = 0.0)
    {
        bLoopMovement = false;
        bLoopMovementBackward = false;
        bLoopMovementBackwardCycle = false;
        timeStartLoopMovementCycle = 0;
        setMovementContainerPosition(animationMovingContainer, xDraw, yDraw);
        setCurrentDrawPosition(0, 0);
        setMovementContainerDimension(animationMovingContainer, wDraw, hDraw);
        setCurrentDrawDimension(0, 0);
        setMovementContainerRotation(animationMovingContainer, rDraw);
        setCurrentDrawContainer(0.0);
        AnimationObjectBase::reset(type, x, y, w, h, r);
    }
    virtual void setMovementContainerPosition(AnimationMovingContainer & movingContainer, float x, float y)
    {
        movingContainer.position.x = x;
        movingContainer.position.y = y;
    }
    virtual ofVec2f getMovementContainerPosition(AnimationMovingContainer & movingContainer)
    {
        return movingContainer.position;
    }
    virtual void setCurrentDrawPosition(float x, float y)
    {
        currentDrawContainer.position.x = x;
        currentDrawContainer.position.y = y;
    }
    virtual ofVec2f getCurrentDrawPosition()
    {
        return currentDrawContainer.position;
    }
    virtual void setMovementContainerDimension(AnimationMovingContainer & movingContainer, float w, float h)
    {
        movingContainer.dimension.x = w;
        movingContainer.dimension.y = h;
    }
    virtual ofVec2f getMovementContainerDimension(AnimationMovingContainer & movingContainer)
    {
        return movingContainer.dimension;
    }
    virtual void setCurrentDrawDimension(float w, float h)
    {
        currentDrawContainer.dimension.x = w;
        currentDrawContainer.dimension.y = h;
    }
    virtual ofVec2f getCurrentDrawDimension()
    {
        return currentDrawContainer.dimension;
    }
    virtual void setMovementContainerRotation(AnimationMovingContainer & movingContainer, float r)
    {
        movingContainer.rotation = r;
    }
    virtual float getEndDrawRotation(AnimationMovingContainer & movingContainer)
    {
        return movingContainer.rotation;
    }
    virtual void setCurrentDrawContainer(float r)
    {
        currentDrawContainer.rotation = r;
    }
    virtual float getCurrentDrawContainer()
    {
        return currentDrawContainer.rotation;
    }
    inline void setLoopMovement(bool b)
    {
        bLoopMovement = b;
    }
    inline bool getLoopMovement()
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
    inline bool getLoopMovementBackward()
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
        AnimationObjectBase::start(time);
        timeLastMovementUpdate = time;
        timeStartLoopMovementCycle = time;
        timeStartLoopMovementCyclef = ofGetElapsedTimef();
        setCurrentDrawPosition(position.x, position.y);
        setCurrentDrawDimension(dimension.x, dimension.y);
        setCurrentDrawContainer(rotation);
    }
    virtual void restart(unsigned long long time)
    {
        AnimationObjectBase::restart(time);
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
    virtual void calcuateMovement()
    {
        //Moved to draw because we only need call this on draw
        float timef = ofGetElapsedTimef();
        //float timeStartLoopMovementCyclef = float(timeStartLoopMovementCycle/1000000.0);
        //float timeLoopMovementDurationf = (float(timeLoopMovementDuration)/1000000.0);
        if (bLoopMovementBackward)
        {
            if (bLoopMovementBackwardCycle)
            {
                currentDrawContainer.position.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, animationMovingContainer.position.x, position.x, &ofxeasing::linear::easeIn);
                currentDrawContainer.position.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, animationMovingContainer.position.y, position.y, &ofxeasing::linear::easeIn);
                
                currentDrawContainer.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, animationMovingContainer.dimension.x, dimension.x, &ofxeasing::linear::easeIn);
                currentDrawContainer.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, animationMovingContainer.dimension.y, dimension.y, &ofxeasing::linear::easeIn);
                
                currentDrawContainer.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, animationMovingContainer.rotation, rotation, &ofxeasing::linear::easeIn);
            } else
            {
                currentDrawContainer.position.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, position.x, animationMovingContainer.position.x, &ofxeasing::linear::easeIn);
                currentDrawContainer.position.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, position.y, animationMovingContainer.position.y, &ofxeasing::linear::easeIn);
                
                currentDrawContainer.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.x, animationMovingContainer.position.x, &ofxeasing::linear::easeIn);
                currentDrawContainer.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.y, animationMovingContainer.dimension.y, &ofxeasing::linear::easeIn);
                
                currentDrawContainer.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, rotation, animationMovingContainer.rotation, &ofxeasing::linear::easeIn);
                
            }
        } else
        {
            currentDrawContainer.position.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, position.x, animationMovingContainer.position.x, &ofxeasing::linear::easeIn);
            animationMovingContainer.position.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, position.y, animationMovingContainer.position.y, &ofxeasing::linear::easeIn);
            
            currentDrawContainer.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.x, animationMovingContainer.dimension.x, &ofxeasing::linear::easeIn);
            currentDrawContainer.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.y, animationMovingContainer.dimension.y, &ofxeasing::linear::easeIn);
            
            currentDrawContainer.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, rotation, animationMovingContainer.rotation, &ofxeasing::linear::easeIn);
        }
    }
    virtual void update(unsigned long long time)
    {
        //if (!bStart & !isFinished())
        //    return;
        AnimationObjectBase::update(time);
        
        switch(AnimationBackend::getMode())
        {
            case ANIMATION_MODE_EDIT:
            {
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
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement();
                    timeLastMovementUpdate = time;
                }
                break;
            }
            case ANIMATION_MODE_VIEW:
            {
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
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement();
                    timeLastMovementUpdate = time;
                }
                break;
            }
            case ANIMATION_MODE_PLAY:
            {
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
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement();
                    timeLastMovementUpdate = time;
                }
                break;
            }
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
                
                calcuateMovement();
                
                ofPushMatrix();
                {
                    ofTranslate(currentDrawContainer.position.x+(currentDrawContainer.dimension.x/2), currentDrawContainer.position.y+(currentDrawContainer.dimension.y/2), 0);
                    ofRotate(currentDrawContainer.rotation, 0, 0, 1);
                    ofPushMatrix();
                    {
                        ofTranslate(-(currentDrawContainer.position.x+(currentDrawContainer.dimension.x/2)), -(currentDrawContainer.position.y+(currentDrawContainer.dimension.y/2)), 0);
                        images[imageDrawIndex].draw(currentDrawContainer.position.x, currentDrawContainer.position.y, currentDrawContainer.dimension.x, currentDrawContainer.dimension.y);
                    }
                    ofPopMatrix();
                }
                ofPopMatrix();
                
                return;
            }
        }
    }
    
    //TEMP
    AnimationMovingContainer animationMovingContainer;
protected:
    //ofVec2f currentDrawContainer.position;
    //ofVec2f currentDrawContainer.dimension;
    //float  currentDrawContainer.rotation;
    AnimationMovingContainer currentDrawContainer;
    bool bLoopMovement;
    bool bLoopMovementBackward;
    bool bLoopMovementBackwardCycle;
    
    int loopMovementNumber;
    int loopMovementCount;
    
    unsigned long long timeStartLoopMovementCycle;
    unsigned long long timeLoopMovementDuration;
    unsigned long long timeLastMovementUpdate;
    float timeStartLoopMovementCyclef;
    float timeLoopMovementDurationf;
    
    
private:
};



class AnimationObjectMovingBezier : public AnimationObjectMoving
{
public:
    AnimationObjectMovingBezier()
    {
        AnimationObjectMoving::AnimationObjectMoving();
        bezierDrawIndex = 0;
        bezierPositionNumber = 1000;
        initialiseBezier();
        //calcBezierPoints();
        
        type = ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_BEZIER;
    }
    virtual void initialiseBezier()
    {
        setBezierPoints(animationBezierContainer, 200, 400, 210, 110, 390, 210, 400, 400);
    }
    virtual void setBezierPoint(AnimationBezierContainer & bezierContainer, int index, float x, float y)
    {
        bezierContainer.points[index].x = x;
        bezierContainer.points[index].y = y;
        calculateBezierPositions(bezierContainer);
    }
    void setBezierPoints(AnimationBezierContainer & bezierContainer, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
    {
        bezierContainer.points[0].x = x1;
        bezierContainer.points[0].y = y1;
        bezierContainer.points[1].x = x2;
        bezierContainer.points[1].y = y2;
        bezierContainer.points[2].x = x3;
        bezierContainer.points[2].y = y3;
        bezierContainer.points[3].x = x4;
        bezierContainer.points[3].y = y4;
        calculateBezierPositions(bezierContainer);
    }
    /*virtual void setBezierPoint1(float x, float y)
     {
     bezierPoint1.x = x;
     bezierPoint1.y = y;
     calcBezierPoints(bezierPoint1, bezierPoint2, bezierPoint3, bezierPoint4);
     }
     virtual void setBezierPoint2(float x, float y)
     {
     bezierPoint2.x = x;
     bezierPoint2.y = y;
     calcBezierPoints(bezierPoint1, bezierPoint2, bezierPoint3, bezierPoint4);
     }
     virtual void setBezierPoint3(float x, float y)
     {
     bezierPoint3.x = x;
     bezierPoint3.y = y;
     calcBezierPoints(bezierPoint1, bezierPoint2, bezierPoint3, bezierPoint4);
     }
     virtual void setBezierPoint4(float x, float y)
     {
     bezierPoint4.x = x;
     bezierPoint4.y = y;
     calcBezierPoints(bezierPoint1, bezierPoint2, bezierPoint3, bezierPoint4);
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
     }*/
    inline float calculateBezierPoint( float n1 , float n2 , float perc )
    {
        float diff = n2 - n1;
        
        return n1 + ( diff * perc );
    }
    virtual void calculateBezierPositions(AnimationBezierContainer & bezierContainer)
    {
        bezierContainer.position.clear();
        float numberCalc = (1.0/bezierPositionNumber);
        for( float i = 0 ; i < 1 ; i += numberCalc)
        {
            int xa = calculateBezierPoint( bezierContainer.points[0].x , bezierContainer.points[1].x , i );
            int ya = calculateBezierPoint( bezierContainer.points[0].y , bezierContainer.points[1].y , i );
            int xb = calculateBezierPoint( bezierContainer.points[1].x , bezierContainer.points[2].x , i );
            int yb = calculateBezierPoint( bezierContainer.points[1].y , bezierContainer.points[2].y , i );
            int xc = calculateBezierPoint( bezierContainer.points[2].x , bezierContainer.points[3].x , i );
            int yc = calculateBezierPoint( bezierContainer.points[2].y , bezierContainer.points[3].y , i );
            
            // The Blue Line
            int xm = calculateBezierPoint( xa , xb , i );
            int ym = calculateBezierPoint( ya , yb , i );
            int xn = calculateBezierPoint( xb , xc , i );
            int yn = calculateBezierPoint( yb , yc , i );
            
            
            // The Black Dot
            ofVec2f point;
            point.x = calculateBezierPoint( xm , xn , i );
            point.y = calculateBezierPoint( ym , yn , i );
            bezierContainer.position.push_back(point);
        }
    }
    
    virtual void setCalcBezierPointNumber(int n)
    {
        bezierPositionNumber = n;
        //calcBezierPoints();
    }
    /*virtual ofVec2f getCalcBezierPoint(int i)
     {
     return bezierPoints[i];
     }*/
    virtual void calcuateMovement()
    {
        float timef = ofGetElapsedTimef();
        if (bLoopMovementBackward)
        {
            if (bLoopMovementBackwardCycle)
            {
                bezierDrawIndex = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, bezierPositionNumber, 0, &ofxeasing::linear::easeIn);
                
                currentDrawContainer.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, animationMovingContainer.dimension.x, dimension.x, &ofxeasing::linear::easeIn);
                currentDrawContainer.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, animationMovingContainer.dimension.y, dimension.y, &ofxeasing::linear::easeIn);
                
                 currentDrawContainer.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf,animationMovingContainer.rotation, rotation, &ofxeasing::linear::easeIn);
                
            } else
            {
                bezierDrawIndex = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+durationTotal, 0, bezierPositionNumber, &ofxeasing::linear::easeIn);
                
                currentDrawContainer.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.x, animationMovingContainer.dimension.x, &ofxeasing::linear::easeIn);
                currentDrawContainer.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.y, animationMovingContainer.dimension.y, &ofxeasing::linear::easeIn);
                
                 currentDrawContainer.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, rotation, animationMovingContainer.rotation, &ofxeasing::linear::easeIn);
            }
        } else
        {
            bezierDrawIndex = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, 0, bezierPositionNumber, &ofxeasing::linear::easeIn);
            
            currentDrawContainer.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.x, animationMovingContainer.dimension.x, &ofxeasing::linear::easeIn);
            currentDrawContainer.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, dimension.y, animationMovingContainer.dimension.y, &ofxeasing::linear::easeIn);
            
             currentDrawContainer.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+timeLoopMovementDurationf, rotation, animationMovingContainer.rotation, &ofxeasing::linear::easeIn);
        }
    }
    virtual void draw()
    {
        //if (!bStart & !isFinished())
        //    return;
        
        
        ofPushMatrix();
        {
            ofTranslate(animationBezierContainer.position[bezierDrawIndex].x+(currentDrawContainer.dimension.x/2), animationBezierContainer.position[bezierDrawIndex].y+(currentDrawContainer.dimension.y/2), 0);
            ofRotate( currentDrawContainer.rotation, 0, 0, 1);
            ofPushMatrix();
            {
                ofTranslate(-(animationBezierContainer.position[bezierDrawIndex].x+(currentDrawContainer.dimension.x/2)), -(animationBezierContainer.position[bezierDrawIndex].y+(currentDrawContainer.dimension.y/2)), 0);
                images[imageDrawIndex].draw(animationBezierContainer.position[bezierDrawIndex].x, animationBezierContainer.position[bezierDrawIndex].y, currentDrawContainer.dimension.x, currentDrawContainer.dimension.y);
            }
        }
        ofPopMatrix();
    }
protected:
    int bezierDrawIndex;
    int bezierPositionNumber;
    
    /*vector<ofVec2f> bezierPoints;
     ofVec2f bezierPoint1;
     ofVec2f bezierPoint2;
     ofVec2f bezierPoint3;
     ofVec2f bezierPoint4;*/
    
    ofImage point;
    
private:
    AnimationBezierContainer animationBezierContainer;
    
};
class AnimationObjectMovingMulitple : public AnimationObjectMoving
{
    void addMovementPoint(float x, float y, float w, float h)
    {
        AnimationMovingContainer * movementContainer = new AnimationMovingContainer;
        movementContainer->position.x = x;
        movementContainer->position.y = y;
        movementContainer->dimension.x = w;
        movementContainer->dimension.y = h;
        
        animationMovementContainers.push_back(*movementContainer);
        delete movementContainer;
    }
    
    void alterMovementPoint(int index, float x, float y, float w, float h)
    {
        animationMovementContainers[index].position.x = x;
        animationMovementContainers[index].position.y = y;
        animationMovementContainers[index].dimension.x = w;
        animationMovementContainers[index].dimension.y = h;
    }
    
    void removeMovementPoint(int index)
    {
        animationMovementContainers.erase(animationMovementContainers.begin()+index);
    }
    
    virtual void update(unsigned long long time)
    {
        //if (!bStart & !isFinished())
        //    return;
        AnimationObjectBase::update(time);
        
        switch(AnimationBackend::getMode())
        {
            case ANIMATION_MODE_EDIT:
            {
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
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement();
                    timeLastMovementUpdate = time;
                }
                break;
            }
            case ANIMATION_MODE_VIEW:
            {
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
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement();
                    timeLastMovementUpdate = time;
                }
                break;
            }
            case ANIMATION_MODE_PLAY:
            {
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
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement();
                    timeLastMovementUpdate = time;
                }
                break;
            }
        }
    }

    
protected:
    int currentMovementContainerIndex;
    vector<AnimationMovingContainer> animationMovementContainers;
    
};
