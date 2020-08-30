
#include "FractalRoot.h"

FractalRoot::FractalRoot(){
    
}

void FractalRoot::setup(float startAngle, int _numSides, float _strutFactor, int maxlevels){
    pointArr.clear();
    
    float centX = ofGetWidth()/2;
    float centY = ofGetHeight()/2;
    int numSides = _numSides;
    float angleStep = 360.0f/numSides;
    strutFactor = _strutFactor;
     for(int i = 0; i < 360; i+=angleStep){
        float x = centX + (380 * cos(ofDegToRad(startAngle + i)));
        float y = centY + (380 * sin(ofDegToRad(startAngle + i)));
        ofVec2f pp (x, y);
        pointArr.push_back(pp);
    }
    rootBranch.setup(0, 0, pointArr, strutFactor, maxlevels);
}

void FractalRoot::drawShape(int lineWidth){
    rootBranch.drawMe(lineWidth);
}
