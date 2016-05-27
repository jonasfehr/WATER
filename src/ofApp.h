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
    
    
    ofxRay::Camera laser;
        
    vector<ofVec2f> rayCoords;
    vector<ofRay> rays;
    
    ofxMultiKinectV2 kinectV2;

    // Laser
    ofxIlda::Frame ildaFrame;
    ofxRayComposer ildaDAC;
    
    bool showInfoILDA = true;
    
    // WaterSystem
    WaveParticleSystem wave;
    
    struct PointIldaSpace{
        ofVec2f position;
        bool isInside = false;
    };
    
    vector<PointIldaSpace> pointsIldaSpace;

    
};
