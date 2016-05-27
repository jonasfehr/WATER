//
//  Kinect.hpp
//  Laser_KinectCalibrated
//
//  Created by Jonas Fehr on 12/05/16.
//
//

#include "ofxMultiKinectV2.h"
#include "GpuRegistration.h"

#include "ofxCv.h"


class Kinect{
public:
    void init();
    void update();
    void close();
    
    void drawColoredPC();
    
    // ForKinect
    ofxMultiKinectV2 kinectV2;
    GpuRegistration gpuRegistration;
    ofVboMesh pointCloud;
    
    ofVboMesh pointCloudClipped;

    
    ofShortImage irImage;
    ofPixels irPixels8;
    ofFloatPixels worldPixels;
    ofImage colorInDepth;
    
    ofVec4f getIRCamParams();
    

    
    ofPixels * getColorPixels();
    ofPixels * getIRPixels();
    ofShortPixels * getIRPixelsShort();
    ofFloatPixels * getDepthPixelsFloat();
    ofFloatPixels * getWorldPixels();
    ofTexture * getIRTexture();
    ofImage * getColorInDepthImage();
    
    ofTexture colorPreview;
    ofTexture depthPreview;

    ofTexture depthText;

    
    ofxCv::ContourFinder contourFinder;
    ofFloatImage depthImage;
    ofImage blobImage;
    

    
};