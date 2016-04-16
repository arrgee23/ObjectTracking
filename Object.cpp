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
	
	bool isSame(vector<Point> c){
		double a = contourArea(c,false);
		cout<<matchShapes(this->contour,c,1,0.0)<<endl;
		//if(abs(this->area - a) < (this->area*8)/10 && matchShapes(this->contour,c,CV_CONTOURS_MATCH_I1,0)>15)
		if(matchShapes(this->contour,c,CV_CONTOURS_MATCH_I1,0)<2)
			return true;
		
		return false;
			
	}
	
	void addCM(	Point2f cm){
		cmHistory.push_back(cm);
	}
}
;
