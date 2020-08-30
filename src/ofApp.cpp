#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundAuto(true);
    ofBackground(0, 0, 0);
    
    ofEnableSmoothing();

    setColor();
    
    //set GUI
    gui.setup();
    gui.add(strutFactor.setup("strutFactor", 0.8652208, -1.0, 2.0)); //支柱の長さ
    gui.add(circleResolution.setup("circle res", 5, 3, 9)); //頂点数
    gui.add(maxlevels.setup("max level", 3, 2, 4)); //繰り返し数
    gui.add(lineWidth.setup("lineWidth", 6, 5, 10)); //ラインの太さ
    gui.add(startAngle.setup("angle", -18, -360, 360)); //角度
    
    bHide = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){


}

//--------------------------------------------------------------
void ofApp::draw(){
    drawFbo();
    
    pentagon.setup(startAngle, circleResolution, strutFactor, maxlevels);
    pentagon.drawShape(lineWidth);
    
    myImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    
    drawMesh();
    
    myImage2.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    
    if(!bHide){
        gui.draw();
    }
}
//--------------------------------------------------------------
void ofApp::setColor(){    
    colors.push_back(ofColor(255, ofRandom(193, 227), 208)); //0 //pink
    colors.push_back(ofColor(255, colors[0].g + ofRandom(-40, -20), 208)); // 1
    colors.push_back(ofColor(243, 243, 243)); //2 //white
    colors.push_back(ofColor(243, 243, 243)); //3
    colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //4 //yellow
    colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //5
    colors.push_back(ofColor(200, 230, 244)); //6 //blue
    colors.push_back(ofColor(ofRandom(120, 175), 208, 236)); //7
    colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //8 //orange
    colors.push_back(ofColor(255, colors[8].g, 50 + ofRandom(50, 120))); //9
     
    
    for (float i = ofGetWidth()/2; i >= 0; i-=20) {
        int cr = (int)ofRandom(colors.size()/2);
        glassColors.push_back(colors[cr*2]);
        glassColors.push_back(colors[cr*2+1]);
    }
}
//--------------------------------------------------------------
void ofApp::drawFbo(){
    int cut = 20;
    int degree = 360/cut;
    int cc = 1;
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    fbo.begin();
    ofSetColor(255, 255, 255);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255);
    ofDrawEllipse(ofGetWidth()/2, ofGetHeight()/2, 20, 20);
    
    for (float i = ofGetWidth()/2; i >= 0; i-=20) {
        float tmp_x = 0;
        float tmp_y = 0;
        
        ofSetLineWidth(22);
        for (int deg = 0; deg <= 360; deg++) {
            float x = ofGetWidth()/2 + (cos(deg * DEG_TO_RAD) * i);
            float y = ofGetHeight()/2 + (sin(deg * DEG_TO_RAD) * i);
            int xxx = (deg/degree)%2;
            if (tmp_x != 0 && tmp_y != 0) {
            if (xxx == 0) {
                ofSetColor(glassColors[cc]);
                ofDrawLine(x, y, tmp_x, tmp_y);
            } else {
                ofSetColor(glassColors[cc+1]);
                ofDrawLine(x, y, tmp_x, tmp_y);
            }
            }
            tmp_x = x;
            tmp_y = y;
        }
        cc += 2;
    }
    fbo.end();
    
}
//--------------------------------------------------------------
void ofApp::drawMesh(){
    myImage.setImageType(OF_IMAGE_COLOR); //PNGからアルファチャンネルを取り除く
    colorImage.setFromPixels(myImage.getPixels()); //put the pixels into the image format openCV understands
    
    grayImage = colorImage; // convert the image to grayscale using the handy overloaded operator
    
    thresholdImage = grayImage; // make a copy for thresholding
    
    int totalArea = thresholdImage.width*thresholdImage.height;
    int minArea = totalArea * 0.00001;
    int maxArea = totalArea * 0.75;
    int nConsidered = 200;
    
    contourFinder.findContours(grayImage, minArea, maxArea, nConsidered, true);
    
    
    for (int i = 0; i < contourFinder.nBlobs; i++) {
        ofPolyline poly;
        for (int j = 0; j < contourFinder.blobs[i].pts.size(); j ++) {
            
            ofPoint pos = contourFinder.blobs[i].pts[j];
            poly.addVertex(pos);
            
        }
        poly.close();
        
        
        ofMesh mesh;
        ofTessellator tess;
        tess.tessellateToMesh(poly, ofPolyWindingMode::OF_POLY_WINDING_ODD, mesh, true);
        ofPoint pos2 = poly.getCentroid2D();
        ofColor myc = readBackground(pos2.x, pos2.y);
        ofFill();
        ofSetColor(myc);
        mesh.draw();
        
    }
}
//--------------------------------------------------------------
ofColor ofApp::readBackground(float x, float y){
    
    ofPixels pixels2;
    fbo.readToPixels(pixels2);
    ofColor c = pixels2.getColor(x, y);
    return c;
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == 'h' || key == 'H') {
        bHide = !bHide;
    }
    if (key == 'S' || key == 's') {
        myImage2.save(ofGetTimestampString("%Y%m%d%H%M%S")+"##.png");
    }
    if (key == 'c' || key == 'C'){ //change color
        glassColors.clear();
        glassColors.push_back(colors[ofRandom(colors.size())]);
        
        for (float i = ofGetWidth()/2; i >= 0; i-=20) {
            int cr = (int)ofRandom(colors.size()/2);
            glassColors.push_back(colors[cr*2]);
            glassColors.push_back(colors[cr*2+1]);
        }
    }
    
    //配色変える　(1 ~ 5)
    if (key == '1') {
        colors.clear();
        colors.push_back(ofColor(255, ofRandom(193, 227), 208)); //0 //pink
        colors.push_back(ofColor(255, colors[0].g + ofRandom(-40, -20), 208)); // 1
        colors.push_back(ofColor(243, 243, 243)); //2 //white
        colors.push_back(ofColor(243, 243, 243)); //3
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //4 //yellow
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //5
        colors.push_back(ofColor(200, 230, 244)); //6 //blue
        colors.push_back(ofColor(ofRandom(120, 175), 208, 236)); //7
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //8 //orange
        colors.push_back(ofColor(255, colors[8].g, 50 + ofRandom(50, 120))); //9
        
        glassColors.clear();
        glassColors.push_back(colors[ofRandom(colors.size())]);
        
        for (float i = ofGetWidth()/2; i >= 0; i-=20) {
            int cr = (int)ofRandom(colors.size()/2);
            glassColors.push_back(colors[cr*2]);
            glassColors.push_back(colors[cr*2+1]);
        }
        
    }
    if (key == '2') {
        colors.clear();
        colors.push_back(ofColor(255, ofRandom(193, 227), 208, ofRandom(200, 250))); // 0 //pink
        colors.push_back(ofColor(255, colors[0].g + ofRandom(-40, -20), 208, ofRandom(200, 250))); // 1
        colors.push_back(ofColor(243, 243, 243)); //2 //white
        colors.push_back(ofColor(243, 243, 243, ofRandom(0, 200))); //3
        colors.push_back(ofColor(255, ofRandom(180, 208), 50, ofRandom(200, 250))); //4 //yellow
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //5
        colors.push_back(ofColor(ofRandom(150,200), 230, 244)); //6 //blue
        colors.push_back(ofColor(ofRandom(0, 175), 208, 236, ofRandom(200, 250))); //7
        colors.push_back(ofColor(ofRandom(140, 200), ofRandom(200, 230), 244)); //8 //orange
        colors.push_back(ofColor(ofRandom(120, 175), 208, 236)); //9
        colors.push_back(ofColor(0, ofRandom(150, 220), 244, ofRandom(200, 255))); //10 //orange
        colors.push_back(ofColor(4, 77, 117, ofRandom(200, 255)));
        
        glassColors.clear();
        glassColors.push_back(colors[ofRandom(colors.size())]);
        
        for (float i = ofGetWidth()/2; i >= 0; i-=20) {
            int cr = (int)ofRandom(colors.size()/2);
            glassColors.push_back(colors[cr*2]);
            glassColors.push_back(colors[cr*2+1]);
        }
    }
    
    if (key == '3') {
        colors.clear();
        colors.push_back(ofColor(ofRandom(170,220), 29, 35)); // 0 //red
        colors.push_back(ofColor(ofRandom(110, 170), 20, 24)); // 1
        colors.push_back(ofColor(243, 243, 243)); //2 //white
        colors.push_back(ofColor(243, 243, 243)); //3
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //4 //yellow
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //5
        colors.push_back(ofColor(5, 154, 176)); //6 //blue
        colors.push_back(ofColor(4, 77, 117)); //7
        colors.push_back(ofColor(243, 243, 243)); //8 //white
        colors.push_back(ofColor(243, 243, 243)); //9
        
        glassColors.clear();
        glassColors.push_back(colors[ofRandom(colors.size())]);
        
        for (float i = ofGetWidth()/2; i >= 0; i-=20) {
            int cr = (int)ofRandom(colors.size()/2);
            glassColors.push_back(colors[cr*2]);
            glassColors.push_back(colors[cr*2+1]);
        }
    }
    
    if (key == '4') {
        colors.clear();
        colors.push_back(ofColor(255, 96, 50)); // 0 //orange
        colors.push_back(ofColor(255, 132, 50)); // 1
        colors.push_back(ofColor(243, 243, 243)); //2 //white
        colors.push_back(ofColor(243, 243, 243)); //3
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //4 //yellow
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //5
        colors.push_back(ofColor(5, 154, 176)); //6 //blue
        colors.push_back(ofColor(4, 77, 117)); //7
        colors.push_back(ofColor(243, 243, 243)); //8 //white
        colors.push_back(ofColor(243, 243, 243)); //9
        
        glassColors.clear();
        glassColors.push_back(colors[ofRandom(colors.size())]);
        
        for (float i = ofGetWidth()/2; i >= 0; i-=20) {
            int cr = (int)ofRandom(colors.size()/2);
            glassColors.push_back(colors[cr*2]);
            glassColors.push_back(colors[cr*2+1]);
        }
    }
    
    if (key == '5') {
        colors.clear();
        colors.push_back(ofColor(255, ofRandom(193, 227), 208)); //0 //pink
        colors.push_back(ofColor(255, ofRandom(193, 227)+ofRandom(-40, -20), 208)); // 1
        colors.push_back(ofColor(243, 243, 243)); //2 //white
        colors.push_back(ofColor(243, 243, 243)); //3
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //4 //yellow
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //5
        colors.push_back(ofColor(200, 230, 244)); //6 //blue
        colors.push_back(ofColor(ofRandom(120, 175), 208, 236)); //7
        colors.push_back(ofColor(255, ofRandom(176, 238), 50)); //8 //orange
        colors.push_back(ofColor(255, ofRandom(276, 238), 50+ofRandom(50, 120))); //9
        
        glassColors.clear();
        
        for (float i = ofGetWidth()/2; i >= 0; i-=20) {
            int cr = (int)ofRandom(colors.size()/2);
            glassColors.push_back(colors[cr*2]);
            glassColors.push_back(colors[cr*2+1]);
        }
    }

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
