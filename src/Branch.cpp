#include "Branch.h"


Branch::Branch(){
    level = 0;
    num = 0;
    strutFactor = 0;
}

void Branch::setup(int _lev, int _n, vector <ofVec2f> _points, float _strutFactor, int _maxlevels){
    level = _lev;
    num = _n;
    outerPoints = _points;
    strutFactor = _strutFactor;
    maxlevels = _maxlevels;
    midPoints = calcMidPoints();
    projPoints = calcStrutPoints();
    
    myBranches.clear();
    
    
    
    if ((level+1) < maxlevels) {
        
        
        Branch childBranch;
        childBranch.setup(level+1, 0, projPoints, strutFactor, maxlevels);
        myBranches.emplace_back(childBranch);
        
        
        for (int k = 0; k < outerPoints.size(); k++) {
            int nextk = k-1;
            if (nextk < 0) {nextk += outerPoints.size();}
            vector <ofVec2f> newPoints = { projPoints[k], midPoints[k], outerPoints[k], midPoints[nextk], projPoints[nextk]};
            childBranch.setup(level+1, k+1, newPoints, strutFactor, maxlevels);
            myBranches.emplace_back(childBranch);
        }
        

    }
    
             
}


void Branch::drawMe(int lineWidth){
    if(level == 0){
    ofSetLineWidth(6 - level);
        ofBeginShape();
        for (int i = 0; i < outerPoints.size(); i++) {
            ofFill();
            ofSetColor(255);
            ofVertex(outerPoints[i].x, outerPoints[i].y);
        }
        ofEndShape();
    } else {
        ofSetLineWidth(lineWidth - level);
    }
    
    for (int i = 0; i < outerPoints.size(); i++){
        int nexti = i+1;
        if (nexti == outerPoints.size()) { nexti = 0; }
        
        ofSetColor(0, 0, 0);
        ofDrawLine(outerPoints[i].x, outerPoints[i].y, outerPoints[nexti].x, outerPoints[nexti].y);
    }
    
    for (int k = 0; k < myBranches.size(); k++) {
        myBranches[k].drawMe(lineWidth);
    }
    
}

vector <ofVec2f> Branch::calcMidPoints(){
    vector <ofVec2f> mpArray;
    mpArray.resize(outerPoints.size());
    for (int i = 0; i<outerPoints.size(); i++) {
        int nexti = i+1;
        if (nexti == outerPoints.size()) {nexti = 0;}
        ofVec2f thisMP = calcMidPoint(outerPoints[i], outerPoints[nexti]);
        mpArray[i] = thisMP;
    }
    return mpArray;
}

ofVec2f Branch::calcMidPoint(ofVec2f end1, ofVec2f end2){
    float mx, my;
    if (end1.x > end2.x) {
        mx = end2.x + ((end1.x - end2.x)/2);
    } else {
        mx = end1.x + ((end2.x - end1 .x)/2);
    }
    if (end1.y > end2.y) {
        my = end2.y + ((end1.y - end2.y)/2);
    } else {
        my = end1.y + ((end2.y - end1.y)/2);
    }
    
    return ofVec2f(mx, my);
}


vector <ofVec2f> Branch::calcStrutPoints(){
    vector <ofVec2f> strutArray;
    strutArray.resize(midPoints.size());
    for (int i = 0; i < midPoints.size(); i++) {
        int nexti = i+3;
        if (nexti >= midPoints.size()) {nexti -= midPoints.size();}
        ofVec2f thisSP = calcProjPoint(midPoints[i], outerPoints[nexti]);
        strutArray[i] = thisSP;
    }
    return strutArray;
}

ofVec2f Branch::calcProjPoint(ofVec2f mp, ofVec2f op){
    float px, py;
    float adj, opp;
    if (op.x > mp.x) {
        opp = op.x - mp.x;
    } else {
        opp = mp.x - op.x;
    }
    if (op.y > mp.y) {
        adj = op.y - mp.y;
    } else {
        adj = mp.y - op.y;
    }
    if (op.x > mp.x) {
        px = mp.x + (opp * strutFactor);
    } else {
        px = mp.x - (opp * strutFactor);
    }
    if (op.y > mp.y) {
        py = mp.y + (adj * strutFactor);
    } else {
        py = mp.y - (adj * strutFactor);
    }
    
    return ofVec2f(px, py);
}

