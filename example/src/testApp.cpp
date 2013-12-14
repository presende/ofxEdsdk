#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    gifEncoder.setup(720, 480, 0.1, 256);
    gifEncoder.setDitherMode(OFX_GIF_DITHER_NONE);
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &testApp::onGifSaved);
    
    setupArduino();
    
}


void testApp::setupCamera() {
    Eds::InitializeSDK();
    
	camera.setup(0);
	camera2.setup(1);
}

void testApp::setupArduino() {    
    
    arduino.connect("/dev/tty.usbmodem1a1211", 57600);
    ofAddListener(arduino.EInitialized, this, &testApp::onSetupArduino);
	bSetupArduino = false;
    
    //setupCamera();
    
}

void testApp::onSetupArduino(const int & version) {
    
	ofRemoveListener(arduino.EInitialized, this, &testApp::onSetupArduino);
    
    bSetupArduino = true;
    
    arduino.sendDigitalPinMode(2, ARD_INPUT);
	arduino.sendDigitalPinMode(3, ARD_OUTPUT);
	arduino.sendDigitalPinMode(5, ARD_OUTPUT);
	arduino.sendDigitalPinMode(6, ARD_OUTPUT);
    
    arduino.sendDigital(3,ARD_LOW);
    arduino.sendDigital(5,ARD_LOW);
    arduino.sendDigital(6,ARD_LOW);
	
    ofAddListener(arduino.EDigitalPinChanged, this, &testApp::digitalPinChanged);
    
}

void testApp::digitalPinChanged(const int & pinNum) {
    ofLogVerbose() << "digital pin: " << ofToString(pinNum) << " = " + ofToString(arduino.getDigital(pinNum));
}

void testApp::update() {
    
    if(!isTakingPhoto && bSetupArduino) {
        arduino.update();
    }
    
	if(camera.isFrameNew()) {
		// process the live view with camera.getLivePixels()
	}
	if(camera.isPhotoNew()) {
		
        ofImage image1;
        image1.setImageType(OF_IMAGE_COLOR);
        image1.setFromPixels(camera.getPhotoPixels());
        image1.width = 720;
        image1.height = 480;
        
        gifEncoder.addFrame(image1);
        
		camera.savePhoto("data/" + ofToString(ofGetFrameNum()) + ".jpg");
	}
    if(camera2.isPhotoNew()) {
        
		ofImage image2;
        image2.setImageType(OF_IMAGE_COLOR);
        image2.setFromPixels(camera2.getPhotoPixels());
        image2.width = 720;
        image2.height = 480;
        
        gifEncoder.addFrame(image2);
        
		camera2.savePhoto("data/" + ofToString(ofGetFrameNum()) + "_2.jpg");
	}
    
    if(gifEncoder.getTotalFrames()==2 && !isSavingGif) {
        saveGif();
    }
    
}

void testApp::draw() {
    
    if(!isTakingPhoto) {
        if(debug) {
            if(print==0) {
                camera.update();
                camera.draw(0,0,720,480);
            } else {
                camera2.update();
                camera2.draw(0,0,720,480);
            }
            
            if(counter<2) {
                
                counter += 1;
                
            } else {
                counter = 0;
                if(print==0) {
                    print=1;
                } else {
                    print=0;
                }
            }
        } else {
            camera.update();
            camera.draw(0,0,720,480);
        }
    }    
    
    
}


void testApp::keyPressed(int key) {
	if(key) {
        
        shoot();
        
	}
}

void testApp::shoot() {
    if(!isTakingPhoto) {
        
        //isTakingPhoto = true;
        
		//camera.takePhoto();
        //camera2.takePhoto();
        
        int color = ofRandom(0,2);
        
        if(color==0) {
            arduino.sendDigital(3,ARD_HIGH);
            arduino.sendDigital(5,ARD_LOW);
            arduino.sendDigital(6,ARD_LOW);
        } else if(color==1) {
            arduino.sendDigital(3,ARD_LOW);
            arduino.sendDigital(5,ARD_HIGH);
            arduino.sendDigital(6,ARD_LOW);
        } else if(color==2) {
            arduino.sendDigital(3,ARD_LOW);
            arduino.sendDigital(5,ARD_LOW);
            arduino.sendDigital(6,ARD_HIGH);
        }8
        
	}
}

void testApp::saveGif() {
    
    isSavingGif = true;
    
    gifEncoder.save("test.gif");
}

void testApp::onGifSaved(string &fileName) {
    isSavingGif = false;
    isTakingPhoto = false;
    gifEncoder.reset();
}
