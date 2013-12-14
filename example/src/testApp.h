#pragma once

#include "ofMain.h"

#include "ofxEdsdk.h"
#include "ofxGifEncoder.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw(); n
	void keyPressed(int key);
    
    //arduino
    void setupArduino();
    void onSetupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    
    bool bSetupArduino;
    ofArduino arduino;
    
    //gif
    void setupCamera();
    int print = 0;
    int counter = 0;
	
    
    //camera
	ofxEdsdk::Camera camera;
	ofxEdsdk::Camera camera2;
    
    ofxGifEncoder gifEncoder;
    
    bool debug = false;
    bool isTakingPhoto = false;
    bool isSavingGif = false;
    
    void saveGif();
    void shoot();
    
    void onGifSaved(string & fileName);
    
};
