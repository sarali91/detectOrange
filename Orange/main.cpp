//
//  main.cpp
//  Orange
//
//  Created by Xiangyu Li on 2/2/16.
//  Copyright (c) 2016 Xiangyu Li. All rights reserved.
//

#include <iostream>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <stdio.h>
#include <opencv/cxcore.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



using namespace cv;
using namespace std;

void on_trackbar( int, void* );
Mat frame;
Mat edges;
Mat bnr2;
Mat keypoints;

int iLowH=2;//13 ;
int iHighH=84;//55;

int iLowS=114;//90 ;
int iHighS =255;

int iLowV=140;//80 ;
int iHighV=255 ;

RNG rng(12345);


int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    namedWindow("Control",1);
    //Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 179,on_trackbar); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 179,on_trackbar);
    
    createTrackbar("LowS", "Control", &iLowS, 255,on_trackbar); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255,on_trackbar);
    
    createTrackbar("LowV", "Control", &iLowV, 255,on_trackbar); //Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255,on_trackbar);

    
    //frame num = pin
    int pin=0;
    
    for(;;)
    {
        pin++;
        
        cap >> frame; // get a new frame from camera
        
        namedWindow("Original",1);
        imshow("Original", frame);
        
        cvtColor(frame, edges, COLOR_BGR2HSV);
        
        //namedWindow("edges",1);
        //imshow("edges", edges);
        
        inRange(edges,Scalar(iLowH,iLowS,iLowV),Scalar(iHighH,iHighS,iHighV),bnr2);

        //namedWindow("edges",1);
        imshow("Control", bnr2);

        /*
        //morphological opening (remove small objects from the foreground)
        erode(bnr2, bnr2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate(bnr2, bnr2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        
         */
        
        //morphological closing (fill small holes in the foreground)
        dilate(bnr2, bnr2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(bnr2, bnr2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        

        namedWindow("HSV",1);
        imshow("HSV", bnr2);
        
        /*
        //bounding box
        Mat threshold_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        
        /// Detect edges using Threshold
        threshold( bnr2, threshold_output, 150, 255, THRESH_BINARY );
        /// Find contours
        findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        
        /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );
        
        int maxR=0;
        int tempR=0;
        for( int i = 0; i < contours.size(); i++ )
        {   approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
            if(radius[i]>tempR)
            {
                maxR=i;
                tempR=(int)radius[i];
            }
            
        }
        
        //print out radius of orange
        if(pin%10==0)
            cout<<"Radius of orange is "<<(int)tempR<<endl;
        */
        
        Mat orange = Mat::zeros( frame.size(), frame.type() );;
        //for( int i = 0; i< contours.size(); i++ )
        //{
        
            //circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
        //}
        
        /// Show in a window
        //namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
        //imshow( "Contours", orange );
        
        
        
        for( int y = 0; y < frame.rows; y++ )
        { for( int x = 0; x < frame.cols; x++ )
        {
            
            if(bnr2.at<uchar>(y,x)==255) //if is orange
            {
                //[0] is blue channel
                orange.at<Vec3b>(y,x)[0] =
                saturate_cast<uchar>( ( frame.at<Vec3b>(y,x)[0]+100 ) );
                //50 is the value to change orange[0] to blue[0]
                //[2] is red channel
                orange.at<Vec3b>(y,x)[2] =
                saturate_cast<uchar>( ( frame.at<Vec3b>(y,x)[2]-100) );
                //50 is the value to change orange[0] to blue[0]
            }
            else
            {
                orange.at<Vec3b>(y,x)[0] =
                saturate_cast<uchar>( ( frame.at<Vec3b>(y,x)[0] ) );
                orange.at<Vec3b>(y,x)[2] =
                saturate_cast<uchar>( ( frame.at<Vec3b>(y,x)[2] ) );
            }
            
                orange.at<Vec3b>(y,x)[1] =
                saturate_cast<uchar>( frame.at<Vec3b>(y,x)[1] );
            
        }
        }
        
        Mat threshold_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        
        /// Detect edges using Threshold
        threshold( bnr2, threshold_output, 150, 255, THRESH_BINARY );
        /// Find contours
        findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        
        /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );
        
        int MaxR=0; //max i
        int tempR=0;    //temp max R
        for( int i = 0; i < contours.size(); i++ )
        {   approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
            if((int)radius[i]>tempR){
                tempR=radius[i];
                MaxR=i;
            }
        }
        
        if(pin%10==0){
            cout<<"Diameter of orange is "<<2*0.052917*tempR<<" cm"<<endl;
        }
        
        /// Draw polygonal contour + bonding rects + circles
        //Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
        {
            if(i==MaxR){
                Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                //drawContours( orange, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
                rectangle( orange, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
                //circle( orange, center[i], (int)radius[i], color, 2, 8, 0 );
            }
        }
        
        /// Show in a window
//namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//imshow( "Contours", drawing );
       // Mat orange;
        //cvtColor(orange, orange, COLOR_HSV2RGB);
        
        /// Draw polygonal contour + bonding rects + circles
//Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
         //drawContours( orange, contours_poly, maxR, color, 1, 8, vector<Vec4i>(), 0, Point() );
        
//rectangle( orange, boundRect[maxR].tl(), boundRect[maxR].br(), color, 2, 8, 0 );

        namedWindow("Orange",1);
        imshow("Orange", orange);
        
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}



void on_trackbar(int,void*)
{//This function gets called whenever a
    // trackbar position is changed
    
    //inRange(edges,Scalar(iLowH,iLowS,iLowV),Scalar(iHighH,iHighS,iHighV),bnr2);
   // imshow("Control", bnr2);

    
    

    
}
