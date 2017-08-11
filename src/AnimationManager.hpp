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
#include "AnimationClass.hpp"
#include "ofxGui.h"

class AnimationManager
{
public:
    AnimationManager()
    {
        bObjectSelected = false;
        
        animationBaseIndexCount = 0;
        animationMovingIndexCount = 0;
        animationMovingBezierIndexCount = 0;
        
        ofImage * image = new ofImage;
        image->load(ofToDataPath("ui/point.png"));
        utilImages.push_back(*image);
        delete image;
        
        guiBuild(ANIMATION_MANAGER_GUI_WINDOW_MODE);
        
        //guiObjectLoopImageBackward.
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
                guiMode.clear();
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
                guiObject.clear();
                guiObject.setup();
                 //guiObject.setPosition(ofGetViewportWidth()-250, 0);
                guiObjectLoopImage.addListener(this, &AnimationManager::guiOnLoopImage);
                guiObject.add(guiObjectLoopImage.set("Loop Image", false));
                
                guiObjectLoopImageBackward.addListener(this, &AnimationManager::guiOnLoopImageBackward);
                guiObject.add(guiObjectLoopImageBackward.set("Loop Image Backward", false));
                
                guiObject.add(guiObjectLoopLabel.set("0 is Infitite"));
                
                guiObjectLoopImageNumber.addListener(this, &AnimationManager::guiOnSetLoopImageNumber);
                guiObject.add(guiObjectLoopImageNumber.set("Loop Image Number", 0, 0, 120));
                
                guiObjectLoopImageDuration.addListener(this, &AnimationManager::guiOnSetLoopImageDuration);
                guiObject.add(guiObjectLoopImageDuration.set("Loop Image Duration", 5, 0, 60));
                switch (selectedObject.animationClass)
                {
                    case ANIMATION_CLASS_BASE:
                    {
                        break;
                    }
                    case ANIMATION_CLASS_MOVING:
                    {
                        guiObject.add(guiObjectLoopMovement.set("Loop Movement", false));
                        guiObject.add(guiObjectLoopMovementBackward.set("Loop Movement Backward", false));
                        
                        guiObject.add(guiObjectLoopLabel.set("0 is Infitite"));
                        
                        guiObjectLoopMovementNumber.addListener(this, &AnimationManager::guiOnSetLoopMovementNumber);
                        guiObject.add(guiObjectLoopMovementNumber.set("Loop Movement Number", 0, 0, 120));
                        
                        guiObjectLoopMovementDuration.addListener(this, &AnimationManager::guiOnSetLoopMovementDuration);
                        guiObject.add(guiObjectLoopMovementDuration.set("Loop Movement Duration", 5, 0, 60));
                        break;
                    }
                    case ANIMATION_CLASS_MOVING_BEZIER:
                    {
                        guiObject.add(guiObjectLoopMovement.set("Loop Movement", false));
                        guiObject.add(guiObjectLoopMovementBackward.set("Loop Movement Backward", false));
                        
                        guiObject.add(guiObjectLoopLabel.set("0 is Infitite"));
                        
                        guiObjectLoopMovementNumber.addListener(this, &AnimationManager::guiOnSetLoopMovementNumber);
                        guiObject.add(guiObjectLoopMovementNumber.set("Loop Movement Number", 0, 0, 120));
                        
                        guiObjectLoopMovementDuration.addListener(this, &AnimationManager::guiOnSetLoopMovementDuration);
                        guiObject.add(guiObjectLoopMovementDuration.set("Loop Movement Duration", 5, 0, 60));
                        break;
                        
                        //DO BEZIER types;
                    }
                }
                break;
            }
            case ANIMATION_MANAGER_GUI_WINDOW_PLAY:
            {
                guiPlay.clear();
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
                AnimationBase * animationBase = new AnimationBase;
                animationBase->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationBaseV.push_back(*animationBase);
                delete animationBase;
                
                AnimationManagerIndex animationManagerIndex;

                animationManagerIndex.index = animationBaseIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_BASE;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                animationBaseIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_SINGLE_MOVING:
            {
                AnimationMoving * animationMoving = new AnimationMoving;
                animationMoving->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationMovingV.push_back(*animationMoving);
                delete animationMoving;
                
                AnimationManagerIndex animationManagerIndex;
                
                animationManagerIndex.index = animationMovingIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_MOVING;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                animationMovingIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER:
            {
                AnimationMovingBezier * animationMovingBezier = new AnimationMovingBezier;
                animationMovingBezier->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationMovingBezierV.push_back(*animationMovingBezier);
                delete animationMovingBezier;
                
                AnimationManagerIndex animationManagerIndex;
                
                animationManagerIndex.index = animationMovingIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_MOVING_BEZIER;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                animationMovingBezierIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_MULTIPLY_STILL:
            {
                AnimationBase * animationBase = new AnimationBase;
                animationBase->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationBaseV.push_back(*animationBase);
                delete animationBase;
                
                AnimationManagerIndex animationManagerIndex;
                
                animationManagerIndex.index = animationBaseIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_BASE;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                animationBaseIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING:
            {
                AnimationMoving * animationMoving = new AnimationMoving;
                animationMoving->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationMovingV.push_back(*animationMoving);
                delete animationMoving;
                
                AnimationManagerIndex animationManagerIndex;
                animationManagerIndex.index = animationMovingIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_MOVING;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                animationMovingIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING_BEZIER:
            {
                AnimationMovingBezier * animationMovingBezier = new AnimationMovingBezier;
                animationMovingBezier->setup(fileDirectory, type, duration, x, y, w, h, r);
                animationMovingBezierV.push_back(*animationMovingBezier);
                delete animationMovingBezier;
                
                AnimationManagerIndex animationManagerIndex;
                animationManagerIndex.index = animationMovingBezierIndexCount;
                animationManagerIndex.animationClass = ANIMATION_CLASS_MOVING_BEZIER;
                animationManagerIndex.drawIndex = drawIndex;
                animationIndices.push_back(animationManagerIndex);
                
                animationMovingBezierIndexCount++;
                break;
            }
        }
        sort(animationIndices.begin(), animationIndices.end(), compareDrawIndex);
    }
    void setLoopImage(AnimationManagerIndex & animationManagerIndex, bool b)
    {
        switch (animationManagerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                animationBaseV[animationManagerIndex.index].setLoopImage(b);
                break;
            }
            case ANIMATION_CLASS_MOVING:
            {
                animationMovingV[animationManagerIndex.index].setLoopImage(b);
                break;
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                animationMovingBezierV[animationManagerIndex.index].setLoopImage(b);
                break;
            }
        }
    }
    void setLoopImageBackward(AnimationManagerIndex & animationManagerIndex, bool b)
    {
        switch (animationManagerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                animationBaseV[animationManagerIndex.index].setLoopImageBackward(b);
                break;
            }
            case ANIMATION_CLASS_MOVING:
            {
                animationMovingV[animationManagerIndex.index].setLoopImageBackward(b);
                break;
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                animationMovingBezierV[animationManagerIndex.index].setLoopImageBackward(b);
                break;
            }
        }
    }
    
    void mousePressed(int x, int y, int button)
    {
        bool bNewObjectSelected = false;
        switch(AnimationBackend::getMode())
        {
            case ANIMATION_MODE_EDIT:
            case ANIMATION_MODE_VIEW:
            {
                for (int i = 0; i < animationIndices.size(); i++)
                {
                    switch(animationIndices[i].animationClass)
                    {
                        case ANIMATION_CLASS_BASE:
                        {
                            ofVec2f p = animationBaseV[animationIndices[i].index].getPosition();
                            ofVec2f d = animationBaseV[animationIndices[i].index].getDimension();
                            if (((x > p.x) && (y > p.y) && ((x < p.x+d.x) && (y < p.y+d.y))))
                            {
                                selectedObject = animationIndices[i];
                                bNewObjectSelected = true;
                                bObjectSelected = true;
                            }
                            break;
                        }
                        case ANIMATION_CLASS_MOVING:
                        {
                            ofVec2f p = animationMovingV[animationIndices[i].index].getPosition();
                            ofVec2f d = animationMovingV[animationIndices[i].index].getDimension();
                            if (((x > p.x) && (y > p.y) && ((x < p.x+d.x) && (y < p.y+d.y))))
                            {
                                selectedObject = animationIndices[i];
                                bNewObjectSelected = true;
                                bObjectSelected = true;
                            }
                            break;
                        }
                        case ANIMATION_CLASS_MOVING_BEZIER:
                        {
                            ofVec2f p = animationMovingBezierV[animationIndices[i].index].getPosition();
                            ofVec2f d = animationMovingBezierV[animationIndices[i].index].getDimension();
                            if (((x > p.x) && (y > p.y) && ((x < p.x+d.x) && (y < p.y+d.y))))
                            {
                                selectedObject = animationIndices[i];
                                bNewObjectSelected = true;
                                bObjectSelected = true;
                            }
                            break;
                        }
                    }
                    
                }
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
    }
    
    void update(unsigned long long time)
    {
        if ((AnimationBackend::getMode() == ANIMATION_MODE_PLAY) && (AnimationBackend::getPlayState() == ANIMATION_PLAY_STATE_PLAY))
        {
            unsigned long long playTime = AnimationBackend::calcCurrentPlayTime();
            
            //for (int i = 0; i < animationBaseV.size(); i++)
            //{
            //    animationBaseV[i].update(time);
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
                    animationBaseV[animationIndices[i].index].draw();
                    switch(AnimationBackend::getMode())
                    {
                        case ANIMATION_MODE_EDIT:
                        case ANIMATION_MODE_VIEW:
                        {
                            drawAssetBoxes(animationBaseV[animationIndices[i].index].getPosition(), animationBaseV[animationIndices[i].index].getDimension());
                        }
                        case ANIMATION_MODE_PLAY:
                        {
                            
                        }
                    }
                    break;
                }
                case ANIMATION_CLASS_MOVING:
                {
                    animationMovingV[animationIndices[i].index].draw();
                    switch(AnimationBackend::getMode())
                    {
                        case ANIMATION_MODE_EDIT:
                        case ANIMATION_MODE_VIEW:
                        {
                            drawAssetBoxes(animationMovingV[animationIndices[i].index].getPosition(), animationMovingV[animationIndices[i].index].getDimension());
                        }
                        case ANIMATION_MODE_PLAY:
                        {
                            
                        }
                    }
                    break;
                }
                case ANIMATION_CLASS_MOVING_BEZIER:
                {
                    animationMovingBezierV[animationIndices[i].index].draw();
                    switch(AnimationBackend::getMode())
                    {
                        case ANIMATION_MODE_EDIT:
                        case ANIMATION_MODE_VIEW:
                        {
                            drawAssetBoxes(animationMovingBezierV[animationIndices[i].index].getPosition(), animationMovingBezierV[animationIndices[i].index].getDimension());
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
        
    }
    void setModeView()
    {
        
    }
    void setModePlay()
    {
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
                //setLoopMovement(selectedObject, b);
                guiObjectLoopMovement = true;
            }
            
            //setLoopMovementBackward(selectedObject, b);
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
    int animationBaseIndexCount;
    vector<AnimationBase> animationBaseV;
    
    int animationMovingIndexCount;
    vector<AnimationMoving> animationMovingV;
    
    int animationMovingBezierIndexCount;
    vector<AnimationMovingBezier> animationMovingBezierV;
    
    vector<AnimationManagerIndex> animationIndices;
    
    bool bObjectSelected;
    AnimationManagerIndex selectedObject;
    
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
