#pragma once

#include "ofMain.h"
#include "AnimationDefinitions.hpp"
#include "AnimationObjectClasses.hpp"
#include "AnimationManager.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        AnimationObjectMovingBezier animation;
    AnimationObjectMoving animation1;
    AnimationObjectMoving animation2;
    AnimationObjectMoving animation3;
    
    AnimationManager animationManager;
};
