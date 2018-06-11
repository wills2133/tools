#include <opencv/cxcore.h>
#include <opencv2/opencv.hpp>

#include <cmath>
#include <vector>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/video/tracking.hpp"
using namespace std;

const int winHeight=600;
const int winWidth=800;


cv::Point mousePosition=cvPoint(winWidth>>1,winHeight>>1);

//mouse event callback
void mouseEvent(int event, int x, int y, int flags, void *param )
{
    if (event==CV_EVENT_MOUSEMOVE) {
        mousePosition=cvPoint(x,y);
    }
}

int main (void)
{
    //1.kalman filter setup
    const int stateNum=4;
    const int measureNum=2;
    cv::KalmanFilter kalman( stateNum, measureNum, 0 );//state(x,y,detaX,detaY)
    cv::Mat process_noise( stateNum, 1, CV_32FC1 );
    cv::Mat measurement( measureNum, 1, CV_32FC1 );//measurement(x,y)
    //cv::RNG rng = cvRNG(-1);
    kalman.transitionMatrix = (cv::Mat_<float>(4,4)<<
        1,0,1,0,
        0,1,0,1,
        0,0,1,0,
        0,0,0,1);
    std::cout<<kalman.transitionMatrix<<std::endl;
    //kalman.transitionMatrix = tranMat;
    //memcpy( kalman.transitionMatrix.data.fl,A,sizeof(A));//copy matrix to kalman data
    cv::setIdentity(kalman.measurementMatrix, cv::Scalar::all(1) );
    cv::setIdentity(kalman.processNoiseCov, cv::Scalar::all(1e-5));
    cv::setIdentity(kalman.measurementNoiseCov, cv::Scalar::all(1e-1));
    cv::setIdentity(kalman.errorCovPost, cv::Scalar::all(1));
    kalman.statePost.ptr<float>(0)[0] = 300;
    kalman.statePost.ptr<float>(1)[0] = 400;
    //setIdentity(kf->measurementNoiseCov, Scalar(26 / 1000.0, 25 / 1000.0, 95 / 1000.0, 96 / 1000.0));
    //initialize post state of kalman filter at random
    //randArr(&rng,kalman.state_post,CV_RAND_UNI,cvRealScalar(0),cvRealScalar(winHeight>winWidth?winWidth:winHeight));

    //cv::Font font;
    //cv::initFont(&font,CV_FONT_HERSHEY_SCRIPT_COMPLEX,1,1);

    cv::namedWindow("kalman");
    cv::setMouseCallback("kalman",mouseEvent);
    cv::Mat img(cv::Size(winWidth,winHeight),CV_8UC3,cv::Scalar(255,255,255));
    while (1){
        //2.kalman prediction
        const cv::Mat prediction=kalman.predict();
        cv::Point predict_pt=cvPoint((int)prediction.ptr<float>(0)[0],(int)prediction.ptr<float>(1)[0]);

        //3.update measurement
        measurement.ptr<float>(0)[0]=(float)mousePosition.x;
        measurement.ptr<float>(1)[0]=(float)mousePosition.y;

        //4.update
        kalman.correct( measurement );     

        cv::circle(img,predict_pt,5,CV_RGB(0,255,0),3);//predicted point with green
        cv::circle(img,mousePosition,5,CV_RGB(255,0,0),3);//current position with red
        // char buf[256];
        //sprintf(buf,"predicted position:(%3d,%3d)",predict_pt.x,predict_pt.y);
        //cv::putText(img,buf,cv::Point(10,30),cv::FONT_HERSHEY_PLAIN, CV_RGB(0,0,0));
        //sprintf(buf,"current position :(%3d,%3d)",mousePosition.x,mousePosition.y);
        //cv::putText(img,buf,cv::Point(10,60),cv::FONT_HERSHEY_PLAIN, CV_RGB(0,0,0));
        
        cv::imshow("kalman", img);
    
        int key=cv::waitKey(3);
        if (key==27){//esc   
            break;   
        }
    }      
    return 0;
}
