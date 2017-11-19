//
//  AnimationBackend.hpp
//  animationTest
//
//  Created by Zachary Snow on 07/08/2017.
//
//

#pragma once
#include "ofMain.h"
#include "AnimationDefinitions.hpp"


class AnimationBackend
{
public:
    static void setMode(AnimationEngineMode _mode)
    {
        mode = _mode;
        switch(mode)
        {
            case ANIMATION_MODE_EDIT:
            {
                setModeEdit();
                return;
            }
            case ANIMATION_MODE_VIEW:
            {
                setModeView();
                return;
            }
            case ANIMATION_MODE_PLAY:
            {
                setModePlay();
                return;
            }
        }
    }
    
    static AnimationEngineMode getMode()
    {
        return mode;
    }
    
    ///Return the current play time
    static unsigned long long getCurrentPlayTime()
    {
        switch(getPlayState())
        {
            case ANIMATION_PLAY_STATE_PLAY:
            {
                timeCurrentPlay = ofGetElapsedTimeMicros() - timeStartPlay;
                break;
            }
            case ANIMATION_PLAY_STATE_PAUSE:
            {
                timeCurrentPlay = timePause - timeStartPlay;
                break;
            }
            case ANIMATION_PLAY_STATE_STOP:
            {
                timeCurrentPlay = 0;
                break;
            }
        }
        return timeCurrentPlay;
    }
    
    ///Calcuate and return current play time, should be called every play tick
    /*static unsigned long long calcCurrentPlayTime()
    {
        //if (mode != ANIMATION_MODE_PLAY)
        //    return 0;
        //if (playState != ANIMATION_PLAY_STATE_PLAY)
        //    return 0;
        
        timeCurrentPlay = ofGetElapsedTimeMicros() - timeStartPlay;
        return timeCurrentPlay;
    }*/
    
    /*///Possible function, would require a lot of coding to make
    void setCurrentPlayTime(unsigned long long timePlay)
    {
        timeCurrentPlay = timePlay;
    }*/
    
    static void setPlayState(AnimationPlayState _playState)
    {
        //if (mode != ANIMATION_MODE_PLAY)
        //    return;
        switch(_playState)
        {
            case ANIMATION_PLAY_STATE_STOP:
            {
                timeCurrentPlay = 0;
                timeStartPlay = 0;
                timePause = 0;
                break;
            }
            case ANIMATION_PLAY_STATE_PAUSE:
            {
                timePause = ofGetElapsedTimeMicros();
                break;
            }
            case ANIMATION_PLAY_STATE_PLAY:
            {
                switch(playState)
                {
                    case ANIMATION_PLAY_STATE_STOP:
                    {
                        timeStartPlay = ofGetElapsedTimeMicros();
                        break;
                    }
                    case ANIMATION_PLAY_STATE_PAUSE:
                    {
                        timeStartPlay = timeStartPlay+(ofGetElapsedTimeMicros()-timePause);
                        break;
                    }
                    case ANIMATION_PLAY_STATE_PLAY:
                    {
                        //Do nothing already playing
                        //setPlayState(ANIMATION_PLAY_STATE_STOPPED);
                        //timeStartPlay = ofGetElapsedTimeMicros();
                        break;
                    }
                }
                break;
            }
        }

        playState = _playState;
        
    }
    static AnimationPlayState getPlayState()
    {
        return playState;
    }
    
    static int loadImages(string directoryPath)
    {
        for (int i = 0; i < animationImages.size(); i++)
        {
            if (animationImages[i].directoryPath == directoryPath)
            {
                return i;
            }
        }
        ofDirectory d(ofToDataPath(directoryPath));
        d.listDir();
        d.sort();
        
        AnimationImagesContainer * imagesContainer = new AnimationImagesContainer;
        imagesContainer->directoryPath = directoryPath;
        for (int i = 0; i < d.size(); i++)
        {
            imagesContainer->images.push_back(*new ofImage);
            imagesContainer->images[imagesContainer->images.size()-1].load(d.getFile(i));
        }
        
        animationImages.push_back(*imagesContainer);
        delete imagesContainer;
        
        return animationImages.size()-1;
    }
    
    static ofImage & getDrawImage(int containerIndex, int imageIndex)
    {
        if (containerIndex >= animationImages.size())
            return;
        if (imageIndex >= animationImages[containerIndex].images.size())
            return;
        
        return animationImages[containerIndex].images[imageIndex];
    }
    
    static int getNumberImages(int containerIndex)
    {
        if (containerIndex >= animationImages.size())
            return;
        
        return animationImages[containerIndex].images.size();
    }
private:
    static AnimationEngineMode mode;
    static AnimationPlayState playState;
    
    static unsigned long long timeCurrentPlay;
    static unsigned long long timeStartPlay;
    static unsigned long long timePause;
    
    static vector<AnimationImagesContainer> animationImages;
    static void setModeEdit()
    {
        
    }
    static void setModeView()
    {
        
    }
    static void setModePlay()
    {
        
    }
};

