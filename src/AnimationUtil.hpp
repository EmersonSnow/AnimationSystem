//
//  AnimationUtil.hpp
//  animationTest
//
//  Created by Zachary Snow on 07/08/2017.
//
//

#pragma once

class AnimationUtil
{
public:
    static bool getImageMultiply(AnimationType type)
    {
        if ((type == ANIMATION_TYPE_IMAGE_SINGLE_STILL) || (type == ANIMATION_TYPE_IMAGE_SINGLE_MOVING) || (type == ANIMATION_TYPE_IMAGE_SINGLE_MOVING_MULTIPLY) || (type == ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER) || (type == ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER_MULTIPLY))
        {
            return false;
        } else if ((type == ANIMATION_TYPE_IMAGE_MULTIPLY_STILL) || (type == ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING) || (type == ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING_MULTIPLY) || (type == ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING_MULTIPLY) || (ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING_BEZIER) || (type == ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING_BEZIER_MULTIPLY))
        {
            return true;
        }
    }
};
