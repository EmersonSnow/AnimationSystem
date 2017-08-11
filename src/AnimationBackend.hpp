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
        if (mode != ANIMATION_MODE_PLAY)
            return 0;
        
        return timeCurrentPlay;
    }
    
    ///Calcuate and return current play time, should be called every play tick
    static unsigned long long calcCurrentPlayTime()
    {
        //if (mode != ANIMATION_MODE_PLAY)
        //    return 0;
        //if (playState != ANIMATION_PLAY_STATE_PLAY)
        //    return 0;
        
        timeCurrentPlay = ofGetElapsedTimeMicros() - timeStartPlay;
        return timeCurrentPlay;
    }
    
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
            case ANIMATION_PLAY_STATE_STOPPED:
            {
                timeCurrentPlay = 0;
                timeStartPlay = 0;
                break;
            }
            case ANIMATION_PLAY_STATE_PAUSED:
            {
                timePause = ofGetElapsedTimeMicros();
                break;
            }
            case ANIMATION_PLAY_STATE_PLAY:
            {
                switch(playState)
                {
                    case ANIMATION_PLAY_STATE_STOPPED:
                    {
                        timeStartPlay = ofGetElapsedTimeMicros();
                        break;
                    }
                    case ANIMATION_PLAY_STATE_PAUSED:
                    {
                        timeStartPlay = timeStartPlay+(ofGetElapsedTimeMicros()-timePause);
                        break;
                    }
                    case ANIMATION_PLAY_STATE_PLAY:
                    {
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
    
private:
    static AnimationEngineMode mode;
    static AnimationPlayState playState;
    
    static unsigned long long timeCurrentPlay;
    static unsigned long long timeStartPlay;
    static unsigned long long timePause;
    
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

