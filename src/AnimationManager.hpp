//
//  AnimationManager.h
//  animationTest
//
//  Created by Zachary Snow on 28/06/2017.
//
//

#pragma once

#include <algorithm>
#include "AnimationDefinitions.hpp"
#include "AnimationBackend.hpp"
#include "AnimationObjectClasses.hpp"
#include "ofThread.h"
#include "ofxGui.h"

class AnimationManager : public ofThread
{
public:
    AnimationManager()
    {
        bObjectSelected = false;
        
        AnimationObjectBaseIndexCount = 0;
        AnimationObjectMovingIndexCount = 0;
        AnimationObjectMovingBezierIndexCount = 0;
        
        ofImage * image = new ofImage;
        image->load(ofToDataPath("ui/point.png"));
        utilImages.push_back(*image);
        delete image;
        
        guiBuild(ANIMATION_MANAGER_GUI_WINDOW_MODE);
        
        //guiObjectLoopImageBackward.
        startThread(true, false);
    }
    
    void guiBuildLoopMovement(bool bMovement, bool bMovementBackward)
    {
        guiObject.add(guiObjectLoopMovement.set("Loop Movement", false));
        guiObject.add(guiObjectLoopMovementBackward.set("Loop Movement Backward", false));
        
        guiObjectLoopMovementNumber.addListener(this, &AnimationManager::guiOnSetLoopMovementNumber);
        guiObject.add(guiObjectLoopMovementNumber.set("Loop Movement Number", 0, 0, 120));
        
        guiObjectLoopMovementDuration.addListener(this, &AnimationManager::guiOnSetLoopMovementDuration);
        guiObject.add(guiObjectLoopMovementDuration.set("Loop Movement Duration", 5, 0, 60));
    }
    void guiBuild(AnimationManagerGUIWindow guiWindow)
    {
        if (guiIsWindowOpen(guiWindow))
        {
            return;
        }
        switch(guiWindow)
        {
            case ANIMATION_MANAGER_GUI_WINDOW_MODE:
            {
                guiMode.setup("Modes");
                guiMode.setPosition(ofGetViewportWidth()-210, 10);
                
                guiModeEdit.addListener(this, &AnimationManager::guiOnSetModeEdit);
                guiMode.add(guiModeEdit.set("Edit Mode", false));
                
                guiModeView.addListener(this, &AnimationManager::guiOnSetModeView);
                guiMode.add(guiModeView.set("View Mode", false));
                
                guiModePlay.addListener(this, &AnimationManager::guiOnSetModePlay);
                guiMode.add(guiModePlay.set("Play Mode", false));
                
                switch(AnimationBackend::getMode())
                {
                    case ANIMATION_MODE_EDIT:
                    {
                        guiModeEdit = true;
                        setModeEdit();
                        break;
                    }
                    case ANIMATION_MODE_VIEW:
                    {
                        guiModeView = true;
                        setModeView();
                        break;
                    }
                    case ANIMATION_MODE_PLAY:
                    {
                        guiModePlay = true;
                        setModePlay();
                        break;
                    }
                }
                break;
            }
            case ANIMATION_MANAGER_GUI_WINDOW_OBJECT:
            {
                bool bLoopImage = false;
                bool bLoopImageBackward = false;
                int getLoopImageNumber = 0;
                switch (selectedObject.animationClass)
                {
                    case ANIMATION_CLASS_BASE:
                    {
                        bLoopImage = animationObjectBaseV[selectedObject.index].getLoopImage();
                        bLoopImageBackward = animationObjectBaseV[selectedObject.index].getLoopImageBackward();
                        getLoopImageNumber = animationObjectBaseV[selectedObject.index].getLoopImageNumber();
                        break;
                    }
                    case ANIMATION_CLASS_MOVING:
                    {
                        bLoopImage = animationObjectMovingV[selectedObject.index].getLoopImage();
                        bLoopImageBackward = animationObjectMovingV[selectedObject.index].getLoopImageBackward();
                        getLoopImageNumber = animationObjectMovingV[selectedObject.index].getLoopImageNumber();
                        break;
                    }
                    case ANIMATION_CLASS_MOVING_BEZIER:
                    {
                        bLoopImage = animationObjectMovingBezierV[selectedObject.index].getLoopImage();
                        bLoopImageBackward = animationObjectMovingBezierV[selectedObject.index].getLoopImageBackward();
                        getLoopImageNumber = animationObjectMovingBezierV[selectedObject.index].getLoopImageNumber();
                        break;
                    }
                }
                guiObject.setup();
                guiObject.setPosition(ofGetViewportWidth()-210, 100);
                guiObjectLoopImage.addListener(this, &AnimationManager::guiOnLoopImage);
                guiObject.add(guiObjectLoopImage.set("Loop Image", bLoopImage));
                
                guiObjectLoopImageBackward.addListener(this, &AnimationManager::guiOnLoopImageBackward);
                guiObject.add(guiObjectLoopImageBackward.set("Loop Image Backward", bLoopImageBackward));
                
                guiObject.add(guiObjectLoopLabel.set("0 is Infitite"));
                
                guiObjectLoopImageNumber.addListener(this, &AnimationManager::guiOnSetLoopImageNumber);
                guiObject.add(guiObjectLoopImageNumber.set("Loop Image Number", getLoopImageNumber, 0, 120));
                
                guiObjectLoopImageDuration.addListener(this, &AnimationManager::guiOnSetLoopImageDuration);
                guiObject.add(guiObjectLoopImageDuration.set("Loop Image Duration", 5, 0, 60));
                
                bool bLoopMovement = false;
                bool bLoopMovementBackward = false;
                switch (selectedObject.animationClass)
                {
                    case ANIMATION_CLASS_BASE:
                    {
                        break;
                    }
                    case ANIMATION_CLASS_MOVING:
                    {
                        bLoopMovement = animationObjectMovingV[selectedObject.index].getLoopMovement();
                        bLoopMovementBackward = animationObjectMovingV[selectedObject.index].getLoopMovementBackward();
                        guiBuildLoopMovement(bLoopMovement, bLoopMovementBackward);
                        break;
                    }
                    case ANIMATION_CLASS_MOVING_BEZIER:
                    {
                        bLoopMovement = animationObjectMovingBezierV[selectedObject.index].getLoopMovement();
                        bLoopMovementBackward = animationObjectMovingBezierV[selectedObject.index].getLoopMovementBackward();
                        guiBuildLoopMovement(bLoopMovement, bLoopMovementBackward);
                        break;
                        
                        //DO BEZIER types;
                    }
                }
                break;
            }
            case ANIMATION_MANAGER_GUI_WINDOW_PLAY:
            {
                guiPlay.setup("Play Options");
                
                guiPlayButtonPlay.addListener(this, &AnimationManager::guiOnSetPlay);
                guiPlay.add(guiPlayButtonPlay.set("Play", false));
                
                guiPlayButtonPause.addListener(this, &AnimationManager::guiOnSetPause);
                guiPlay.add(guiPlayButtonPause.set("Pause", false));
                
                guiPlayButtonStop.addListener(this, &AnimationManager::guiOnSetStop);
                guiPlay.add(guiPlayButtonStop.set("Stop", false));
                
                break;
            }
        }
        guiBuiltWindows.push_back(guiWindow);
    }
    bool guiIsWindowOpen(AnimationManagerGUIWindow guiWindow)
    {
        for (int i = 0; i < guiBuiltWindows.size(); i++)
        {
            if (guiBuiltWindows[i] == guiWindow)
            {
                return true;
            }
        }
        return false;
    }
    void guiRemoveWindow(AnimationManagerGUIWindow guiWindow)
    {
        for (int i = 0; i < guiBuiltWindows.size(); i++)
        {
            if (guiBuiltWindows[i] == guiWindow)
            {
                switch(guiBuiltWindows[i])
                {
                    case ANIMATION_MANAGER_GUI_WINDOW_MODE:
                    {
                        guiMode.clear();
                        break;
                    }
                    case ANIMATION_MANAGER_GUI_WINDOW_OBJECT:
                    {
                        guiObject.clear();
                        break;
                    }
                    case ANIMATION_MANAGER_GUI_WINDOW_OBJECT_IMAGE_DURATION:
                    {
                        //TODO
                        break;
                    }
                    case ANIMATION_MANAGER_GUI_WINDOW_PLAY:
                    {
                        guiPlay.clear();
                        break;
                    }
                }
                guiBuiltWindows.erase(guiBuiltWindows.begin()+i);
            }
        }
    }
    AnimationManagerIndex createAnimationImage(string fileDirectory, AnimationType type, float duration, float x, float y, float w, float h, float r, float drawIndex)
    {
        switch (type)
        {
            case ANIMATION_TYPE_IMAGE_SINGLE_STILL:
            {
                AnimationObjectBase * animationObjectBase = new AnimationObjectBase;
                animationObjectBase->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationObjectBaseV.push_back(*animationObjectBase);
                delete animationObjectBase;
                
                AnimationManagerIndex animationManagerIndex;

                animationManagerIndex.index = AnimationObjectBaseIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_BASE;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                AnimationObjectBaseIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_SINGLE_MOVING:
            {
                AnimationObjectMoving * animationObjectMoving = new AnimationObjectMoving;
                animationObjectMoving->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationObjectMovingV.push_back(*animationObjectMoving);
                delete animationObjectMoving;
                
                AnimationManagerIndex animationManagerIndex;
                
                animationManagerIndex.index = AnimationObjectMovingIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_MOVING;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                AnimationObjectMovingIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER:
            {
                AnimationObjectMovingBezier * animationObjectMovingBezier = new AnimationObjectMovingBezier;
                animationObjectMovingBezier->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationObjectMovingBezierV.push_back(*animationObjectMovingBezier);
                delete animationObjectMovingBezier;
                
                AnimationManagerIndex animationManagerIndex;
                
                animationManagerIndex.index = AnimationObjectMovingIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_MOVING_BEZIER;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                AnimationObjectMovingBezierIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_MULTIPLE_STILL:
            {
                AnimationObjectBase * animationObjectBase = new AnimationObjectBase;
                animationObjectBase->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationObjectBaseV.push_back(*animationObjectBase);
                delete animationObjectBase;
                
                AnimationManagerIndex animationManagerIndex;
                
                animationManagerIndex.index = AnimationObjectBaseIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_BASE;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                AnimationObjectBaseIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING:
            {
                AnimationObjectMoving * animationObjectMoving = new AnimationObjectMoving;
                animationObjectMoving->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationObjectMovingV.push_back(*animationObjectMoving);
                delete animationObjectMoving;
                
                AnimationManagerIndex animationManagerIndex;
                animationManagerIndex.index = AnimationObjectMovingIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_MOVING;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                AnimationObjectMovingIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_BEZIER:
            {
                AnimationObjectMovingBezier * animationObjectMovingBezier = new AnimationObjectMovingBezier;
                animationObjectMovingBezier->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationObjectMovingBezierV.push_back(*animationObjectMovingBezier);
                delete animationObjectMovingBezier;
                
                AnimationManagerIndex animationManagerIndex;
                animationManagerIndex.index = AnimationObjectMovingBezierIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_MOVING_BEZIER;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                AnimationObjectMovingBezierIndexCount++;
                break;
            }
        }
        sort(animationIndices.begin(), animationIndices.end(), compareDrawIndex);
    }
    template<class T>
    T * getAnimationObject(AnimationManagerIndex animationManagerIndex)
    {
        switch (animationManagerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                return animationObjectBaseV[animationManagerIndex.index];
            }
            case ANIMATION_CLASS_MOVING:
            {
                return animationObjectMovingV[animationManagerIndex.index];
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                return animationObjectMovingBezierV[animationManagerIndex.index];
            }
        }
    }
    template<class T>
    bool getLoopImageTemplate(T test)
    {
        return test.getLoopImage();
    }
    void setLoopImage(AnimationManagerIndex & animationManagerIndex, bool b)
    {
        switch (animationManagerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                animationObjectBaseV[animationManagerIndex.index].setLoopImage(b);
                break;
            }
            case ANIMATION_CLASS_MOVING:
            {
                animationObjectMovingV[animationManagerIndex.index].setLoopImage(b);
                break;
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                animationObjectMovingBezierV[animationManagerIndex.index].setLoopImage(b);
                break;
            }
        }
    }
    bool getLoopImage(AnimationManagerIndex & animationManagerIndex)
    {
        switch (animationManagerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                return animationObjectBaseV[animationManagerIndex.index].getLoopImage();
            }
            case ANIMATION_CLASS_MOVING:
            {
                return animationObjectMovingV[animationManagerIndex.index].getLoopImage();
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                return animationObjectMovingBezierV[animationManagerIndex.index].getLoopImage();
            }
        }
    }
    void setLoopImageBackward(AnimationManagerIndex & animationManagerIndex, bool b)
    {
        switch (animationManagerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                animationObjectBaseV[animationManagerIndex.index].setLoopImageBackward(b);
                break;
            }
            case ANIMATION_CLASS_MOVING:
            {
                animationObjectMovingV[animationManagerIndex.index].setLoopImageBackward(b);
                break;
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                animationObjectMovingBezierV[animationManagerIndex.index].setLoopImageBackward(b);
                break;
            }
        }
    }
    bool getLoopMovement(AnimationManagerIndex & animationManagerIndex)
    {
        switch (animationManagerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                return false;
            }
            case ANIMATION_CLASS_MOVING:
            {
                return animationObjectMovingV[animationManagerIndex.index].getLoopMovement();
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                return animationObjectMovingBezierV[animationManagerIndex.index].getLoopMovement();
            }
        }
    }
    void setLoopMovement(AnimationManagerIndex & animationManagerIndex, bool b)
    {
        switch (animationManagerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                return;
            }
            case ANIMATION_CLASS_MOVING:
            {
                animationObjectMovingV[animationManagerIndex.index].setLoopMovement(b);
                break;
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                animationObjectMovingBezierV[animationManagerIndex.index].setLoopMovement(b);
                break;
            }
        }
    }
    void setLoopMovementBackward(AnimationManagerIndex & animationManagerIndex, bool b)
    {
        switch (animationManagerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                return;
            }
            case ANIMATION_CLASS_MOVING:
            {
                animationObjectMovingV[animationManagerIndex.index].setLoopMovementBackward(b);
                break;
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                animationObjectMovingBezierV[animationManagerIndex.index].setLoopMovementBackward(b);
                break;
            }
        }
    }
    bool getLoopMovementBackward(AnimationManagerIndex & animationManagerIndex)
    {
        switch(animationManagerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                return false;
            }
            case ANIMATION_CLASS_MOVING:
            {
                return animationObjectMovingV[animationManagerIndex.index].getLoopMovementBackward();
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                return animationObjectMovingBezierV[animationManagerIndex.index].getLoopMovementBackward();
            }
        }
    }
    bool isObjectBeingPressed(int i, int x, int y, ofVec2f & p, ofVec2f & d)
    {
        if (((x > p.x) && (y > p.y) && ((x < p.x+d.x) && (y < p.y+d.y))))
        {
            offsetDragObject.x = p.x-x;
            offsetDragObject.y = p.y-y;
            selectedObject = animationIndices[i];;
            bObjectSelected = true;
            
            if (guiIsWindowOpen(ANIMATION_MANAGER_GUI_WINDOW_OBJECT))
            {
                guiRemoveWindow(ANIMATION_MANAGER_GUI_WINDOW_OBJECT);
            }
            guiBuild(ANIMATION_MANAGER_GUI_WINDOW_OBJECT);
            
            return true;
        }
        return false;

    }
    void mousePressed(int x, int y, int button)
    {
        switch(button)
        {
            case 0:
            {
                bool bNewObjectSelected = false;
                switch(AnimationBackend::getMode())
                {
                    case ANIMATION_MODE_EDIT:
                    {
                        for (int i = 0; i < animationIndices.size(); i++)
                        {
                            switch(animationIndices[i].animationClass)
                            {
                                case ANIMATION_CLASS_BASE:
                                {
                                    ofVec2f p = animationObjectBaseV[animationIndices[i].index].getPosition();
                                    ofVec2f d = animationObjectBaseV[animationIndices[i].index].getDimension();
                                    
                                    if (bNewObjectSelected == false)
                                    {
                                        bNewObjectSelected = isObjectBeingPressed(i, x, y, p, d);
                                    }
                                    break;
                                }
                                case ANIMATION_CLASS_MOVING:
                                {
                                    ofVec2f p = animationObjectMovingV[animationIndices[i].index].getPosition();
                                    ofVec2f d = animationObjectMovingV[animationIndices[i].index].getDimension();
                                    
                                    if (bNewObjectSelected == false)
                                    {
                                        bNewObjectSelected = isObjectBeingPressed(i, x, y, p, d);
                                    }
                                    break;
                                }
                                case ANIMATION_CLASS_MOVING_BEZIER:
                                {
                                    ofVec2f p = animationObjectMovingBezierV[animationIndices[i].index].getPosition();
                                    ofVec2f d = animationObjectMovingBezierV[animationIndices[i].index].getDimension();
                                    
                                    if (bNewObjectSelected == false)
                                    {
                                        bNewObjectSelected = isObjectBeingPressed(i, x, y, p, d);
                                    }
                                    break;
                                }
                            }
                            
                        }
                        break;
                    }
                    case ANIMATION_MODE_VIEW:
                    {
                        
                        break;
                    }
                    case ANIMATION_MODE_PLAY:
                    {
                        break;
                    }
                }
                if (!bNewObjectSelected)
                {
                    bObjectSelected = false;
                }
                break;
            }
            case 1:
            {
                break;
            }
            case 2:
            {
                break;
            }
        }
        
    }
    void mouseDragged(int x, int y, int button)
    {
        if (AnimationBackend::getMode() == ANIMATION_MODE_EDIT)
        {
            if (bObjectSelected)
            {
                switch (selectedObject.animationClass)
                {
                    case ANIMATION_CLASS_BASE:
                    {
                        break;
                    }
                    case ANIMATION_CLASS_MOVING:
                    {
                        //p.x =
                        animationObjectMovingV[selectedObject.index].setPosition(offsetDragObject.x+x, offsetDragObject.y+y);
                        break;
                    }
                    case ANIMATION_CLASS_MOVING_BEZIER:
                    {
                        break;
                    }
                }
            }
        }
    }
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            unsigned long long time = ofGetElapsedTimeMicros();
            for (int i = 0; i < animationObjectBaseV.size(); i++)
            {
                animationObjectBaseV[i].update(time);
            }
            for (int i = 0; i < animationObjectMovingV.size(); i++)
            {
                animationObjectMovingV[i].update(time);
            }
            for (int i = 0; i < animationObjectMovingBezierV.size(); i++)
            {
                animationObjectMovingBezierV[i].update(time);
            }
        }
    }
    void update(unsigned long long time)
    {
        if ((AnimationBackend::getMode() == ANIMATION_MODE_PLAY) && (AnimationBackend::getPlayState() == ANIMATION_PLAY_STATE_PLAY))
        {
            unsigned long long playTime = AnimationBackend::calcCurrentPlayTime();
            
            //for (int i = 0; i < animationObjectBaseV.size(); i++)
            //{
            //    animationObjectBaseV[i].update(time);
            //}
            
        }
    }
    void drawAssetBoxes(ofVec2f p, ofVec2f d)
    {
        ofSetColor(0, 0, 0);
        ofDrawLine(p.x,             p.y, d.x + p.x,      p.y);
        ofDrawLine(d.x + p.x,       p.y, d.x + p.x, d.y + p.y);
        ofDrawLine(d.x + p.x, d.y + p.y, p.x      , d.y + p.y);
        ofDrawLine(p.x,       d.y + p.y, p.x      , p.y);
        ofSetColor(255, 255, 255);
        
        utilImages[static_cast<int>(ANIMATION_MANAGER_UTIL_IMAGE_POINT)].draw(p.x-5, p.y-5);
        utilImages[static_cast<int>(ANIMATION_MANAGER_UTIL_IMAGE_POINT)].draw(p.x+d.x-5, p.y-5);
        utilImages[static_cast<int>(ANIMATION_MANAGER_UTIL_IMAGE_POINT)].draw(p.x+d.y-5, p.y+d.y-5);
        utilImages[static_cast<int>(ANIMATION_MANAGER_UTIL_IMAGE_POINT)].draw(p.x-5, p.y+d.y-5);
    }
    void draw()
    {
        for (int i = 0; i < animationIndices.size(); i++)
        {
            switch(animationIndices[i].animationClass)
            {
                case ANIMATION_CLASS_BASE:
                {
                    animationObjectBaseV[animationIndices[i].index].draw();
                    switch(AnimationBackend::getMode())
                    {
                        case ANIMATION_MODE_EDIT:
                        case ANIMATION_MODE_VIEW:
                        {
                            drawAssetBoxes(animationObjectBaseV[animationIndices[i].index].getPosition(), animationObjectBaseV[animationIndices[i].index].getDimension());
                        }
                        case ANIMATION_MODE_PLAY:
                        {
                            
                        }
                    }
                    break;
                }
                case ANIMATION_CLASS_MOVING:
                {
                    animationObjectMovingV[animationIndices[i].index].draw();
                    switch(AnimationBackend::getMode())
                    {
                        case ANIMATION_MODE_EDIT:
                        case ANIMATION_MODE_VIEW:
                        {
                            drawAssetBoxes(animationObjectMovingV[animationIndices[i].index].getPosition(), animationObjectMovingV[animationIndices[i].index].getDimension());
                        }
                        case ANIMATION_MODE_PLAY:
                        {
                            
                        }
                    }
                    break;
                }
                case ANIMATION_CLASS_MOVING_BEZIER:
                {
                    animationObjectMovingBezierV[animationIndices[i].index].draw();
                    switch(AnimationBackend::getMode())
                    {
                        case ANIMATION_MODE_EDIT:
                        case ANIMATION_MODE_VIEW:
                        {
                            drawAssetBoxes(animationObjectMovingBezierV[animationIndices[i].index].getPosition(), animationObjectMovingBezierV[animationIndices[i].index].getDimension());
                        }
                        case ANIMATION_MODE_PLAY:
                        {
                            
                        }
                    }
                    break;
                }
            }
        }
        for (int i = 0; i < guiBuiltWindows.size(); i++)
        {
            switch(guiBuiltWindows[i])
            {
                case ANIMATION_MANAGER_GUI_WINDOW_MODE:
                {
                    guiMode.draw();
                    break;
                }
                case ANIMATION_MANAGER_GUI_WINDOW_OBJECT:
                {
                    guiObject.draw();
                    break;
                }
                case ANIMATION_MANAGER_GUI_WINDOW_PLAY:
                {
                    guiPlay.draw();
                    break;
                }
            }
        }
        //guiObject.draw();
    }
    void setModeEdit()
    {
        AnimationBackend::setMode(ANIMATION_MODE_EDIT);
    }
    void setModeView()
    {
        AnimationBackend::setMode(ANIMATION_MODE_VIEW);
    }
    void setModePlay()
    {
        AnimationBackend::setMode(ANIMATION_MODE_PLAY);
        guiBuild(ANIMATION_MANAGER_GUI_WINDOW_PLAY);
    }
    void guiOnLoopImage(bool & b)
    {
        if (bObjectSelected)
        {
            setLoopImage(selectedObject, b);
        }
    }
    
    void guiOnLoopImageBackward(bool & b)
    {
        if (bObjectSelected)
        {
            if (b)
            {
                setLoopImage(selectedObject, b);
                guiObjectLoopImage = true;
            }
            
            setLoopImageBackward(selectedObject, b);
        }
    }
    
    void guiOnSetLoopImageNumber(int & i)
    {
        
        
    }
    
    void guiOnSetLoopImageDuration(float & f)
    {
        
    }
    
    void guiOnLoopMovement(bool & b)
    {
        if (bObjectSelected)
        {
            setLoopImage(selectedObject, b);
        }
    }
    
    void guiOnLoopMovementBackward(bool & b)
    {
        if (bObjectSelected)
        {
            if (b)
            {
                if (!getLoopMovementBackward(selectedObject))
                {
                    setLoopMovement(selectedObject, b);
                }
                
                setLoopMovementBackward(selectedObject, b);
                guiObjectLoopMovement = true;
            }
        }
    }
    
    void guiOnSetLoopMovementNumber(int & i)
    {
        
        
    }
    
    void guiOnSetLoopMovementDuration(float & f)
    {
        
    }
    
    void guiOnSetModeEdit(bool & b)
    {
        if (b)
        {
            guiModeEdit = b;
            guiModeView = false;
            guiModePlay = false;
            if (guiIsWindowOpen(ANIMATION_MANAGER_GUI_WINDOW_PLAY))
            {
                guiRemoveWindow(ANIMATION_MANAGER_GUI_WINDOW_PLAY);
            }
            setModeEdit();
        } else if ((guiModePlay == false) && (guiModeView == false))
        {
            guiModeEdit = true;
        }
    }
    
    void guiOnSetModeView(bool & b)
    {
        if (b)
        {
            guiModeEdit = false;
            guiModeView = b;
            guiModePlay = false;
            if (guiIsWindowOpen(ANIMATION_MANAGER_GUI_WINDOW_PLAY))
            {
                guiRemoveWindow(ANIMATION_MANAGER_GUI_WINDOW_PLAY);
            }
            setModeView();
        } else if ((guiModeEdit == false) && (guiModePlay == false))
        {
            guiModeView = true;
        }
    }
    
    void guiOnSetModePlay(bool & b)
    {
        if (b)
        {
            guiModeEdit = false;
            guiModeView = false;
            guiModePlay = b;
            setModePlay();
        } else if ((guiModeEdit == false) && (guiModeView == false))
        {
            guiModePlay = true;
        }
    }
    
    void guiOnSetPlay(bool & b)
    {
        if (b)
        {
            guiPlayButtonPlay = b;
            guiPlayButtonPause = false;
            guiPlayButtonStop = false;
        } else if ((guiPlayButtonPause == false) && (guiPlayButtonStop == false))
        {
            guiPlayButtonPlay = true;
        }
        
    }
    
    void guiOnSetPause(bool & b)
    {
        if (b)
        {
            guiPlayButtonPlay = false;
            guiPlayButtonPause = b;
            guiPlayButtonStop = false;
        } else if ((guiPlayButtonPlay == false) && (guiPlayButtonStop == false))
        {
            guiPlayButtonStop = true;
        }
    }
    
    void guiOnSetStop(bool & b)
    {
        if (b)
        {
            guiPlayButtonPlay = false;
            guiPlayButtonPause = false;
            guiPlayButtonStop = b;
        } else if ((guiPlayButtonPlay == false) && (guiPlayButtonPause == false))
        {
            guiPlayButtonStop = true;
        }
    }
private:
    int AnimationObjectBaseIndexCount;
    vector<AnimationObjectBase> animationObjectBaseV;
    
    int AnimationObjectMovingIndexCount;
    vector<AnimationObjectMoving> animationObjectMovingV;
    
    int AnimationObjectMovingBezierIndexCount;
    vector<AnimationObjectMovingBezier> animationObjectMovingBezierV;
    
    vector<AnimationManagerIndex> animationIndices;
    
    bool bObjectSelected;
    AnimationManagerIndex selectedObject;
    ofVec2f offsetDragObject;
    
    vector<ofImage> utilImages;
    
    static bool compareDrawIndex(AnimationManagerIndex & firstIndex, AnimationManagerIndex & secondIndex)
    {
        return firstIndex.drawIndex < secondIndex.drawIndex;
    }
    
    vector<AnimationManagerGUIWindow> guiBuiltWindows;
    
    ofxPanel guiMode;
    ofParameter<bool> guiModeEdit;
    ofParameter<bool> guiModeView;
    ofParameter<bool> guiModePlay;
    
    
    
    ofxPanel guiObject;
    
    ofxButton guiChangeNameButton;
    ofParameter<string> guiUniqueID;
    ofxButton guiChangeAnimationFolder;
    
    
    ofParameter<bool> guiObjectLoopImage;
    ofParameter<bool> guiObjectLoopImageBackward;
    
    ofParameter<string> guiObjectLoopLabel;
    ofParameter<int> guiObjectLoopImageNumber;
    ofParameter<float> guiObjectLoopImageDuration;
    
    
    ofParameter<bool> guiObjectLoopMovement;
    ofParameter<bool> guiObjectLoopMovementBackward;
    
    ofParameter<string> guiObjectLabelLoopMovement;
    ofParameter<int> guiObjectLoopMovementNumber;
    ofParameter<float> guiObjectLoopMovementDuration;
    
    
    
    ofxPanel guiPlay;
    
    //ofParameter<bool> guiButtonRewind;
    ofParameter<bool> guiPlayButtonPlay;
    //ofParameter<bool> guiButtonFastForward;
    ofParameter<bool> guiPlayButtonPause;
    ofParameter<bool> guiPlayButtonStop;
    
};
