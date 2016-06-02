#include "opencv2/opencv.hpp"
#include<iostream>
#include<cstdio>
#include "Object.cpp"

using namespace cv;
using namespace std;

// being passed a contour and objectlist returns the index of the object in ObjectList if found
// else returns -1

vector<Object> objectList;
Mat frame;
vector<int> aeroplanes; // aeroplane indeces in objectList


bool droppedFromPlane(Object o){
	return true;
}

 
int ifExistsInObjectList(vector<Point> c, vector<Object> objectList){
	
	for(int i=0;i<objectList.size();i++)
		if(objectList[i].isSame(c))
			return i;
	
	return -1;
}

Point pt(-1,-1);
bool newCoords = false;

void help()
{
    cout
        << "--------------------------------------------------------------------------"  << endl
        << "Usage:"                                                                      << endl
        << "./DisplayImage {<video filename with path>}"                          << endl
        << "for example: ./DisplayImage Video/circle.mp4"                                            << endl
        << "--------------------------------------------------------------------------"  << endl
        << endl;
}

int main(int argc, char* argv[])
{
	
    
    //check for the input parameter correctness
    if(argc != 2) {
        cerr <<"Incorret input list" << endl;
         //print help information
		help();
        return EXIT_FAILURE;
    }

	VideoCapture cap(argv[1]); // open this video
	//VideoCapture cap("input/input%02d.jpg"); // open image files following a naming pattern
	//VideoCapture cap(0); // open video cam
	
    if(!cap.isOpened())  // check if we succeeded
        return -1;
	double fps = cap.get(CV_CAP_PROP_FPS);
	//cout<<fps<<endl;
	
	//Create a window
     namedWindow("original", 1);


	
	
	int iterationCount = 0;
    for(;;)
    {
		iterationCount++;
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

        
       	findContours(bin,contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE ); // finds contours on bin image
       	Scalar color( 255,255,255 );

       	for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
		{
       	    if((contourArea(contours[i],false))>10)
            { // if counter area >100 pixel draw it filled
       	          drawContours( bin, contours, i , color, CV_FILLED, 8, hierarchy ); //Draw contours on itself as filled
            }
        }

        //imshow("threshold",bin); 
        /*
         * Main calculation
         */
		findContours( bin, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
		//vector<Point2f> mc( contours.size() );
		//vector<Moments> mu(contours.size() );
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
																				 // object is fully visible
																				 
					//mu[i] = moments( contours[i], false );
					//mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
					
					int indexInList = ifExistsInObjectList(contours[i],objectList);
					
					if(indexInList == -1) //object is new 
						objectList.push_back(Object(contours[i]));
					
					else{ 
						// update the contour position
						objectList[indexInList].updateContour(contours[i]);
					
						// object already exists
						//if(iterationCount%5 == 0){  // record its centerof mass every 5th frame
						//objectList[indexInList].addCM(contours[i]);
							
							/* draw its path
							for(int i=0;i<objectList[indexInList].cmHistory.size()-1;i++){
								line(frame,
								objectList[indexInList].cmHistory[i],
								objectList[indexInList].cmHistory[i+1],
								Scalar(0,0,255),
								Scalar(0,0,255),
								1,8,0
									);
							}*/
						//}
					}
					
					
					//cout<<objectList.size()<<endl;
					//Bound and Draw rectangle each object which detected at the end on src(original image)
					// draw cm
					//circle(frame,mc[i], 1, Scalar(0,0,255), 1, 8, 0);
					rectangle(frame, bounding_rect,  Scalar(0,255,0),3, 8,0);
				}
            }
       	     
       	}
		
					/*
					// draw every aeroplane's path
					for(int j=0;j<aeroplanes.size();j++){
						
						objectList[aeroplanes[j]].drawPath(frame,Scalar(255,0,0));
					}	
					*/
					// draw path 
					for(int i=0;i<objectList.size();i++){
						objectList[i].drawPath(frame,Scalar(0,0,255));
					}


       	   imshow("original",frame);
       	   //cout<<contours.size()<<"\t";

           //waitKey(1000/fps);
         waitKey(50);
        //if(waitKey(500) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor

    return 0;
}
