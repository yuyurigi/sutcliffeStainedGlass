#pragma once

#include "ofMain.h"
#include "FractalRoot.h"
#include "Branch.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"

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
    
    void setColor();
    void drawFbo();
    void drawMesh();
    ofColor readBackground(float x, float y);
    
    FractalRoot pentagon;
    
    ofImage myImage;
    ofImage myImage2;
    
    ofxCvColorImage colorImage;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage thresholdImage;
    ofxCvContourFinder contourFinder;
    
    vector<ofColor> colors;
    vector<ofColor> glassColors;
    ofFbo fbo;

    // parameter of GUI
       ofxPanel gui;
       ofxFloatSlider strutFactor;
       ofxIntSlider circleResolution;
       ofxIntSlider maxlevels;
       ofxIntSlider lineWidth;
       ofxIntSlider startAngle;
       bool bHide;
    };
