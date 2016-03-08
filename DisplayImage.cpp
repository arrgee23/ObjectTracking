#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture cap("input/input%02d.jpg"); // open the default camera

    if(!cap.isOpened())  // check if we succeeded
        return -1;

    /*
		Getting the greyscale color value of sky to threshold wrt it
		Assuming most of the image is of sky
    */

    // read first frame
    Mat dummyFrame;
    if(!cap.read(dummyFrame))
    	return 1;


	//cout<<dummyFrame.channels(); 3 channels

    // convert to greyscale
    cvtColor(dummyFrame,dummyFrame,CV_BGR2GRAY);
    medianBlur(dummyFrame,dummyFrame,7); // blur it to remove noise

    // cout<<dummyFrame.channels(); 1 channel

    //CV_Assert(dummyFrame.depth() == CV_8U);
    uchar histogram[256];
    for(int i=0;i<256;i++)
    	histogram[i]=0;

    for(int j=0;j<dummyFrame.rows;j++) 
	{
  		for (int i=0;i<dummyFrame.cols;i++)
  		{
    		   
       		histogram[dummyFrame.at<uchar>(j,i)]++;
  		}
	}

	// find most used colour for thresholding
	int maxcount = 0;
	for(int i=0;i<256;i++)
		if(histogram[i]>maxcount)
			maxcount = histogram[i];

	/* maxcount is the color of the sky */


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
       	threshold(dest, bin, 40, 255, CV_THRESH_BINARY_INV); // // Tresholds image with level = 40 from gray level(dst) to binary (bin)
       	imshow("threshold",bin);
       	findContours(bin,contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE ); // finds contours on bin image

       	Scalar color( 255,255,255 );
       	for( int i = 0; i< contours.size(); i++ ) // iterate through each contour. 
       	  {
       	   if((contourArea(contours[i],false))>100){ // if counter area >100 pixel draw it on ero which is new image variable
       	    drawContours( bin, contours, i , color, CV_FILLED, 8, hierarchy ); //Draw contours on itself as filled
       	                                            }
       	   }

       	findContours( bin, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );

       	 for( int i = 0; i< contours.size(); i++ ) // iterate through each contour. 
       	  {
       	      bounding_rect=boundingRect(contours[i]); //Bound and Draw rectangle each object which detected at the end on src(original image)
       	      rectangle(frame, bounding_rect,  Scalar(0,255,0),3, 8,0);  
       	   }
       	   
       	   imshow("original",frame);
       	   cout<<contours.size();

        if(waitKey(500) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}