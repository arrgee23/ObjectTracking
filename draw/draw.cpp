#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Object.cpp"

using namespace cv;
using namespace std;


vector<Object> objectList;

int ifExistsInObjectList(vector<Point> c, vector<Object> objectList){
	
	for(int i=0;i<objectList.size();i++)
		if(objectList[i].isSame(c))
			return i;
	
	return -1;
}

int main( int argc, char** argv )
{
    VideoCapture cap(0); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0; 
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;
	
	int draw = 0;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
	
	cvCreateTrackbar("draw", "Control", &draw, 1);
    
    while (true)
    {
        Mat imgOriginal;
		Rect bounding_rect;
		vector<Vec4i> hierarchy;
		vector<vector<Point> > contours;
		
        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
		flip(imgOriginal, imgOriginal, 1);
		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
		
		
		//morphological opening (remove small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
		dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) ); 

		//morphological closing (fill small holes in the foreground)
		dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) ); 
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
		
		
		dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) ); 
		dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) ); 
		
		if(draw==1){
			

			//imshow("threshold",bin); 
			/*
			 * Main calculation
			 */
			findContours( imgThresholded, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
			
			for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
			{
				
				if((contourArea(contours[i],false))>20)
				{  
					
					bounding_rect=boundingRect(contours[i]);
					Point tl = bounding_rect.tl(); // topleft and bottom right
					Point br = bounding_rect.br();
				
					Size s = imgThresholded.size();
					
					//cout<<"sizze "<<s<<endl;
					
					if(tl.x > 3 && tl.y > 3 && br.x< s.width-3 && br.y< s.height-3){ // make sure the bounding rectangle is within window
																					 // object is fully visible
																					 
						
						int indexInList = ifExistsInObjectList(contours[i],objectList);
						
						if(indexInList == -1) //object is new 
							objectList.push_back(Object(contours[i]));
						
						else{ 
							// update the contour position
							objectList[indexInList].updateContour(contours[i]);
						
						}
						
					
						rectangle(imgOriginal, bounding_rect,  Scalar(0,255,0),3, 8,0);
					}
				}
				 
			}
			
						
						// draw path 
						for(int i=0;i<objectList.size();i++){
							objectList[i].drawPath(imgOriginal,Scalar(0,0,255));
						}
		
		}
		
		imshow("Control", imgThresholded); //show the thresholded image
		imshow("Original", imgOriginal); //show the original image
		
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
    }

   return 0;

}
