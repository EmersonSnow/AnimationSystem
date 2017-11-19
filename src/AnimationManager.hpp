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
#include "AnimationTemplate.hpp"
#include "ofThread.h"
#include "ofxGui.h"

class AnimationManager : public ofThread
{
public:
    AnimationManager()
    {
        bObjectSelected = false;
        
        animationObjectBaseIndexCount = 0;
        animationObjectMovingIndexCount = 0;
        animationObjectMovingBezierIndexCount = 0;
        animationObjectUniqueIndexCount = 0;
        
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
                int setObjectLoopImageNumber = 0;
                switch (selectedObject.animationClass)
                {
                    case ANIMATION_CLASS_BASE:
                    {
                        bLoopImage = vAnimationObjectBase[selectedObject.index].getLoopImage();
                        bLoopImageBackward = vAnimationObjectBase[selectedObject.index].getLoopImageBackward();
                        guiObjectLoopImageNumber = vAnimationObjectBase[selectedObject.index].getLoopImageNumber();
                        break;
                    }
                    case ANIMATION_CLASS_MOVING:
                    {
                        bLoopImage = vAnimationObjectMoving[selectedObject.index].getLoopImage();
                        bLoopImageBackward = vAnimationObjectMoving[selectedObject.index].getLoopImageBackward();
                        guiObjectLoopImageNumber = vAnimationObjectMoving[selectedObject.index].getLoopImageNumber();
                        break;
                    }
                    case ANIMATION_CLASS_MOVING_BEZIER:
                    {
                        bLoopImage = vAnimationObjectMovingBezier[selectedObject.index].getLoopImage();
                        bLoopImageBackward = vAnimationObjectMovingBezier[selectedObject.index].getLoopImageBackward();
                        guiObjectLoopImageNumber = vAnimationObjectMovingBezier[selectedObject.index].getLoopImageNumber();
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
                guiObject.add(guiObjectLoopImageNumber.set("Loop Image Number", guiObjectLoopImageNumber, 0, 120));
                
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
                        bLoopMovement = vAnimationObjectMoving[selectedObject.index].getLoopMovement();
                        bLoopMovementBackward = vAnimationObjectMoving[selectedObject.index].getLoopMovementBackward();
                        guiBuildLoopMovement(bLoopMovement, bLoopMovementBackward);
                        break;
                    }
                    case ANIMATION_CLASS_MOVING_BEZIER:
                    {
                        bLoopMovement = vAnimationObjectMovingBezier[selectedObject.index].getLoopMovement();
                        bLoopMovementBackward = vAnimationObjectMovingBezier[selectedObject.index].getLoopMovementBackward();
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
                
                guiPlay.add(guiPlayStringTime.set("Time", "00:00:00"));
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
                vAnimationObjectBase.push_back(*animationObjectBase);
                delete animationObjectBase;
                
                AnimationManagerIndex managerIndex;

                managerIndex.index = animationObjectBaseIndexCount;
                managerIndex.uniqueIndex = animationObjectUniqueIndexCount;
                managerIndex.animationClass = ANIMATION_CLASS_BASE;
                managerIndex.drawIndex = drawIndex;
                animationIndices.push_back(managerIndex);
                
                animationObjectUniqueIndexCount++;
                animationObjectBaseIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_SINGLE_MOVING:
            {
                AnimationObjectMoving * animationObjectMoving = new AnimationObjectMoving;
                animationObjectMoving->setup(fileDirectory, type, duration, x, y, w, h, r);
                vAnimationObjectMoving.push_back(*animationObjectMoving);
                delete animationObjectMoving;
                
                AnimationManagerIndex managerIndex;
                
                managerIndex.index = animationObjectMovingIndexCount;
                managerIndex.uniqueIndex = animationObjectUniqueIndexCount;
                managerIndex.animationClass = ANIMATION_CLASS_MOVING;
                managerIndex.drawIndex = drawIndex;
                animationIndices.push_back(managerIndex);
                
                animationObjectUniqueIndexCount++;
                animationObjectMovingIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER:
            {
                AnimationObjectMovingBezier * animationObjectMovingBezier = new AnimationObjectMovingBezier;
                animationObjectMovingBezier->setup(fileDirectory, type, duration, x, y, w, h, r);
                vAnimationObjectMovingBezier.push_back(*animationObjectMovingBezier);
                delete animationObjectMovingBezier;
                
                AnimationManagerIndex managerIndex;
                
                managerIndex.index = animationObjectMovingIndexCount;
                managerIndex.uniqueIndex = animationObjectUniqueIndexCount;
                managerIndex.animationClass = ANIMATION_CLASS_MOVING_BEZIER;
                managerIndex.drawIndex = drawIndex;
                animationIndices.push_back(managerIndex);
                
                animationObjectUniqueIndexCount++;
                animationObjectMovingBezierIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_MULTIPLE_STILL:
            {
                AnimationObjectBase * animationObjectBase = new AnimationObjectBase;
                animationObjectBase->setup(fileDirectory, type, duration, x, y, w, h, r);
                vAnimationObjectBase.push_back(*animationObjectBase);
                delete animationObjectBase;
                
                AnimationManagerIndex managerIndex;
                
                managerIndex.index = animationObjectBaseIndexCount;
                managerIndex.uniqueIndex = animationObjectUniqueIndexCount;
                managerIndex.animationClass = ANIMATION_CLASS_BASE;
                managerIndex.drawIndex = drawIndex;
                animationIndices.push_back(managerIndex);
                
                animationObjectUniqueIndexCount++;
                animationObjectBaseIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING:
            {
                AnimationObjectMoving * animationObjectMoving = new AnimationObjectMoving;
                animationObjectMoving->setup(fileDirectory, type, duration, x, y, w, h, r);
                vAnimationObjectMoving.push_back(*animationObjectMoving);
                delete animationObjectMoving;
                
                AnimationManagerIndex managerIndex;
                managerIndex.index = animationObjectMovingIndexCount;
                managerIndex.uniqueIndex = animationObjectUniqueIndexCount;
                managerIndex.animationClass = ANIMATION_CLASS_MOVING;
                managerIndex.drawIndex = drawIndex;
                animationIndices.push_back(managerIndex);
                
                animationObjectUniqueIndexCount++;
                animationObjectMovingIndexCount++;
                break;
            }
            case ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_BEZIER:
            {
                AnimationObjectMovingBezier * animationObjectMovingBezier = new AnimationObjectMovingBezier;
                animationObjectMovingBezier->setup(fileDirectory, type, duration, x, y, w, h, r);
                vAnimationObjectMovingBezier.push_back(*animationObjectMovingBezier);
                delete animationObjectMovingBezier;
                
                AnimationManagerIndex managerIndex;
                managerIndex.index = animationObjectMovingBezierIndexCount;
                managerIndex.uniqueIndex = animationObjectUniqueIndexCount;
                managerIndex.animationClass = ANIMATION_CLASS_MOVING_BEZIER;
                managerIndex.drawIndex = drawIndex;
                animationIndices.push_back(managerIndex);
                
                animationObjectUniqueIndexCount++;
                animationObjectMovingBezierIndexCount++;
                break;
            }
        }
        sort(animationIndices.begin(), animationIndices.end(), compareDrawIndex);
    }
    /*template<class T>
    T & getAnimationObject(AnimationManagerIndex & managerIndex)
    {
        switch (managerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                return vAnimationObjectBase[managerIndex.index];
            }
            case ANIMATION_CLASS_MOVING:
            {
                return vAnimationObjectMoving[managerIndex.index];
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                return vAnimationObjectMovingBezier[managerIndex.index];
            }
        }
    }*/
    
    void createObjectEvent(AnimationManagerIndex & managerIndex, AnimationObjectEvents event, unsigned long long time)
    {
        AnimationEventContainer * eventContainer = new AnimationEventContainer;
        eventContainer->bHasBeenTriggered = false;
        eventContainer->objectUniqueIndex = managerIndex.uniqueIndex;
        eventContainer->event = event;
        eventContainer->timeTrigger = time;
        
        animationEvents.push_back(*eventContainer);
        delete eventContainer;
    }
    void resetObjectEvents()
    {
        for (int i = 0; i < animationEvents.size(); i++)
        {
            animationEvents[i].bHasBeenTriggered = false;
        }
    }
    AnimationManagerIndex & getObjectManagerIndexByUniqueIndex(int index)
    {
        for (int i = 0; animationIndices.size(); i++)
        {
            if (animationIndices[i].uniqueIndex == index)
            {
                return animationIndices[i];
            }
        }
    }
    /*template<class T>
    bool getObjectLoopImageTemplate(T test)
    {
        return test.getLoopImage();
    }*/
    AnimationObjectBase & getAnimationObject(AnimationManagerIndex & managerIndex)
    {
        switch (managerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                return vAnimationObjectBase[managerIndex.index];
            }
            case ANIMATION_CLASS_MOVING:
            {
                return vAnimationObjectMoving[managerIndex.index];
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                return vAnimationObjectMovingBezier[managerIndex.index];
            }
        }
    }
    AnimationObjectMoving & getAnimationObjectMoving(AnimationManagerIndex & managerIndex)
    {
        switch (managerIndex.animationClass)
        {
            case ANIMATION_CLASS_BASE:
            {
                return;
            }
            case ANIMATION_CLASS_MOVING:
            {
                return vAnimationObjectMoving[managerIndex.index];
            }
            case ANIMATION_CLASS_MOVING_BEZIER:
            {
                return vAnimationObjectMovingBezier[managerIndex.index];
            }
        }
    }
    void setObjectLoopImage(AnimationManagerIndex & managerIndex, bool b)
    {
        getAnimationObject(managerIndex).setLoopImage(b);
    }
    
    bool getObjectLoopImage(AnimationManagerIndex & managerIndex)
    {
        return getAnimationObject(managerIndex).getLoopImage();
    }
    void setObjectLoopImageBackward(AnimationManagerIndex & managerIndex, bool b)
    {
        getAnimationObject(managerIndex).setLoopImageBackward(b);
    }
    void getObjectLoopImageBackward(AnimationManagerIndex & managerIndex)
    {
        getAnimationObject(managerIndex).getLoopImageBackward();
    }
    void setObjectLoopMovement(AnimationManagerIndex & managerIndex, bool b)
    {
        getAnimationObjectMoving(managerIndex).setLoopMovement(b);
    }
    bool getObjectLoopMovement(AnimationManagerIndex & managerIndex)
    {
        getAnimationObjectMoving(managerIndex).getLoopMovement();
    }
    void setObjectLoopMovementBackward(AnimationManagerIndex & managerIndex, bool b)
    {
        getAnimationObjectMoving(managerIndex).setLoopMovementBackward(b);
    }
    bool getObjectLoopMovementBackward(AnimationManagerIndex & managerIndex)
    {
        getAnimationObjectMoving(managerIndex).getLoopMovementBackward();
    }
    void startObject(AnimationManagerIndex & managerIndex)
    {
        //getAnimationObject(managerIndex).start(ofGetElapsedTimeMicros());
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
                                    if (!bNewObjectSelected)
                                    {
                                        ofVec2f p = vAnimationObjectBase[animationIndices[i].index].getPosition();
                                        ofVec2f d = vAnimationObjectBase[animationIndices[i].index].getDimension();
                                        bNewObjectSelected = isObjectBeingPressed(i, x, y, p, d);
                                    }
                                    break;
                                }
                                case ANIMATION_CLASS_MOVING:
                                {
                                    if (!bNewObjectSelected)
                                    {
                                        ofVec2f p = vAnimationObjectMoving[animationIndices[i].index].getPosition();
                                        ofVec2f d = vAnimationObjectMoving[animationIndices[i].index].getDimension();
                                        bNewObjectSelected = isObjectBeingPressed(i, x, y, p, d);
                                    }
                                    break;
                                }
                                case ANIMATION_CLASS_MOVING_BEZIER:
                                {
                                    if (!bNewObjectSelected)
                                    {
                                        ofVec2f p = vAnimationObjectMovingBezier[animationIndices[i].index].getPosition();
                                        ofVec2f d = vAnimationObjectMovingBezier[animationIndices[i].index].getDimension();
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
                        vAnimationObjectMoving[selectedObject.index].setPosition(offsetDragObject.x+x, offsetDragObject.y+y);
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
    
    //TODO: It working, but formatting of the zeros need work
    string formatePlayTimeString(unsigned long long time)
    {
        //stringstream ss;
        //string stringTime = ofToString(time);
        int timeRounded = time / 10000;
        string timeMil;
        string timeSec = "00";
        string timeMin = "000";
        if ((time >= 10000) && (time <= 1000000))
        {
            if (time >= 100000)
            {
                //cout << "Two Digit " << time << " Time Float " << time/1000000.0 << "\n";
                timeMil = ofToString(timeRounded);
            } else
            {
                //cout << "One Digit " << time << "\n";
                timeMil = "0" + ofToString(time);
            }
        } else if ((time >= 1000000) && (time < 60000000))
        {
            int nearestHundred = ((timeRounded + 50) / 100 * 100);
            timeMil = ofToString(timeRounded - nearestHundred + 50); //For some reason I have to add 50
            nearestHundred /= 100;
            if (nearestHundred < 10)
            {
                timeSec = "0" + ofToString(nearestHundred);
            } else
            {
                timeSec = ofToString(nearestHundred);
            }
        }
        else
        {
            int nearestHundred = ((timeRounded + 50) / 100 * 100);
            timeMil = ofToString(timeRounded - nearestHundred + 50); //For some reason I have to add 5
            nearestHundred /= 100;
            if (nearestHundred >= 60)
            {
                int numberMinutes = nearestHundred / 60;
                nearestHundred -= numberMinutes*60;
                if (nearestHundred < 10)
                {
                    timeSec = "0" + ofToString(nearestHundred);
                } else
                {
                    timeSec = ofToString(nearestHundred);
                }
                if (numberMinutes < 10)
                {
                    timeMin = "00" + ofToString(numberMinutes);
                } else if (numberMinutes < 100)
                {
                    timeMin = "0" + ofToString(numberMinutes);
                }
            }
        }
        return timeMin + ":" + timeSec + ":" + timeMil;
    }
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            unsigned long long time = ofGetElapsedTimeMicros();
            float timef = ofGetElapsedTimef();
            
            for (int i = 0; i < animationEvents.size(); i++)
            {
                if (!animationEvents[i].bHasBeenTriggered)
                {
                    if (animationEvents[i].timeTrigger >= AnimationBackend::getCurrentPlayTime())
                    {
                        //TODO: events
                        animationEvents[i].bHasBeenTriggered = true;
                    }
                }
            }
            
            
            
            //TODO: Only call update if object actually needs updating
            for (int i = 0; i < vAnimationObjectBase.size(); i++)
            {
                vAnimationObjectBase[i].update(time);
            }
            for (int i = 0; i < vAnimationObjectMoving.size(); i++)
            {
                //vAnimationObjectMoving[i].update(time, timef);
            }
            for (int i = 0; i < vAnimationObjectMovingBezier.size(); i++)
            {
                vAnimationObjectMovingBezier[i].update(time, timef);
            }
            
            
            
            
            
            if (AnimationBackend::getMode() == ANIMATION_MODE_PLAY)
            {
                if ((AnimationBackend::getPlayState() == ANIMATION_PLAY_STATE_PLAY) || (AnimationBackend::getPlayState() == ANIMATION_PLAY_STATE_PAUSE))
                {
                    guiPlayStringTime = formatePlayTimeString(AnimationBackend::getCurrentPlayTime());
                } else
                {
                    if (!guiPlayStopStringSet)
                    {
                        guiPlayStringTime = "000:00:00";
                        guiPlayStopStringSet = true;
                    }
                }
            }
        }
    }
    void update(unsigned long long time)
    {
        if ((AnimationBackend::getMode() == ANIMATION_MODE_PLAY) && (AnimationBackend::getPlayState() == ANIMATION_PLAY_STATE_PLAY))
        {
            //unsigned long long playTime = AnimationBackend::calcCurrentPlayTime();
            
            //for (int i = 0; i < vAnimationObjectBase.size(); i++)
            //{
            //    vAnimationObjectBase[i].update(time);
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
    void drawMovementLine(ofVec2f start, ofVec2f end)
    {
        ofSetColor(0, 0, 0);
        
        ofDrawLine(start.x, start.y, end.x, end.y);
        
        ofSetColor(255, 255, 255);
        
    }
    void drawBezierCurve(ofVec2f a, ofVec2f b, ofVec2f c, ofVec2f d)
    {
        ofSetColor(0, 0, 0);
        
        ofCurvePoint(a, b, c, d, 0);
        
        ofSetColor(255, 255, 255);
    }
    void draw()
    {
        for (int i = 0; i < animationIndices.size(); i++)
        {
            switch(animationIndices[i].animationClass)
            {
                case ANIMATION_CLASS_BASE:
                {
                    vAnimationObjectBase[animationIndices[i].index].draw();
                    switch(AnimationBackend::getMode())
                    {
                        case ANIMATION_MODE_EDIT:
                        case ANIMATION_MODE_VIEW:
                        {
                            drawAssetBoxes(vAnimationObjectBase[animationIndices[i].index].getPosition(), vAnimationObjectBase[animationIndices[i].index].getDimension());
                            break;
                        }
                        case ANIMATION_MODE_PLAY:
                        {
                            break;
                        }
                    }
                    break;
                }
                case ANIMATION_CLASS_MOVING:
                {
                    vAnimationObjectMoving[animationIndices[i].index].draw();
                    switch(AnimationBackend::getMode())
                    {
                        case ANIMATION_MODE_EDIT:
                        case ANIMATION_MODE_VIEW:
                        {
                            drawAssetBoxes(vAnimationObjectMoving[animationIndices[i].index].getPosition(), vAnimationObjectMoving[animationIndices[i].index].getDimension());
                            drawMovementLine(vAnimationObjectMoving[animationIndices[i].index].getPosition(), vAnimationObjectMoving[animationIndices[i].index].getMovementPoint().origin.position);
                            break;
                        }
                        case ANIMATION_MODE_PLAY:
                        {
                            break;
                        }
                    }
                    break;
                }
                case ANIMATION_CLASS_MOVING_BEZIER:
                {
                    vAnimationObjectMovingBezier[animationIndices[i].index].draw();
                    switch(AnimationBackend::getMode())
                    {
                        case ANIMATION_MODE_EDIT:
                        case ANIMATION_MODE_VIEW:
                        {
                            drawAssetBoxes(vAnimationObjectMovingBezier[animationIndices[i].index].getPosition(), vAnimationObjectMovingBezier[animationIndices[i].index].getDimension());
                            break;
                            
                        }
                        case ANIMATION_MODE_PLAY:
                        {
                            break;
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
        AnimationBackend::setPlayState(ANIMATION_PLAY_STATE_PLAY);
        guiPlayStopStringSet = false;
        guiBuild(ANIMATION_MANAGER_GUI_WINDOW_PLAY);
    }
    void guiOnLoopImage(bool & b)
    {
        if (bObjectSelected)
        {
            setObjectLoopImage(selectedObject, b);
            //setObjectLoopImageTemplate(getAnimationObject(0), b);
        }
    }
    
    void guiOnLoopImageBackward(bool & b)
    {
        if (bObjectSelected)
        {
            if (b)
            {
                setObjectLoopImage(selectedObject, b);
                guiObjectLoopImage = true;
            }
            
            setObjectLoopImageBackward(selectedObject, b);
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
            setObjectLoopImage(selectedObject, b);
        }
    }
    
    void guiOnLoopMovementBackward(bool & b)
    {
        if (bObjectSelected)
        {
            if (b)
            {
                if (!getObjectLoopMovementBackward(selectedObject))
                {
                    setObjectLoopMovement(selectedObject, b);
                }
                
                setObjectLoopMovementBackward(selectedObject, b);
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
            AnimationBackend::setPlayState(ANIMATION_PLAY_STATE_PLAY);
            guiPlayStopStringSet = false;
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
            guiPlayStopStringSet = false;
            AnimationBackend::setPlayState(ANIMATION_PLAY_STATE_PAUSE);
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
            guiPlayStopStringSet = false;
            AnimationBackend::setPlayState(ANIMATION_PLAY_STATE_STOP);
        } else if ((guiPlayButtonPlay == false) && (guiPlayButtonPause == false))
        {
            guiPlayButtonStop = true;
        }
    }
private:
    int animationObjectBaseIndexCount;
    vector<AnimationObjectBase> vAnimationObjectBase;
    
    int animationObjectMovingIndexCount;
    vector<AnimationObjectMoving> vAnimationObjectMoving;
    
    int animationObjectMovingBezierIndexCount;
    vector<AnimationObjectMovingBezier> vAnimationObjectMovingBezier;
    
    vector<AnimationManagerIndex> animationIndices;
    int animationObjectUniqueIndexCount;
    
    
    vector<AnimationEventContainer> animationEvents;
    
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
    
    bool guiPlayStopStringSet;
    //ofParameter<bool> guiButtonRewind;
    ofParameter<bool> guiPlayButtonPlay;
    //ofParameter<bool> guiButtonFastForward;
    ofParameter<bool> guiPlayButtonPause;
    ofParameter<bool> guiPlayButtonStop;
    ofParameter<string> guiPlayStringTime;
    
};
