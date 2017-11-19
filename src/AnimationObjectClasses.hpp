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
#include "AnimationShaders.hpp"

//TODO Add movement finsihed bool
//TODO add image animating finished bool
//TODO add bool if image dies on finish
//TODO Possibly change getBezierCurve and getMovementPoint to a generic function that returns the correct data type
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
        
        //bImageDurationSetManually = false;
        
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
        //imageInstance.drawIndex = 0;
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
        
        setPosition(x, y);
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
    virtual void setTotalDuration(unsigned long long duration)
    {
        durationTotal = duration;
    }
    virtual unsigned long long getTotalDuration()
    {
        return durationTotal;
    }
    /*virtual void setDurationPreImage(int imageIndex, float duration)
    {
        if (!bImagesLoaded || (imageIndex >= images.size()))
            return;
        
        unsigned long long durationl = (duration * 1000000);
        
        unsigned long long durationCount = 0;
        if (bImagesLoaded)
        {
            for (int i = 0; i < imageInstance.durations.size(); i++)
            {
                if (i == imageIndex)
                {
                    durationCount += duration;
                } else
                {
                    durationCount += imageInstance.durations[i];
                }
            }
            
            if (durationCount > durationTotal)
            {
                cout << "Can't change the frame duration, it exceed the total duration\n";
                return;
            } else
            {
                imageInstance.durations[imageIndex] = durationl;
                bImageDurationSetManually = true;
            }
            
        }
    }*/
    inline void setPosition(float x, float y)
    {
        origin.position.x = x;
        origin.position.y = y;
    }
    inline ofVec2f getPosition()
    {
        return origin.position;
    }
    inline void setDimension(float w, float h)
    {
        origin.dimension.x = w;
        origin.dimension.y = h;
    }
    virtual ofVec2f getDimension()
    {
        return origin.dimension;
    }
    inline void setRotation(float r)
    {
        origin.rotation = r;
    }
    inline void getRotation()
    {
        return origin.rotation;
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
    {        imageInstance.containerIndex = -1;
        imageInstance.drawIndex = 0;
        imageInstance.durations.clear();
        imageInstance.totalDuration = 0;
    }
    virtual void loadImagesFromDirectory(string fileDirectory)
    {
        releaseImages();
        //TEMP Clear image durations if new images are loaded
        
        imageInstance.containerIndex = AnimationBackend::loadImages(fileDirectory);
        int numImages = AnimationBackend::getNumberImages(imageInstance.containerIndex);
        unsigned long long durationPerImage = getTotalDuration() / numImages;
        for (int i = 0; i < numImages; i++)
        {
            imageInstance.durations.push_back(durationPerImage);
        }
        imageInstance.totalDuration = getTotalDuration();
        
    }
    /*virtual void setNextDrawImage(int i)
    {
        if (i > imageNumber)
            return;
        imageNumber = i;
    }*/
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
        //            /*if (bInUse && (imageInstance.drawIndex == imageNumber-1) & (ofGetElapsedTimeMicros() >= (timeStartPlay+durationImageMicros)))
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
        imageInstance.drawIndex = 0;
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
            if (imageInstance.drawIndex == 0)
            {
                return 0;
            } else if (imageInstance.drawIndex == 1)
            {
                return imageInstance.durations[0];
            }
            unsigned long long duration = 0;
            for (int i = 0; i <= imageInstance.drawIndex-1; i++)
            {
                duration += imageInstance.durations[i];
            }
            return duration;
        } else
        {
            int imageIndex = AnimationBackend::getNumberImages(imageInstance.containerIndex)-1;
            if (imageInstance.drawIndex == imageIndex)
            {
                return 0;
            } else if (imageInstance.drawIndex == imageIndex-1)
            {
                return imageInstance.durations[imageIndex-1];
            }
            unsigned long long duration = 0;
            for (int i = imageIndex; i > imageInstance.drawIndex; i--)
            {
                duration += imageInstance.durations[i];
            }
            return duration;
        }
        
    }
    virtual void calcuateLoopImageState(unsigned long long time)
    {
        int imageNumber = AnimationBackend::getNumberImages(imageInstance.containerIndex);
        switch(loopState)
        {
            case ANIMATION_LOOP_STATE_NONE:
            {
                if (time >= (timeStartLoopImageCycle+imageInstance.durations[imageInstance.drawIndex]+getPreviousDrawnImagesDuration(true)))
                {
                    if (++imageInstance.drawIndex >= imageNumber)
                    {
                        timeStartLoopImageCycle = time;
                        imageInstance.drawIndex = imageNumber-1;
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
                    if (time >= (timeStartLoopImageCycle+imageInstance.durations[imageInstance.drawIndex]+getPreviousDrawnImagesDuration(true)))
                    {
                        if (++imageInstance.drawIndex >= imageNumber)
                        {
                            timeStartLoopImageCycle = time;
                            imageInstance.drawIndex = imageNumber-1;
                            loopImageCount++;
                            if (bLoopImageBackward)
                            {
                                loopState = ANIMATION_LOOP_STATE_BACKWARD;
                            } else
                            {
                                imageInstance.drawIndex = 0;
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
                    if (time >= (timeStartLoopImageCycle+imageInstance.durations[imageInstance.drawIndex]+getPreviousDrawnImagesDuration(false)))
                    {
                        if (--imageInstance.drawIndex <= 0)
                        {
                            timeStartLoopImageCycle = time;
                            imageInstance.drawIndex = 0;
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
            ofTranslate(origin.position.x+(origin.dimension.x/2), origin.position.y+(origin.dimension.y/2), 0);
            ofRotate(origin.rotation, 0, 0, 1);
            ofPushMatrix();
            {
                ofTranslate(-(origin.position.x+(origin.dimension.x/2)), -(origin.position.y+(origin.dimension.y/2)), 0);
                AnimationBackend::getDrawImage(imageInstance.containerIndex, imageInstance.drawIndex).draw(origin.position.x, origin.position.y, origin.dimension.x, origin.dimension.y);
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
    
    AnimationType type;
    /*ofVec2f position;
    ofVec2f dimension;
    float rotation;*/
    AnimationOrigin origin;
    
    float durationTotal;
    float durationMovement;
    unsigned long long timeStart;
    //unsigned long durationImageMicros;
    //unsigned long durationEachImageMicros;
    unsigned long long timeStartPlay;
    unsigned long long timeStartLoopImageCycle;
    
    
    bool bImagesFinished;
    bool bImageMultiple;
    bool bImagesLoaded;
    AnimationImageInstance imageInstance;
    
    bool bLoopImage;
    bool bLoopImageBackward;
    AnimationLoopState loopState;
    int loopImageNumber;
    int loopImageCount;
    //vector<unsigned long> imageInstance.durations;
   

    
    
    //int imageNumber;
    
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
        
        //timeLoopMovementDuration = 0;
        timeLastMovementUpdate = 0;
        timeStartLoopMovementCyclef = 0.0;
        //animationMovementPoint.duration.f = 0.0;
        timeStartLoopMovementCycle = 0;
        timeLastMovementUpdate = 0;
        
        animationMovementPoint.bEndPoint = true;
        setMovementPointPosition(animationMovementPoint, 0, 0);
        setCurrentDrawPosition(0, 0);
        setMovementPointDimension(animationMovementPoint, 0, 0);
        setCurrentDrawDimension(0, 0);
        setMovementPointRotation(animationMovementPoint, 0.0);
        setCurrentDrawOrigin(0.0);
    }
    virtual void setup(string fileDirectory, AnimationType type, float duration, float x, float y, float w, float h, float r)
    {
        AnimationObjectBase::setup(fileDirectory, type, duration, x, y, w, h, r);
        setMovementPointDuration(getMovementPoint(), duration*1000000);
    }
    virtual void reset(AnimationType type, float x = 0.0, float y = 0.0, float w = 0.0, float h = 0.0, float r = 0.0, float xDraw = 0.0, float yDraw = 0.0, float wDraw = 0.0, float hDraw = 0.0, float rDraw = 0.0)
    {
        bLoopMovement = false;
        bLoopMovementBackward = false;
        bLoopMovementBackwardCycle = false;
        timeStartLoopMovementCycle = 0;
        setMovementPointPosition(animationMovementPoint, xDraw, yDraw);
        setCurrentDrawPosition(0, 0);
        setMovementPointDimension(animationMovementPoint, wDraw, hDraw);
        setCurrentDrawDimension(0, 0);
        setMovementPointRotation(animationMovementPoint, rDraw);
        setCurrentDrawOrigin(0.0);
        AnimationObjectBase::reset(type, x, y, w, h, r);
    }
    virtual AnimationMovementPoint & getMovementPoint()
    {
        return animationMovementPoint;
    }
    virtual void setMovementPointDuration(AnimationMovementPoint & movementPoint, unsigned long long duration)
    {
        movementPoint.duration.l = duration;
        movementPoint.duration.f = duration/1000000.0;
    }
    virtual void setMovementPointPosition(AnimationMovementPoint & movementPoint, float x, float y)
    {
        movementPoint.origin.position.x = x;
        movementPoint.origin.position.y = y;
    }
    virtual ofVec2f getMovementPointPosition(AnimationMovementPoint & movementPoint)
    {
        return movementPoint.origin.position;
    }
    virtual void setCurrentDrawPosition(float x, float y)
    {
        origin.position.x = x;
        origin.position.y = y;
    }
    virtual ofVec2f getCurrentDrawPosition()
    {
        return origin.position;
    }
    virtual void setMovementPointDimension(AnimationMovementPoint & movementPoint, float w, float h)
    {
        movementPoint.origin.dimension.x = w;
        movementPoint.origin.dimension.y = h;
    }
    virtual ofVec2f getMovementPointDimension(AnimationMovementPoint & movementPoint)
    {
        return movementPoint.origin.dimension;
    }
    virtual void setCurrentDrawDimension(float w, float h)
    {
        currentDrawOrigin.dimension.x = w;
        currentDrawOrigin.dimension.y = h;
    }
    virtual ofVec2f getCurrentDrawDimension()
    {
        return currentDrawOrigin.dimension;
    }
    virtual void setMovementPointRotation(AnimationMovementPoint & movementPoint, float r)
    {
        movementPoint.origin.rotation = r;
    }
    virtual float getEndDrawRotation(AnimationMovementPoint & movementPoint)
    {
        return movementPoint.origin.rotation;
    }
    virtual void setCurrentDrawOrigin(float r)
    {
        currentDrawOrigin.rotation = r;
    }
    virtual float getCurrentDrawOrigin()
    {
        return currentDrawOrigin.rotation;
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
    /*inline void setLoopMovementDuration(float d)
    {
        timeLoopMovementDuration = (unsigned long long)d*1000000;
        animationMovementPoint.duration.f = duration/1;
    }*/
    /*inline void getLoopMovementDuration()
    {
        return timeLoopMovementDuration;
    }*/
    virtual void start(unsigned long long time)
    {
        AnimationObjectBase::start(time);
        timeLastMovementUpdate = time;
        timeStartLoopMovementCycle = time;
        timeStartLoopMovementCyclef = ofGetElapsedTimef();
        setCurrentDrawPosition(origin.position.x, origin.position.y);
        setCurrentDrawDimension(origin.dimension.x, origin.dimension.y);
        setCurrentDrawOrigin(origin.rotation);
    }
    virtual void restart(unsigned long long time)
    {
        AnimationObjectBase::restart(time);
        timeStartLoopMovementCycle = 0;
        imageInstance.drawIndex = 0;
        timeStartPlay = ofGetElapsedTimeMicros();
    }
    virtual void getNextLoopMovementCycle(unsigned long long time)
    {
        if (bLoopMovement)
        {
            
            cout << "getNextLoopMovementCycle()\n";
        }
    }
    virtual void calcuateMovementForward(float timef)
    {
        
        currentDrawOrigin.position.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationMovementPoint.duration.f, origin.position.x, animationMovementPoint.origin.position.x, &ofxeasing::linear::easeIn);
        animationMovementPoint.origin.position.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationMovementPoint.duration.f, origin.position.y, animationMovementPoint.origin.position.y, &ofxeasing::linear::easeIn);
        
        currentDrawOrigin.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationMovementPoint.duration.f, origin.dimension.x, animationMovementPoint.origin.dimension.x, &ofxeasing::linear::easeIn);
        currentDrawOrigin.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationMovementPoint.duration.f, origin.dimension.y, animationMovementPoint.origin.dimension.y, &ofxeasing::linear::easeIn);
        
        currentDrawOrigin.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationMovementPoint.duration.f, origin.rotation, animationMovementPoint.origin.rotation, &ofxeasing::linear::easeIn);
    }
    virtual void calcuateMovement(float timef)
    {
        //Moved to draw because we only need call this on draw
        //float timeStartLoopMovementCyclef = float(timeStartLoopMovementCycle/1000000.0);
        //float animationMovementPoint.duration.f = (float(timeLoopMovementDuration)/1000000.0);
        if (bLoopMovementBackward)
        {
            if (bLoopMovementBackwardCycle)
            {
                currentDrawOrigin.position.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationMovementPoint.duration.f, animationMovementPoint.origin.position.x, origin.position.x, &ofxeasing::linear::easeIn);
                currentDrawOrigin.position.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationMovementPoint.duration.f, animationMovementPoint.origin.position.y, origin.position.y, &ofxeasing::linear::easeIn);
                
                currentDrawOrigin.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationMovementPoint.duration.f, animationMovementPoint.origin.dimension.x, origin.dimension.x, &ofxeasing::linear::easeIn);
                currentDrawOrigin.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationMovementPoint.duration.f, animationMovementPoint.origin.dimension.y, origin.dimension.y, &ofxeasing::linear::easeIn);
                
                currentDrawOrigin.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationMovementPoint.duration.f, animationMovementPoint.origin.rotation, origin.rotation, &ofxeasing::linear::easeIn);
            } else
            {
                calcuateMovementForward(timef);
                
            }
        } else
        {
            calcuateMovementForward(timef);
        }
    }
    virtual void checkNextLoopMovementCycle(unsigned long long time, float timef)
    {
        if (bLoopMovement)
        {
            if (time > (timeStartLoopMovementCycle+animationMovementPoint.duration.l))
            {
                timeStartLoopMovementCycle = time;
                timeStartLoopMovementCyclef = timef;
                if (bLoopMovementBackward)
                    bLoopMovementBackwardCycle = !bLoopMovementBackwardCycle;
            }
        }
    }
    virtual void update(unsigned long long time, float timef)
    {
        //if (!bStart & !isFinished())
        //    return;
        AnimationObjectBase::update(time);
        
        switch(AnimationBackend::getMode())
        {
            case ANIMATION_MODE_EDIT:
            {
                checkNextLoopMovementCycle(time, timef);
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement(timef);
                    timeLastMovementUpdate = time;
                }
                break;
            }
            case ANIMATION_MODE_VIEW:
            {
                checkNextLoopMovementCycle(time, timef);
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement(timef);
                    timeLastMovementUpdate = time;
                }
                break;
            }
            case ANIMATION_MODE_PLAY:
            {
                checkNextLoopMovementCycle(time, timef);
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement(timef);
                    timeLastMovementUpdate = time;
                }
                break;
            }
        }
    }
    virtual void drawOrigin()
    {
        ofPushMatrix();
        {
            ofTranslate(origin.position.x+(origin.dimension.x/2), origin.position.y+(origin.dimension.y/2), 0);
            ofRotate(origin.rotation, 0, 0, 1);
            ofPushMatrix();
            {
                ofTranslate(-(origin.position.x+(origin.dimension.x/2)), -(origin.position.y+(origin.dimension.y/2)), 0);
                AnimationBackend::getDrawImage(imageInstance.containerIndex, imageInstance.drawIndex).draw(origin.position.x, origin.position.y, origin.dimension.x, origin.dimension.y);
            }
        }
        ofPopMatrix();
    }
    virtual void drawPlaying()
    {
        ofPushMatrix();
        {
            ofTranslate(currentDrawOrigin.position.x+(currentDrawOrigin.dimension.x/2), currentDrawOrigin.position.y+(currentDrawOrigin.dimension.y/2), 0);
            ofRotate(currentDrawOrigin.rotation, 0, 0, 1);
            ofPushMatrix();
            {
                ofTranslate(-(currentDrawOrigin.position.x+(currentDrawOrigin.dimension.x/2)), -(currentDrawOrigin.position.y+(currentDrawOrigin.dimension.y/2)), 0);
                AnimationBackend::getDrawImage(imageInstance.containerIndex, imageInstance.drawIndex).draw(currentDrawOrigin.position.x, currentDrawOrigin.position.y, currentDrawOrigin.dimension.x, currentDrawOrigin.dimension.y);
            }
            ofPopMatrix();
        }
        ofPopMatrix();
    }
    virtual void drawFinsihed()
    {
        
    }
    virtual void draw()
    {
        //cout << this << " Call draw\n";
        switch(AnimationBackend::getMode())
        {
            case ANIMATION_MODE_EDIT:
            {
                
                return;
            }
            case ANIMATION_MODE_VIEW:
            {
                if (bStart)
                {
                    drawPlaying();
                } else
                {
                    drawOrigin();
                }
                return;
            }
            case ANIMATION_MODE_PLAY:
            {
                if (bStart)
                {
                    drawPlaying();
                } else
                {
                    drawOrigin();
                }
                return;
            }
        }
    }
    
protected:
    //ofVec2f currentDrawOrigin.position;
    //ofVec2f currentDrawOrigin.dimension;
    //float  currentDrawOrigin.rotation;
    AnimationOrigin currentDrawOrigin;
    bool bLoopMovement;
    bool bLoopMovementBackward;
    bool bLoopMovementBackwardCycle;
    
    int loopMovementNumber;
    int loopMovementCount;
    
    float timeStartLoopMovementCyclef;
    //float animationMovementPoint.duration.f;
    unsigned long long timeStartLoopMovementCycle;
    //unsigned long long timeLoopMovementDuration;
    unsigned long long timeLastMovementUpdate;
private:
    AnimationMovementPoint animationMovementPoint;
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
    virtual void setMovementPointDuration(AnimationBezierCurve & bezierCurve, unsigned long long duration)
    {
        bezierCurve.duration.l = duration;
        bezierCurve.duration.f = duration/1000000.0;
    }
    virtual void initialiseBezier()
    {
        setBezierPoints(animationBezierCurve, 200, 400, 210, 110, 390, 210, 400, 400);
    }
    virtual void setBezierCurve(AnimationBezierCurve & bezierCurve, int index, float x, float y)
    {
        bezierCurve.curvePoints[index].x = x;
        bezierCurve.curvePoints[index].y = y;
        calculateBezierPositions(bezierCurve);
    }
    virtual AnimationBezierCurve & getBezierCurve()
    {
        return animationBezierCurve;
    }
    virtual void setBezierPoints(AnimationBezierCurve & bezierCurve, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
    {
        bezierCurve.curvePoints[0].x = x1;
        bezierCurve.curvePoints[0].y = y1;
        bezierCurve.curvePoints[1].x = x2;
        bezierCurve.curvePoints[1].y = y2;
        bezierCurve.curvePoints[2].x = x3;
        bezierCurve.curvePoints[2].y = y3;
        bezierCurve.curvePoints[3].x = x4;
        bezierCurve.curvePoints[3].y = y4;
        calculateBezierPositions(bezierCurve);
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
    virtual void calculateBezierPositions(AnimationBezierCurve & bezierCurve)
    {
        bezierCurve.curvePositions.clear();
        float numberCalc = (1.0/bezierPositionNumber);
        for( float i = 0 ; i < 1 ; i += numberCalc)
        {
            int xa = calculateBezierPoint( bezierCurve.curvePoints[0].x , bezierCurve.curvePoints[1].x , i );
            int ya = calculateBezierPoint( bezierCurve.curvePoints[0].y , bezierCurve.curvePoints[1].y , i );
            int xb = calculateBezierPoint( bezierCurve.curvePoints[1].x , bezierCurve.curvePoints[2].x , i );
            int yb = calculateBezierPoint( bezierCurve.curvePoints[1].y , bezierCurve.curvePoints[2].y , i );
            int xc = calculateBezierPoint( bezierCurve.curvePoints[2].x , bezierCurve.curvePoints[3].x , i );
            int yc = calculateBezierPoint( bezierCurve.curvePoints[2].y , bezierCurve.curvePoints[3].y , i );
            
            // The Blue Line
            int xm = calculateBezierPoint( xa , xb , i );
            int ym = calculateBezierPoint( ya , yb , i );
            int xn = calculateBezierPoint( xb , xc , i );
            int yn = calculateBezierPoint( yb , yc , i );
            
            
            // The Black Dot
            ofVec2f point;
            point.x = calculateBezierPoint( xm , xn , i );
            point.y = calculateBezierPoint( ym , yn , i );
            bezierCurve.curvePositions.push_back(point);
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
    virtual void calcuateMovementForward(float timef)
    {
        bezierDrawIndex = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationBezierCurve.duration.f, 0, bezierPositionNumber, &ofxeasing::linear::easeIn);
        
        currentDrawOrigin.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationBezierCurve.duration.f, origin.dimension.x, animationBezierCurve.origin.dimension.x, &ofxeasing::linear::easeIn);
        currentDrawOrigin.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationBezierCurve.duration.f, origin.dimension.y, animationBezierCurve.origin.dimension.y, &ofxeasing::linear::easeIn);
        
        currentDrawOrigin.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationBezierCurve.duration.f, origin.rotation, animationBezierCurve.origin.rotation, &ofxeasing::linear::easeIn);
    }
    virtual void calcuateMovement(float timef)
    {
        if (bLoopMovementBackward)
        {
            if (bLoopMovementBackwardCycle)
            {
                bezierDrawIndex = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationBezierCurve.duration.f, bezierPositionNumber, 0, &ofxeasing::linear::easeIn);
                
                currentDrawOrigin.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationBezierCurve.duration.f, animationBezierCurve.origin.dimension.x, origin.dimension.x, &ofxeasing::linear::easeIn);
                currentDrawOrigin.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationBezierCurve.duration.f, animationBezierCurve.origin.dimension.y, origin.dimension.y, &ofxeasing::linear::easeIn);
                
                 currentDrawOrigin.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+animationBezierCurve.duration.f,animationBezierCurve.origin.rotation, origin.rotation, &ofxeasing::linear::easeIn);
                
            } else
            {
                calcuateMovementForward(timef);
            }
        } else
        {
            calcuateMovementForward(timef);
        }
    }
    virtual void checkNextLoopMovementCycle(unsigned long long time, float timef)
    {
        if (bLoopMovement)
        {
            if (time > (timeStartLoopMovementCycle+animationBezierCurve.duration.l))
            {
                timeStartLoopMovementCycle = time;
                timeStartLoopMovementCyclef = timef;
                if (bLoopMovementBackward)
                    bLoopMovementBackwardCycle = !bLoopMovementBackwardCycle;
            }
        }
    }
    /*virtual void update(unsigned long long time, float timef)
    {
        //if (!bStart & !isFinished())
        //    return;
        AnimationObjectBase::update(time);
        
        switch(AnimationBackend::getMode())
        {
            case ANIMATION_MODE_EDIT:
            {
                checkNextLoopMovementCycle(time, timef);
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement(timef);
                    timeLastMovementUpdate = time;
                }
                break;
            }
            case ANIMATION_MODE_VIEW:
            {
                checkNextLoopMovementCycle(time, timef);
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement(timef);
                    timeLastMovementUpdate = time;
                }
                break;
            }
            case ANIMATION_MODE_PLAY:
            {
                checkNextLoopMovementCycle(time, timef);
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement(timef);
                    timeLastMovementUpdate = time;
                }
                break;
            }
        }
    }*/
    virtual void draw()
    {
        //if (!bStart & !isFinished())
        //    return;
        
        
        ofPushMatrix();
        {
            ofTranslate(animationBezierCurve.curvePositions[bezierDrawIndex].x+(currentDrawOrigin.dimension.x/2), animationBezierCurve.curvePositions[bezierDrawIndex].y+(currentDrawOrigin.dimension.y/2), 0);
            ofRotate( currentDrawOrigin.rotation, 0, 0, 1);
            ofPushMatrix();
            {
                ofTranslate(-(animationBezierCurve.curvePositions[bezierDrawIndex].x+(currentDrawOrigin.dimension.x/2)), -(animationBezierCurve.curvePositions[bezierDrawIndex].y+(currentDrawOrigin.dimension.y/2)), 0);
                AnimationBackend::getDrawImage(imageInstance.containerIndex, imageInstance.drawIndex).draw(animationBezierCurve.curvePositions[bezierDrawIndex].x, animationBezierCurve.curvePositions[bezierDrawIndex].y, currentDrawOrigin.dimension.x, currentDrawOrigin.dimension.y);
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
    AnimationBezierCurve animationBezierCurve;
    
};


//
class AnimationObjectMovingMulitple : public AnimationObjectMoving
{
    
    virtual void setOriginPoint(float x, float y, float w, float h, float r)
    {
        
    }
    virtual void addMovementPoint(unsigned long long duration, float x, float y, float w, float h, float r)
    {
        AnimationMovementPoint * movementContainer = new AnimationMovementPoint;
        movementContainer->duration.l = duration;
        movementContainer->duration.f = duration/1000000.0;
        movementContainer->origin.position.x = x;
        movementContainer->origin.position.y = y;
        movementContainer->origin.dimension.x = w;
        movementContainer->origin.dimension.y = h;
        movementContainer->origin.rotation = r;
        movementContainer->index = animationMovementPoints.size();
        
        animationMovementPoints.push_back(*movementContainer);
        delete movementContainer;
    }
    virtual AnimationMovementPoint & getMovementPoint(int index)
    {
        if (index >= animationMovementPoints.size())
            return;
        return animationMovementPoints[index];
    }
    virtual AnimationMovementPoint & getCurrentMovementPoint()
    {
        return animationMovementPoints[currentMovementContainerIndex];
    }
    virtual AnimationOrigin & getPreviousMovementOrigin()
    {
        if (bLoopMovement)
        {
            if (bLoopMovementBackward)
            {
                if (bLoopMovementBackwardCycle)
                {
                    int index = getCurrentMovementPoint().index;
                    //if (index == animationMovementPoints.size()-1)
                    //{
                        //return static_cast<AnimationOrigin>(animationMovementPoints[index-1]);
                    //}
                    //return static_cast<AnimationOrigin>(animationMovementPoints[index+1]);
                    if (index == 0)
                    {
                        return origin;
                    } else /*if (index == animationMovementPoints.size()-1)*/
                    {
                        return getMovementPoint(index-1).origin;
                    }
                } else
                {
                    int index = getCurrentMovementPoint().index;
                    /*if (index == animationMovementPoints.size()-1)
                    {
                        
                    }*/
                    return getMovementPoint(index-1).origin;
                }
            } else
            {
                int index = getCurrentMovementPoint().index;
                return getMovementPoint(index-1).origin;
            }
        } else
        {
            int index = getCurrentMovementPoint().index;
            return getMovementPoint(index-1).origin;
        }
    }
    virtual unsigned long long getMovementPointsTotalDuration()
    {
        unsigned long long duration = 0;
        for (int i = 0; i < animationMovementPoints.size(); i++)
        {
            duration += animationMovementPoints[i].duration.l;
        }
        return duration;
    }
    virtual float getMovementPointsTotalDurationFloat()
    {
        float duration = 0;
        for (int i = 0; i < animationMovementPoints.size(); i++)
        {
            duration += animationMovementPoints[i].duration.f;
        }
        return duration;
    }
    /*virtual void alterMovementPoint(int index, float x, float y, float w, float h, float r)
    {
        if (animationMovementPoints.size() < index)
            return;
        
        animationMovementPoints[index].position.x = x;
        animationMovementPoints[index].position.y = y;
        animationMovementPoints[index].dimension.x = w;
        animationMovementPoints[index].dimension.y = h;
        animationMovementPoints[index].rotation = r;
    }
    virtual void alterMovementPointDuration(AnimationMovementPoint & movementContainer, unsigned long long duration)
    {
        movementContainer.duration = duration;
    }
    virtual void alterMovementPointPosition(AnimationMovementPoint & movementContainer, float x, float y)
    {
        movementContainer.position.x = x;
        movementContainer.position.y = y;
    }
    virtual void alterMovementPointDimension(AnimationMovementPoint & movementContainer, float w, float h)
    {
        movementContainer.dimension.x = w;
        movementContainer.dimension.y = h;
    }
    virtual void alterMovementPointRotation(AnimationMovementPoint & movementContainer, float r)
    {
        movementContainer.rotation = r;
    }
    virtual void swapMovementPoint(int index, int withIndex)
    {
        iter_swap(animationMovementPoints.begin() + index, animationMovementPoints.begin() + withIndex);
    }
    virtual void removeMovementPoint(int index)
    {
        animationMovementPoints.erase(animationMovementPoints.begin()+index);
    }*/
    virtual unsigned long long getPreviousMovementPointsDuration(bool bLoopForward)
    {
        int numMovementContainers = animationMovementPoints.size();
        if (bLoopForward)
        {
            if (currentMovementContainerIndex == 0)
            {
                return 0;
            } else if (currentMovementContainerIndex == 1)
            {
                return animationMovementPoints[0].duration.l;
            }
            unsigned long long duration = 0;
            for (int i = 0; i <= numMovementContainers-1; i++)
            {
                duration += animationMovementPoints[i].duration.l;
            }
            return duration;
        } else
        {
            int movementIndex = numMovementContainers;
            if (currentMovementContainerIndex == movementIndex)
            {
                return 0;
            } else if (currentMovementContainerIndex == movementIndex-1)
            {
                return animationMovementPoints[movementIndex-1].duration.l;
            }
            unsigned long long duration = 0;
            for (int i = movementIndex; i > numMovementContainers; i--)
            {
                duration += animationMovementPoints[i].duration.l;
            }
            return duration;
        }
    }
    
    virtual void getNextMovementPoint(unsigned long long time)
    {
        if (bLoopMovement)
        {
            if (bLoopMovementBackward)
            {
                if (time > timeStartLoopMovementCycle+getPreviousMovementPointsDuration(bLoopMovementBackwardCycle))
                {
                    if (bLoopMovementBackwardCycle)
                    {
                        if (--currentMovementContainerIndex < 0)
                        {
                            currentMovementContainerIndex = 0;
                            bLoopMovementBackwardCycle = !bLoopMovementBackwardCycle;
                        }
                        timeStartLoopMovementCycle = time;
                        timeStartLoopMovementCyclef = ofGetElapsedTimef();
                        
                    } else
                    {
                        int movementContainerSize = animationMovementPoints.size();
                        if (++currentMovementContainerIndex >= movementContainerSize)
                        {
                            currentMovementContainerIndex = movementContainerSize-1;
                            bLoopMovementBackwardCycle = !bLoopMovementBackwardCycle;
                        }
                        timeStartLoopMovementCycle = time;
                        timeStartLoopMovementCyclef = ofGetElapsedTimef();
                    }
                }
            }
            else
            {
                if (time > timeStartLoopMovementCycle+getPreviousMovementPointsDuration(true))
                {
                    int movementContainerSize = animationMovementPoints.size();
                    if (++currentMovementContainerIndex >= movementContainerSize)
                    {
                        currentMovementContainerIndex = movementContainerSize-1;
                    }
                    timeStartLoopMovementCycle = time;
                    timeStartLoopMovementCyclef = ofGetElapsedTimef();
                }
            }
        } else
        {
            if (time > timeStartLoopMovementCycle+getPreviousMovementPointsDuration(true))
            {
                int movementContainerSize = animationMovementPoints.size();
                if (++currentMovementContainerIndex >= movementContainerSize)
                {
                    currentMovementContainerIndex = movementContainerSize-1;
                }
                timeStartLoopMovementCycle = time;
                timeStartLoopMovementCyclef = ofGetElapsedTimef();
            }
        }
    }
    virtual void calcuateMovementForward(float timef)
    {
        currentDrawOrigin.position.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+getCurrentMovementPoint().duration.f, origin.position.x, getCurrentMovementPoint().origin.position.x, &ofxeasing::linear::easeIn);
        getCurrentMovementPoint().origin.position.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+getCurrentMovementPoint().duration.f, origin.position.y, getCurrentMovementPoint().origin.position.y, &ofxeasing::linear::easeIn);
        
        currentDrawOrigin.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+getCurrentMovementPoint().duration.f, origin.dimension.x, getCurrentMovementPoint().origin.dimension.x, &ofxeasing::linear::easeIn);
        currentDrawOrigin.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+getCurrentMovementPoint().duration.f, origin.dimension.y, getCurrentMovementPoint().origin.dimension.y, &ofxeasing::linear::easeIn);
        
        currentDrawOrigin.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+getCurrentMovementPoint().duration.f, origin.rotation, getCurrentMovementPoint().origin.rotation, &ofxeasing::linear::easeIn);
    }
    virtual void calcuateMovement(float timef)
    {
        //Moved to draw because we only need call this on draw
        //float timeStartLoopMovementCyclef = float(timeStartLoopMovementCycle/1000000.0);
        //float animationMovementPoint.duration.f = (float(timeLoopMovementDuration)/1000000.0);
        if (bLoopMovementBackward)
        {
            if (bLoopMovementBackwardCycle)
            {
                currentDrawOrigin.position.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+getCurrentMovementPoint().duration.f, getCurrentMovementPoint().origin.position.x, origin.position.x, &ofxeasing::linear::easeIn);
                currentDrawOrigin.position.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+getCurrentMovementPoint().duration.f, getCurrentMovementPoint().origin.position.y, origin.position.y, &ofxeasing::linear::easeIn);
                
                currentDrawOrigin.dimension.x = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+getCurrentMovementPoint().duration.f, getCurrentMovementPoint().origin.dimension.x, origin.dimension.x, &ofxeasing::linear::easeIn);
                currentDrawOrigin.dimension.y = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+getCurrentMovementPoint().duration.f, getCurrentMovementPoint().origin.dimension.y, origin.dimension.y, &ofxeasing::linear::easeIn);
                
                currentDrawOrigin.rotation = ofxeasing::map_clamp(timef, timeStartLoopMovementCyclef, timeStartLoopMovementCyclef+getCurrentMovementPoint().duration.f, getCurrentMovementPoint().origin.rotation, origin.rotation, &ofxeasing::linear::easeIn);
            } else
            {
                calcuateMovementForward(timef);
                
            }
        } else
        {
            calcuateMovementForward(timef);
        }
    }    virtual void update(unsigned long long time, float timef)
    {
        //if (!bStart & !isFinished())
        //    return;
        AnimationObjectBase::update(time);
        //AnimationObjectMoving::update(time);
        
        switch(AnimationBackend::getMode())
        {
            case ANIMATION_MODE_EDIT:
            {
                getNextMovementPoint(time);
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement(timef);
                    timeLastMovementUpdate = time;
                }
                break;
            }
            case ANIMATION_MODE_VIEW:
            {
                getNextMovementPoint(time);
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement(timef);
                    timeLastMovementUpdate = time;
                }
                break;
            }
            case ANIMATION_MODE_PLAY:
            {
                getNextMovementPoint(time);
                if (time > (timeLastMovementUpdate+ANIMATION_CALCUATION_RATE))
                {
                    calcuateMovement(timef);
                    timeLastMovementUpdate = time;
                }
                break;
            }
        }
    }

    
protected:
    int currentMovementContainerIndex;
    //animationMovementOrigin origin;
    vector<AnimationMovementPoint> animationMovementPoints;
    
};
