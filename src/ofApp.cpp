#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    int windowX = ofGetViewportWidth();
    int windowY = ofGetViewportHeight();
    animation.setup("dev/", ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING_BEZIER, 5, 0, 0, 100, 100, 0);
    animation.setMovementContainerPosition(animation.animationMovingContainer, 0, windowY/2);
    animation.setMovementContainerRotation(animation.animationMovingContainer, 360);
    animation.setLoopImage(true);
    animation.setLoopImageBackward(true);
    animation.setLoopMovement(true);
    animation.setLoopMovementDuration(animation.getTotalDuration()/1000000);
    //animation.setCalcBezierPointNumber(1000);
    
    //animation1.setup("dev/", ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING, 5, windowX-100, 0, 100, 100, 0);
    //animation1.setEndDrawPosition(windowX/2, windowY/2);
    //animation1.setEndDrawRotation(360);
    //animation1.setLoopImage(true);
    //animation1.setLoopImageBackward(true);
    
    /*animation2.setup("dev/", ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING, 5, 0, windowY-100, 100, 100, 0);
    animation2.setEndDrawPosition(windowX/2, windowY/2);
    animation2.setEndDrawRotation(360);
    animation2.setLoopMovement(true);
    animation2.setLoopMovementDuration(animation2.getTotalDuration()/1000000);
    animation2.setLoopMovementBackward(true);
    //animation2.setLoopImage(true);
    //animation2.setLoopImageBackward(true);
    
    //animation3.setup("dev/", ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING, 5, windowX-100, windowY-100, 100, 100, 0);
    //animation3.setEndDrawPosition(windowX/2, windowY/2);
    //animation3.setEndDrawRotation(360);
    //animation3.setLoopImage(true);
    //animation3.setLoopImageBackward(true);
    
    unsigned long long time = ofGetElapsedTimeMicros();
    animation.start(time);
    //animation1.start(time);
    animation2.start(time);
    //animation3.start(time);*/
    
    animationManager.createAnimationImage("dev/", ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING, 5, 10, 10, 100, 100, 0, 5);
    animationManager.createAnimationImage("dev/", ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING, 5, 110, 10, 100, 100, 0, 3);
    animationManager.createAnimationImage("dev/", ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING, 5, 210, 10, 100, 100, 0, 1);
    AnimationManagerIndex index = animationManager.createAnimationImage("dev/", ANIMATION_TYPE_IMAGE_MULTIPLE_MOVING, 5, 310, 10, 100, 100, 0, 2);
    //animationManager.getLoopImageTemplate();
    //animationManager.getAnimationObject(index);
    //animationManager.createAnimationImage("dev/", ANIMATION_TYPE_IMAGE_MULTIPLY_MOVING_BEZIER, 5, 10, 400, 100, 100, 0, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
    unsigned long long time = ofGetElapsedTimeMicros();
    animation.update(time);
    //animation1.update(time);
    //animation2.update(time);
    //animation3.update(time);
}


//--------------------------------------------------------------
void ofApp::draw(){
    animationManager.draw();
    animation.draw();
    //animation1.draw();
    //animation2.draw();
    //animation3.draw();
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    animationManager.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    animationManager.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
