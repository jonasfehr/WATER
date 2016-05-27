#include "ofApp.h"





//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(100, 100, 100);
    ofEnableSmoothing();
    
    ofFile data;
    if(data.doesFileExist("lasercalibration.txt")){
        ifstream filein(ofToDataPath("lasercalibration.txt"), ios::binary | ios::in);
        filein >> this->laser;
        filein.close();
    }else{
        auto result = ofSystemLoadDialog("Select ofxRay::Camera file for this projector");
        if (!result.bSuccess) {
            ofExit();
        }
        ifstream filein(ofToDataPath(result.filePath), ios::binary | ios::in);
        filein >> this->laser;
        filein.close();
    }
    
    
    ofSetBackgroundColor(0);
    
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    // Initialising Laser DAC
    ildaDAC.setup();
    ildaDAC.setPPS(20000);
    
    // wave simulation
    wave.setup(RES_CIRCLE);
    wave.updateResponse(ATTACK, DAMPING);
    
    // set laser
    ildaFrame.params.output.endCount = 0;
    ildaFrame.params.output.blankCount = 0;
    ildaFrame.polyProcessor.params.targetPointCount = 200;
    
    
    kinectV2.open(true, true, 0, 2);
    kinectV2.start();
}

//--------------------------------------------------------------
void ofApp::update(){
    wave.update();
    
    this->kinectV2.update();
    if(this->kinectV2.isFrameNew()) {
        
        
        // far near cut
        float near = 0;
        float far = 1900;
        
        pointsIldaSpace.clear();
        const auto depthWidth = kinectV2.getDepthPixelsRef().getWidth();
        const auto depthHeight = kinectV2.getDepthPixelsRef().getHeight();
#pragma omp parallel for
        for(int j=0; j<depthHeight; j++) {
            const auto pixelOffset = depthWidth * j;
            for(int i=0; i<depthWidth; i++) {
                float pixValue = kinectV2.getDepthPixelsRef()[i + pixelOffset];
                //                cout << pixValue << endl;
                if(pixValue > near && pixValue < far){ // clip near far from depthImage
                    ofVec3f worldPos = ofVec3f(this->kinectV2.getWorldCoordinateAt(i, j, pixValue/ 1000.0f));
                    
                    PointIldaSpace pIldaSpace;
                    pIldaSpace.position = laser.getNormalizedSCoordinateOfWorldPosition(worldPos);
                    if(pIldaSpace.position.x < 1 && pIldaSpace.position.x > -1  &&  pIldaSpace.position.y < 1 && pIldaSpace.position.y > -1){ //clip if outside ildaSpace
                        pointsIldaSpace.push_back(pIldaSpace);
                    }
                }
                
            }
        }
        
        
        
        
        
        float maxW = 0;
        for(auto & wp : wave.waveParticles){
            if(wp.p+RADIUS>maxW) maxW = wp.p+RADIUS;
        }
        
        for( auto & p : pointsIldaSpace){
            if( maxW>abs(p.position.x) || maxW > abs(p.position.y)){ // far away sort out
                float maxW2 = maxW*maxW;
                float dist2 = ofDistSquared(0, 0, p.position.x, p.position.y);
                if(maxW2 > dist2){ // is potentially inside circle
                    
                    float angle = ofVec2f(0., -1.).angleRad(ofVec2f(p.position.x, -p.position.y));
                    angle += PI;
                    int index = ceil(angle/TWO_PI * RES_CIRCLE);
                    float pW = wave.waveParticles[index].p+RADIUS;
                    
                    float distW2 = pW*pW;
                    if(distW2>dist2){ // is inside
                        float replace = sqrt(distW2-dist2);
                        if(replace < 0.2){
                            wave.waveParticles[index].p -= replace; // just do it in a certain range.
                            p.isInside = true;
                        }
                    }
                }
            }
        }
    }
    
    
    // Create Laser ouptut
    ofPolyline circleWaterwave;
    rayCoords.clear();
    ofPoint center = ofPoint(0.5,0.5);
    float segAng = TWO_PI/RES_CIRCLE;
    for(int i = 0; i < RES_CIRCLE; i++){
        float w = RADIUS/2*(wave.waveParticles[i].p+1);
        if(w > 0.5) w = 0.5;         // clip
        float x = sin(segAng*i)*w;
        float y = cos(segAng*i)*w;
        x += center.x;
        y += center.y;
        circleWaterwave.addVertex(x, y);
    }
    circleWaterwave.close();
    
    // do your thang
    ildaFrame.clear();
    ildaFrame.addPoly(circleWaterwave, ofFloatColor::aquamarine);
    ildaFrame.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    // draw ILDA FRAME
    ofPushMatrix();
    // DRAW ILDA
    ofTranslate(0,0);
    ofSetColor(0);
    ofFill();
    ofDrawRectangle(0, 0, 512, 512);
    ofSetColor(255);
    ildaFrame.draw(0, 0, 512, 512);
    for( auto & p : pointsIldaSpace){
        ofSetColor(255,0,0,255);
        if(p.isInside) ofSetColor(0,0,255,255);
        ofDrawCircle(ofMap(p.position.x, 1., -1. , 0, 512), ofMap(p.position.y, 1., -1. , 0, 512), 1);
    }
    ofPopMatrix();
    
    
    
    // send points to the etherdream
    ildaDAC.setPoints(ildaFrame);
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    this->kinectV2.close();
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
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
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
