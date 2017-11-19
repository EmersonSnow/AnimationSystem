
#pragma once

#include "ofMain.h"

#define ANIMATION_CALCUATION_RATE 8333 //Roughly every 120 frames
#define ANIMATION_STACK_MAX 100
enum AnimationEngineMode
{
    ANIMATION_MODE_EDIT,
    ANIMATION_MODE_VIEW,
    ANIMATION_MODE_PLAY,
};

enum AnimationPlayState
{
    ANIMATION_PLAY_STATE_PLAY,
    ANIMATION_PLAY_STATE_PAUSE,
    ANIMATION_PLAY_STATE_STOP
};
enum AnimationLoopState
{
    ANIMATION_LOOP_STATE_NONE,
    ANIMATION_LOOP_STATE_FORWARD,
    ANIMATION_LOOP_STATE_BACKWARD
};
enum AnimtionFinishState
{
    ANIMATION_FINISH_STATE_STATIC,
    ANIMATION_FINISH_STATE_DELETE,
    ANIMATION_FINISH_STATE_DELETE_FADE,
    //TODO add more
};
///EnumDefinition_AnimationType_Movement_MovementType_MULTIPLE
enum AnimationType
{
    ANIMATION_TYPE_IMAGE_SINGLE_STILL,
    ANIMATION_TYPE_IMAGE_SINGLE_MOVING,
    ANIMATION_TYPE_IMAGE_SINGLE_MOVING_MULTIPLE,
    //ANIMATION_TYPE_IMAGE_SINGLE_MOVING_LOOP,
    //ANIMATION_TYPE_IMAGE_SINGLE_MOVING_LOOP_BACKWARD,
    ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER,
    ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER_MULTIPLE,
    //ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER_LOOP,
    //ANIMATION_TYPE_IMAGE_SINGLE_MOVING_BEZIER_LOOP_BACKWARD,
    ANIMATION_TYPE_IMAGE_MULTIPLE_STILL,
    ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING,
    ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_MULTIPLE,
    ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_BEZIER,
    ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_BEZIER_MULTIPLE,
    
    //To add
    ANIMATION_TYPE_MODEL_STILL,
    ANIMATION_TYPE_MODEL_MOVING,
    
    ANIMATION_TYPE_PARTICLE_STILL,
    ANIMATION_TYPE_PARTICLE_MOVING
};

enum AnimationClass
{
    ANIMATION_CLASS_BASE,
    ANIMATION_CLASS_MOVING,
    ANIMATION_CLASS_MOVING_BEZIER
};

enum AnimationDrawMode
{
    ANIMATION_DRAW_ONLY_START,
    ANIMATION_DRAW_BEFORE_START,
    ANIMATION_DRAW_AFTER_START,
    ANIMATION_DRAW_BEFORE_AFTER_START
};
enum AnimationScreenType
{
    ANIMATION_SCREEN_TYPE_STILL,
    ANIMATION_SCREEN_TYPE_MOVING,
    ANIMATION_SCREEN_TYPE_ZOOM
};

enum AnimationManagerUtilImage
{
    ANIMATION_MANAGER_UTIL_IMAGE_POINT = 0
};

enum AnimationManagerGUIWindow
{
    ANIMATION_MANAGER_GUI_WINDOW_MODE,
    ANIMATION_MANAGER_GUI_WINDOW_OBJECT,
    ANIMATION_MANAGER_GUI_WINDOW_OBJECT_CREATE,
    ANIMATION_MANAGER_GUI_WINDOW_OBJECT_IMAGE_DURATION,
    ANIMATION_MANAGER_GUI_WINDOW_PLAY
};

enum AnimationObjectEvents
{
    ANIMATION_START,
    ANIMATION_STOP
};

struct AnimationOrigin
{
    ofVec2f position;
    ofVec2f dimension;
    float rotation;
};
struct AnimationDuration
{
    unsigned long long l;
    float f;
};
struct AnimationEventContainer
{
    bool bHasBeenTriggered;
    int objectUniqueIndex; //Index of animationManagerIndex
    AnimationObjectEvents event;
    unsigned long long timeTrigger;
    //TODO: possibly add variables for events
};

struct AnimationImageInstance
{
    int drawIndex;
    int containerIndex;
    vector<unsigned long long> durations;
    unsigned long long totalDuration; //Calcuated from vector duration of each image
};

struct AnimationMovementPoint
{
    int index;
    bool bEndPoint;
    AnimationOrigin origin;
    AnimationDuration duration;
};
struct AnimationBezierCurve
{
    int index;
    bool bEndPoint;
    AnimationDuration duration;
    AnimationOrigin origin;
    ofVec2f curvePoints[4];
    vector<ofVec2f> curvePositions;
};
struct AnimationManagerIndex
{
    int index;
    int uniqueIndex;
    AnimationClass animationClass;
    int drawIndex;
};

struct AnimationImagesContainer
{
    string directoryPath;
    vector<ofImage> images;
};
