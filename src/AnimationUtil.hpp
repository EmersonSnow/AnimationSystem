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
    static bool getImageMultiple(AnimationType type)
    {
        if ((type == ANIMATION_TYPE_IMAGE_SINGLE_STILL) || (type == ANIMATION_TYPE_IMAGE_SINGLE_MOVING) || (type == ANIMATION_TYPE_IMAGE_SINGLE_MOVING_MULTIPLE) || (type == ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER) || (type == ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER_MULTIPLE))
        {
            return false;
        } else if ((type == ANIMATION_TYPE_IMAGE_MULTIPLE_STILL) || (type == ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING) || (type == ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_MULTIPLE) || (type == ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_MULTIPLE) || (ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_BEZIER) || (type == ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_BEZIER_MULTIPLE))
        {
            return true;
        }
    }
    
    
};
