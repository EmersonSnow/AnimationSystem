//
//  AnimationBackend.cpp
//  animationTest
//
//  Created by Zachary Snow on 09/08/2017.
//
//

#include <AnimationBackend.hpp>

AnimationEngineMode AnimationBackend::mode = ANIMATION_MODE_VIEW;
AnimationPlayState AnimationBackend::playState = ANIMATION_PLAY_STATE_STOPPED;

unsigned long long AnimationBackend::timeCurrentPlay = 0;
unsigned long long AnimationBackend::timeStartPlay = 0;
unsigned long long AnimationBackend::timePause = 0;

vector<AnimationImagesContainer> AnimationBackend::animationImages;
