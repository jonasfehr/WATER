#pragma once

#include "ofMain.h"

#include "ofxRay.h"
#include "ofxGrabCam.h"

#include "ofxMultiKinectV2.h"

#include "ofxRayComposer.h"
#include "ofxIldaFrame.h"
#include "waveSystem.h"

#include "ofxAutoReloadedShader.h"


#define RES_CIRCLE 50
#define DAMPING 0.98
#define ATTACK  0.02
#define RADIUS 0.6


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    
    ofxRay::Camera laser;
    
    ofxMultiKinectV2 kinectV2;

    // Laser
    ofxIlda::Frame ildaFrame;
    ofxRayComposer ildaDAC;
        
    // WaterSystem
    WaveParticleSystem wave;
    
    struct PointIldaSpace{
        ofVec2f position;
        bool isInside = false;
    };
    
    vector<PointIldaSpace> pointsIldaSpace;

    
};
