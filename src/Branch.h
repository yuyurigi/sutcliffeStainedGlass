#ifndef Branch_h
#define Branch_h

#include "ofMain.h"

class Branch{
public:
    Branch();
    void setup(int lev, int n, vector <ofVec2f> points, float _strutFactor, int _maxlevels);
    void drawMe(int lineWidth);

    vector<ofVec2f> calcMidPoints();
    ofVec2f calcMidPoint(ofVec2f end1, ofVec2f end2);
    
    vector <ofVec2f> calcStrutPoints();
    ofVec2f calcProjPoint(ofVec2f mp, ofVec2f op);
    
public:
    int level, num;
    float strutFactor;
    int maxlevels;
    int circleResolution;
    
    vector <ofVec2f> outerPoints;
    
    vector <ofVec2f> midPoints;
    vector <ofVec2f> projPoints;
    vector <Branch> myBranches;
    
};


#endif /* Branch_h */
