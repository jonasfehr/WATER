//
//  Kinect.cpp
//  Laser_KinectCalibrated
//
//  Created by Jonas Fehr on 12/05/16.
//
//

#include "Kinect.h"
using namespace ofxCv;
using namespace cv;


void Kinect::init(){
    kinectV2.open(true, true, 0, 2);
    kinectV2.start();
				
    this->gpuRegistration.setup(this->kinectV2.getProtonect(), 1.0f);
    
    this->pointCloud.setUsage(GL_DYNAMIC_DRAW);
    this->pointCloud.setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);
    
    this->pointCloudClipped.setUsage(GL_DYNAMIC_DRAW);
    this->pointCloudClipped.setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);
    
    depthImage.setUseTexture(false);
    
}
void Kinect::update(){
    this->kinectV2.update();
    if(this->kinectV2.isFrameNew()) {
        
        // Create clipped PC;
        depthImage.setFromPixels(kinectV2.getDepthPixelsRef());
        depthImage.update();


        // far near cut
        float near = 50;
        float far = 1000;
//        threshold(toCv(depthImage), toCv(depthImage), far, 1, CV_THRESH_TOZERO); // farcut
//        threshold(toCv(depthImage), toCv(depthImage), near, 1, CV_THRESH_TOZERO_INV); // nearCut
        
//        for(int x = 0; x < depthImage.getWidth(); x++){
//            for(int y = 0; y < depthImage.getWidth(); y++){
//                if(depthImage.getPixels()[i]>0.){
//                    pointCloudClipped.addVertex(this->kinectV2.getWorldCoordinateAt(i, j, depthImage.getPixels()[i]) / 1000.0f;)
//                }
//            }
//        }

        pointCloudClipped.clear();
        const auto depthWidth = depthImage.getWidth();
        const auto depthHeight = depthImage.getHeight();
#pragma omp parallel for
        for(int j=0; j<depthHeight; j++) {
            const auto pixelOffset = depthWidth * j;
            for(int i=0; i<depthWidth; i++) {
                float pixValue = depthImage.getPixels()[i + pixelOffset];
//                cout << pixValue << endl;
                if(pixValue > near && pixValue < far){
                    pointCloudClipped.addVertex(this->kinectV2.getWorldCoordinateAt(i, j, depthImage.getPixels()[i + pixelOffset]/ 1000.0f));
                }

            }
        }

        
//        ofxCv::copy(depthImage, blobImage); // convert to char
//        
//        blobImage.update();
        
//        contourFinder.setThreshold(blobThreshold);
//        contourFinder.findContours(blobImage);
//        
//        blobXy.clear();
//        blobXyz.clear();
//        blobXyzProjected.clear();
//        blobProjected.clear();
//        
//        if(contourFinder.size() > 0){
//            
//            for(int i=0;i<contourFinder.size();i++){
//                double min;
//                double max;
//                cv::Point minLoc, maxLoc;
//                
//                // Get contour
//                vector< vector<cv::Point> > maskContour;
//                maskContour.push_back(contourFinder.getContour(i));
//                
//                // Draw as mask
//                cv::Mat mask = cv::Mat::zeros(depthImg.getHeight(), depthImg.getWidth(), CV_8UC1);
//                cv::drawContours(mask, maskContour, -1, cvScalar(255), CV_FILLED);
//                
//                // Calculate max position in blob
//                cv::minMaxLoc(ofxCv::toCv(blobImg), &min, &max, &minLoc, &maxLoc, mask);
//                
//                // Store location
//                ofVec2f p = ofxCv::toOf(maxLoc);
//                ofVec3f worldp = kinect.getWorldCoordinateAt(p.x, p.y);
//                
//                if(worldp.z > 100){
//                    
//                    blobXy.push_back(p);
//                    blobXyz.push_back(worldp);
//                    
//                    // Project to floor in 3d
//                    ofVec3f pp = worldp - corner[0].get();
//                    float d = pp.dot(up);
//                    ofVec3f projected = worldp - d * up;
//                    
//                    blobXyzProjected.push_back(projected);
//                    
//                    // Project to floor in 2d
//                    ofVec3f projectedOffset = projected - corner[0].get();
//                    float x = projectedOffset.dot(d1.getNormalized()) / d1.length();
//                    float y = projectedOffset.dot(d2.getNormalized()) / d2.length();
//                    
//                    blobProjected.push_back(ofVec2f(x,y));
//                }
//            }
//        }
        

        
        
    }

}
void Kinect::close(){
    this->kinectV2.close();

}

void Kinect::drawColoredPC(){
    colorInDepth.bind();
    {
        glPushAttrib(GL_POINT_BIT);
        {
            glPointSize(3.0f);
            glEnable(GL_POINT_SMOOTH);
            pointCloud.draw();
        }
        glPopAttrib();
    }
    colorInDepth.unbind();
}


ofPixels * Kinect::getColorPixels() {
				return & this->kinectV2.getColorPixelsRef();
}

//----------
ofPixels * Kinect::getIRPixels() {
				this->irPixels8.allocate(this->irImage.getWidth(), this->irImage.getHeight(), 1);
				const auto irImageSize = this->irPixels8.size();
				auto irPixels = this->irPixels8.getData();
				auto irPixelsShort = this->irImage.getPixels().getData();
				for(int i=0; i<irImageSize; i++) {
                    irPixels[i] = irPixelsShort[i] >> 8;
                }
				return & this->irPixels8;
}
//----------
ofShortPixels * Kinect::getIRPixelsShort() {
				return & this->irImage.getPixels();
}

//----------
ofFloatPixels * Kinect::getDepthPixelsFloat() {
				return & this->kinectV2.getDepthPixelsRef();
}

//----------
ofFloatPixels * Kinect::getWorldPixels() {
				return & this->worldPixels;
}

//----------
ofTexture * Kinect::getIRTexture() {
				return & this->irImage.getTexture();
}

//----------
ofImage * Kinect::getColorInDepthImage() {
				return & this->colorInDepth;
}

//----------
ofVec4f Kinect::getIRCamParams(){
    libfreenect2::Freenect2Device::IrCameraParams p;
    ofVec4f params;
    if (kinectV2.getProtonect()) {
        p = kinectV2.getProtonect()->getIrCameraParams();
        params = ofVec4f(p.cx, p.cy, p.fx, p.fy);
    }
    return params;
}




