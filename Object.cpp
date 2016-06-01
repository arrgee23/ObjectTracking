#include "opencv2/opencv.hpp"
#include<iostream>
#include<cstdio>
#include<cmath>

using namespace cv;
using namespace std;

class Object{
	public:
	
	vector<Point> contour;
	double area;
	Point2f cm;
	vector<Point2f> cmHistory;
	
	// initialize object
	Object(vector<Point> c) {
		contour = c;
		area = contourArea(c,false);
		Moments mu = moments(c, false );
		cm = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
		cmHistory.push_back(cm);
	}
	
	void updateContour(vector<Point> c){
		contour = c;
		area = contourArea(c,false);
		Moments mu = moments(c, false );
		cm = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
		cmHistory.push_back(cm);
	}
	
	// compares this object to the passed object
	bool isSame(vector<Point> c){
		//double a = contourArea(c,false);
		//cout<<matchShapes(this->contour,c,1,0.0)<<endl;
		//if(abs(this->area - a) < (this->area*8)/10 && matchShapes(this->contour,c,CV_CONTOURS_MATCH_I1,0)>15)
		Moments mu = moments(c, false );
		Point2f cm2 = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
		float x1 = cm.x;
		float x2 = cm2.x;
		float y1 = cm.y;
		float y2 = cm2.y;
		float dist = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
		if(matchShapes(this->contour,c,CV_CONTOURS_MATCH_I1,0)<2 && dist < 2000)
			return true;
		
		return false;
			
	}
	
	void addCM(	vector<Point> c){
		Moments mu = moments(c, false );
		cm = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
		cmHistory.push_back(cm);
	}
	
	bool isMovingDown(){
		if(cmHistory.size()<10)
			return false;
		else{
			// compare this frame and 10 frames before
			if(cmHistory[cmHistory.size()-1].y - cmHistory[cmHistory.size()-1-10].y){
			
			}
		}
	}
	
	void drawPath(Mat frame, Scalar color){
		for(int i=0;i<cmHistory.size()-1;i++){
			line(frame,cmHistory[i],cmHistory[i+1],color,1,8,0);
		}
	}	
}
;
