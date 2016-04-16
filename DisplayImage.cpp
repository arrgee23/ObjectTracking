#include "opencv2/opencv.hpp"
#include<iostream>
#include<cstdio>
#include "Object.cpp"

using namespace cv;
using namespace std;

// being passed a contour and objectlist returns the index of the object in ObjectList if found
// else returns -1
int ifExistsInObjectList(vector<Point> c, vector<Object> objectList){
	
	for(int i=0;i<objectList.size();i++)
		if(objectList[i].isSame(c))
			return i;
	
	return -1;
}
int main(int, char**)
{

	VideoCapture cap("Video/Simulated.mp4"); // open the default camera

    if(!cap.isOpened())  // check if we succeeded
        return -1;
	double fps = cap.get(CV_CAP_PROP_FPS);
	cout<<fps<<endl;
	
	vector<Object> objectList;
    for(;;)
    {
        Mat frame;
        Mat dest;

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        Rect bounding_rect;
        Mat dst,bin;

        if(!cap.read(frame)) // get a new frame from camera
        	break;


        cvtColor(frame, dest, CV_BGR2GRAY);  // converts image from rgb(src) to gray level (dst)


        adaptiveThreshold(dest, bin,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV,75,20);
       	//threshold(dest, bin, 40, 255, CV_THRESH_BINARY_INV); // // Tresholds image with level = 40 from gray level(dst) to binary (bin)

        imshow("threshold",bin);
       	findContours(bin,contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE ); // finds contours on bin image
       	Scalar color( 255,255,255 );

       	for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
		{
       	    if((contourArea(contours[i],false))>10)
            { // if counter area >100 pixel draw it on ero which is new image variable
       	          drawContours( bin, contours, i , color, CV_FILLED, 8, hierarchy ); //Draw contours on itself as filled
            }
        }

       
        
        /*
        
          for( int i = 0; i < contours.size(); i++ ){
              mu[i] = moments( contours[i], false );
          }

        //Mass center
       
          for( int i = 0; i < contours.size(); i++ ){ 
            mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
          }
          */
          
        /*
         * Main calculation
         */
		findContours( bin, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
		vector<Point2f> mc( contours.size() );
		vector<Moments> mu(contours.size() );
		for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
		{
			
			if((contourArea(contours[i],false))>20)
            {  
				// the bounding rect lies within frame (the complete object is in frame)
				bounding_rect=boundingRect(contours[i]);
				Point tl = bounding_rect.tl(); // topleft and bottom right
				Point br = bounding_rect.br();
				//cout<<tl<<endl;
				//cout<<br<<endl;
				Size s = bin.size();
				//cout<<"sizze "<<s<<endl;
				if(tl.x > 3 && tl.y > 3 && br.x< s.width-3 && br.y< s.height-3){ // make sure the bounding rectangle is within window
					//mu[i] = moments( contours[i], false );
					//mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
					
					if(ifExistsInObjectList(contours[i],objectList)== -1) // doesnt exist
						objectList.push_back(Object(contours[i]));
					else{
						
					}
					cout<<objectList.size()<<endl;
					//Bound and Draw rectangle each object which detected at the end on src(original image)
					// draw cm
					circle(frame,mc[i], 1, Scalar(0,0,255), 1, 8, 0);
					rectangle(frame, bounding_rect,  Scalar(0,255,0),3, 8,0);
				}
            }
       	     
       	}



       	   imshow("original",frame);
       	   //cout<<contours.size()<<"\t";

           waitKey(1000/fps);
        //if(waitKey(500) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor

    return 0;
}
