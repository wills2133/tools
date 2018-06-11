#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

#define nWidth 352
#define nHeight 240
// 全局变量 
cv::Mat srcImage;
unsigned char *pBuf = new unsigned char[nWidth*nHeight*3/2];
void MouseEvent(int event, int x, int y, int flags, void* data)
{
    char charText[30];
    cv::Mat tempImage, hsvImage;
    tempImage = srcImage.clone();
    // 左键按下移动显示RGB信息
    if (event == CV_EVENT_LBUTTONDOWN)
    {
        cv::Vec3b p = tempImage.at<cv::Vec3b>(y,x);
        sprintf(charText, "R=%d, G=%d, B=%d", p[2], p[1], p[0]);
        // 写RGB文本信息到图像
	    cv::putText(tempImage, charText, cv::Point(8,20),
	           cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(255,255,255));
        imwrite("infoRGB.jpg", tempImage);
    }
    // 右键按下移动显示HSV信息
    else if (event == CV_EVENT_RBUTTONDOWN)
    {

    		    // 移动鼠标显示坐标
	    sprintf(charText, "x=%d, y=%d", x, y);
	    // 写XY坐标文本信息到图像
    	cv::putText(tempImage, charText, cv::Point(8,20),
           cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(0,0,0));
	    imwrite("info.jpg", tempImage);
    	   
    }
    // 没有按下键显示对应坐标
    else
    {
    	 long int imgHeadU = nWidth*nHeight;
		    long int imgHeadV = nWidth*nHeight*5/4;

    		unsigned char yy = *(pBuf +  (y * nWidth + x));
    		unsigned char uu = *(pBuf + imgHeadU + (y * nWidth / 4 + x / 2));
    		unsigned char vv = *(pBuf + imgHeadV + (y * nWidth / 4 + x / 2));
    		// printf("%C\n", &yy);
    		// cout<<"d "<<hex<<static_cast<const void *>(&yy)<<endl;
    		// cout<<"y coordiante "<<static_cast<const void *>(pBuf)<<endl;
    	     
    	    Mat clrSampleYUV(30, 20, CV_8UC1);

            unsigned char clrPixels[20*20*3/2];
            memset(clrPixels, yy, sizeof(unsigned char)*400);
            memset(clrPixels + (sizeof(unsigned char)*400), uu, sizeof(unsigned char)*100);
            memset(clrPixels + (sizeof(unsigned char)*500), vv, sizeof(unsigned char)*100);
            clrSampleYUV.data = clrPixels;
            Mat clrSampleRGB;
            cv::cvtColor(clrSampleYUV, clrSampleRGB, CV_YUV2BGR_I420);
            imshow("clrSampleRGB", clrSampleRGB);


        // 获取相应的通道值
        sprintf(charText, "Y=%d, U=%d, V=%d", yy, uu, vv);
        // 写HSV文本信息到图像
    	cv::putText(tempImage, charText, cv::Point(8,20),
           cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(0,0,0));
    	cv::imshow("srcImage", tempImage);
	}
    cv::imshow("srcImage", tempImage);
    
}
int main(int argc, char** argv)
{

    FILE *f ;
    if(!(f = fopen("sequence_000.raw","rb")))
    {
        printf("file open error!");
    }


    //----------------------------------------------------------------------实现读取一个文件 显示照片
    
    fread(pBuf, 1, (nWidth * nHeight * 3) / 2, f);// nWidth*nHeight*3/2  means 本程序使用 yuv:4:2:0 yuv分为 Y Cb Cr 三部分
    //cvSetData(yimg, pBuf, nWidth);

    cv::Mat frameYUV(nHeight*3/2, nWidth, CV_8UC1);
    cv::Mat frameRGB;

    frameYUV.data = pBuf;

    cv::cvtColor(frameYUV, frameRGB, CV_YUV2BGR_I420);

    cv::cvtColor(frameRGB, frameYUV, CV_BGR2YUV_I420);

    // imshow("frameRGB", frameYUV);


    	    long int imgHeadU = nWidth*nHeight;
		    long int imgHeadV = nWidth*nHeight*5/4;
		    unsigned char clrPixels[nWidth*nHeight*3/2];

		    for(int x=0; x<nWidth; x++){
		    	for(int y=0; y<nHeight; y++){
		    		unsigned char yy = *(pBuf +  (y * nWidth + x));
		    		unsigned char uu = *(pBuf + imgHeadU + (y * nWidth / 4 + x / 2));
		    		unsigned char vv = *(pBuf + imgHeadV + (y * nWidth / 4 + x / 2));


		            memset(clrPixels+(y * nWidth + x), yy, sizeof(unsigned char));
		            memset(clrPixels+imgHeadU + (y * nWidth / 4 + x / 2), uu, sizeof(unsigned char));
		            memset(clrPixels+imgHeadV + (y * nWidth / 4 + x / 2), vv, sizeof(unsigned char));
		    	}
		    }

    		// printf("%C\n", &yy);
    		// cout<<"d "<<hex<<static_cast<const void *>(&yy)<<endl;
    		// cout<<"y coordiante "<<static_cast<const void *>(pBuf)<<endl;
    	     
    	    Mat clrSampleYUV(nHeight*3/2, nWidth, CV_8UC1);

            

            clrSampleYUV.data = clrPixels;
            Mat clrSampleRGB;
            cv::cvtColor(clrSampleYUV, clrSampleRGB, CV_YUV2BGR_I420);
            imshow("change back", clrSampleRGB);

//////////////////////////////////////////////////////////////////////////////////////

    srcImage =  frameRGB;
    if (srcImage.empty())
        return -1;
    // 回调事件响应
    cv::namedWindow("srcImage");
    cv::setMouseCallback("srcImage", MouseEvent, 0);
    cv::imshow("srcImage", srcImage);
    cv::waitKey(0);
    return 0;
}
