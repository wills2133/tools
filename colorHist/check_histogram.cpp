#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv.hpp>
#include <opencv/cv.h>
//usage  ./test.exe dirname height width

using namespace std;
using namespace cv;


#define nWidth 352
#define nHeight 240
#define FrameSize nWidth*nHeight*3/2

int sumRed = 0;
#if 0
int ShowHistogram2(cv::Mat srcImage)
{

cv::imshow("srcImage",srcImage);  
// 分离图像的三个通道 B  G R
std::vector<cv::Mat> bgr_planes;
split( srcImage, bgr_planes );
// 初始化直方图计算参数
int histSize = 256; 
float range[] = { 0, 256 }; 
const float* histRange = { range }; 
bool uniform = true; 
bool accumulate = false;
cv::Mat b_hist, g_hist, r_hist;
// 计算各个通道的直方图
calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, 
    &histSize, &histRange, uniform, accumulate );
calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, 
    &histSize, &histRange, uniform, accumulate );
calcHist( &bgr_planes[2], 1, 0, cv::Mat(),r_hist, 1, 
    &histSize, &histRange, uniform, accumulate );
// 设置直方图绘图参数
int hist_w = 640; int hist_h = 512;
int bin_w = cvRound( (double) hist_w/histSize );
  cv::Mat histImage( hist_h, hist_w, 
                    CV_8UC3, cv::Scalar( 0,0,0) );
// 分别归一化直方图到[ 0, histImage.rows ]
normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, 
           -1, Mat() );
normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, 
           -1, Mat() );
normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, 
           -1, Mat() );
// 分别对每个通道进行绘图
for( int i = 1; i < histSize; i++ )
{
  line( histImage, Point( bin_w*(i-1), 
         hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
         Point( bin_w*(i), hist_h - 
         cvRound(b_hist.at<float>(i)) ),
         Scalar( 255, 0, 0), 2, 8, 0  );
  line( histImage, Point( bin_w*(i-1), hist_h - 
         cvRound(g_hist.at<float>(i-1)) ) ,
         Point( bin_w*(i), hist_h - 
         cvRound(g_hist.at<float>(i)) ),
                 Scalar( 0, 255, 0), 2, 8, 0  );
  line( histImage, Point( bin_w*(i-1), hist_h - 
         cvRound(r_hist.at<float>(i-1)) ) ,
         Point( bin_w*(i), hist_h - 
         cvRound(r_hist.at<float>(i)) ),
         Scalar( 0, 0, 255), 2, 8, 0  );
}
imshow("calcHist", histImage );
waitKey(0);
return 0;
}

#endif

#if 0
void ShowHistogram(cv::Mat srcImage)
{
    IplImage src2 = srcImage;
    IplImage *src = &src2;
 
    IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );  //第一个为size，第二个为位深度（8为256度），第三个通道数
    IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
    IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
    IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
    IplImage* planes[] = { h_plane, s_plane };
 
    // H 分量划分为16个等级，S分量划分为8个等级 
    int h_bins =16 , s_bins =8, v_bins = 8;
    int hist_size[] = {h_bins, s_bins};
    // H 分量的变化范围 
    float h_ranges[] = { 0, 180 }; 
 
    //  S 分量的变化范围
    float s_ranges[] = { 0, 255 };
    float v_ranges[] = { 0, 255 };

    float* ranges[] = { h_ranges, s_ranges};
 
    // 输入图像转换到HSV颜色空间 
    cvCvtColor( src, hsv, CV_BGR2HSV );
    cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
 
    //创建直方图，二维, 每个维度上均分 
    CvHistogram * hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
    //根据H,S两个平面数据统计直方图 
    cvCalcHist( planes, hist, 0, 0 );
 
    // 获取直方图统计的最大值，用于动态显示直方图 
    float max_value;
    cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
 
 
    // 设置直方图显示图像 
    int height = 300;
    int width = (h_bins*s_bins*8);
    IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
    cvZero( hist_img );

    //用来进行HSV到RGB颜色转换的临时单位图像 
    IplImage * hsv_color = cvCreateImage(cvSize(1,1),8,3);
    IplImage * rgb_color = cvCreateImage(cvSize(1,1),8,3);
    int bin_w = width / (h_bins * s_bins);

    for(int s = 0; s < s_bins; s++)
    {
        for(int h = 0; h < h_bins; h++)
        {
 
            int i = s*h_bins + s;
            // 获得直方图中的统计次数，计算显示在图像中的高度 
            float bin_val = cvQueryHistValue_2D( hist, h, s );
            int intensity = cvRound(bin_val*height/max_value);
 
            // 获得当前直方图代表的颜色，转换成RGB用于绘制 
            cvSet2D(hsv_color,0,0,cvScalar(h*180.f / h_bins,s*255.f/s_bins,255));
            cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
            CvScalar color = cvGet2D(rgb_color,0,0);
 
            cvRectangle( hist_img, cvPoint(i*bin_w,height),
                cvPoint((i+1)*bin_w,height - intensity),
                color, -1, 8, 0 );
            char charText2[100];
            sprintf(charText2, "%d",i);
            cv::Mat hist_img2( hist_img);
            cv::putText(hist_img2, charText2, cv::Point(i*bin_w, height - intensity), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(0, 255, 0), 1);
            // std::cout<<" "<<cv::Point(i*bin_w,height)<<" "<<cv::Point((i+1)*bin_w,height - intensity)<<std::endl;
            // cvNamedWindow( "H-S-V Histogram",1);
            // cvShowImage( "H-S-V Histogram", hist_img );
            // cv::waitKey(0);
            if(i<6||i>112){
                sumRed += intensity;
            }
            // std::cout<<"i "<<i<<"sumRed "<<sumRed<<std::endl;
        }
    }


    cvNamedWindow( "Source22", 1 );
    cvShowImage( "Source22", src );
    cvNamedWindow( "H-S-V Histogram",1);
    cvShowImage( "H-S-V Histogram", hist_img );
    cvWaitKey(0);
}
#endif



int main(int argc, char **argv)
{
    FILE *f ;
    if(!(f = fopen("sequence_000.raw","rb")))
    {
        printf("file open error!");
    }


    //----------------------------------------------------------------------实现读取一个文件 显示照片
    unsigned char *pBuf = new unsigned char[nWidth*nHeight*3/2];
    fread(pBuf, 1, (nWidth * nHeight * 3) / 2, f);// nWidth*nHeight*3/2  means 本程序使用yuv:4:2:0 yuv分为 Y Cb Cr 三部分
    //cvSetData(yimg, pBuf, nWidth);

    cv::Mat frameYUV(nHeight*3/2, nWidth, CV_8UC1);
    cv::Mat frameRGB;

    frameYUV.data = pBuf;

    cv::cvtColor(frameYUV, frameRGB, CV_YUV2BGR_I420);

    cv::cvtColor(frameRGB, frameYUV, CV_BGR2YUV_I420);

    imshow("frameRGB", frameRGB);

    long int imgHeadU = nWidth*nHeight;
    long int imgHeadV = nWidth*nHeight*5/4;

    cv::Mat plateU(nHeight/2, nWidth/2, CV_8UC1); 
    cv::Mat plateV(nHeight/2, nWidth/2, CV_8UC1);

    plateU.data = pBuf+imgHeadU;
    plateV.data = pBuf+imgHeadV;

    imshow("plateU", plateU);
    imshow("plateV", plateV);
    // waitKey(0);
///////////////////////////////////////////////////////////
/*
    int histSize2 = 256;
    float rangeMin = 0, rangeMax = 256;
    float range2[] = { rangeMin, rangeMax }; 
    const float* histRange2 = { range2 }; 
    bool uniform2 = true; 
    bool accumulate2 = false;
    cv::Mat uh, vh;
    // 计算各个通道的直方图
    calcHist( &plateU, 1, 0, cv::Mat(), uh, 1, 
        &histSize2, &histRange2, uniform2, accumulate2 );
    calcHist( &plateV, 1, 0, cv::Mat(), vh, 1, 
        &histSize2, &histRange2, uniform2, accumulate2 );

    // 设置直方图绘图参数
    int hist_w = 640; int hist_h = 512;
    int bin_w = cvRound( (double) hist_w/histSize2 );
      cv::Mat histImage2( hist_h, hist_w, 
                        CV_8UC3, cv::Scalar( 0,0,0) );
    // 分别归一化直方图到[ 0, histImage.rows ]
    normalize(uh, uh, 0, histImage2.rows, NORM_MINMAX, 
               -1, Mat() );
    normalize(vh, vh, 0, histImage2.rows, NORM_MINMAX, 
               -1, Mat() );

    // 分别对每个通道进行绘图
    for( int i = 1; i < histSize2; i++ )
    {
      line( histImage2, Point( bin_w*(i-1), 
             hist_h - cvRound(uh.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - 
             cvRound(uh.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage2, Point( bin_w*(i-1), hist_h - 
             cvRound(vh.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - 
             cvRound(vh.at<float>(i)) ),
                     Scalar( 0, 255, 0), 2, 8, 0  );

                char charText[100];
                sprintf(charText, "%d",i*(rangeMax-rangeMin)/histSize2+rangeMin);
                cv::putText(histImage2, charText, Point( bin_w*(i-1), 
             hist_h - cvRound(uh.at<float>(i-1)) ), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(0, 255, 0), 1);
                char charText2[100];
                sprintf(charText2, "%d",i);
                cv::putText(histImage2, charText2, Point( bin_w*(i-1), hist_h - 
             cvRound(vh.at<float>(i-1)) ), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(0, 255, 0), 1);
    }
    imshow("calcHist2", histImage2 );

*/
////////////////////////////////////////////////////////
 

    // 初始化直方图计算参数
    Mat plateYUV[] = { plateU, plateV };
    // plateYUV = &plateU;
    // plateYUV[1] = &plateV;
    int histSizeU = 16;
    int histSizeV = 16;
    int histSize[] = {histSizeU, histSizeV}; 
    float rangeUmin = 0, rangeUmax = 256;
    float rangeVmin = 0, rangeVmax = 256;
    float rangeU[] = { rangeVmin, rangeUmax }; 
    float rangeV[] = { rangeVmin, rangeVmax }; 
    const float* histRange[] = { rangeU, rangeV }; 
    bool uniform = true; 
    bool accumulate = false;
    cv::Mat hist;
        // 计算各个通道的直方图
        calcHist( plateYUV, 2, 0, cv::Mat(), hist, 2, 
            histSize, histRange, uniform, accumulate );

        double maxVal = 0;
        minMaxLoc(hist, 0, &maxVal, 0, 0);

        // int scale = 10;
        Mat histImg = Mat::zeros(512, histSizeU*histSizeV*5, CV_8UC3);
        cout<<"histImg size"<<histImg.size()<<endl;
        //draw color bar
        for(int v=0; v<histSizeV; v++){
            for(int u=0; u<histSizeU; u++){
            
                int k = v*histSizeU + u;
                // int k = u*histSizeV + v;
                float binVal = hist.at<float>(u, v);
                int intensity = cvRound(binVal*512/maxVal);
                // binClr = CvScalar

                Mat clrSampleYUV(30, 20, CV_8UC1);
                unsigned char clrPixels[30*20];
                memset(clrPixels, 128, sizeof(unsigned char)*400);
                memset(clrPixels+sizeof(unsigned char)*400, u*(rangeUmax-rangeUmin)/histSizeU+rangeUmin, sizeof(unsigned char)*100);
                memset(clrPixels+sizeof(unsigned char)*500, v*(rangeVmax-rangeVmin)/histSizeV+rangeVmin, sizeof(unsigned char)*100);
                // memset(clrPixels+sizeof(unsigned char)*400, 100, sizeof(unsigned char)*100);
                // memset(clrPixels+sizeof(unsigned char)*500, 100 , sizeof(unsigned char)*100);
                clrSampleYUV.data = clrPixels;
                Mat clrSampleRGB;
                cv::cvtColor(clrSampleYUV, clrSampleRGB, CV_YUV2BGR_I420);
                imshow("clrSampleRGB", clrSampleRGB);
                rectangle( histImg, Point( k*5, 512-intensity ), Point( (k+1)*5, 512 ), mean(clrSampleRGB), CV_FILLED );
                int histNum = u*(rangeUmax-rangeUmin)/histSizeU+rangeUmin;
                int histNum2 = v*(rangeVmax-rangeVmin)/histSizeV+rangeVmin;
                if(intensity>0){
                    cout<<"u "<<histNum<<" v "<<histNum2<<" intensity "<<intensity<<endl; 
                }
                if(k%10==0){
                    char charText[100];
                    sprintf(charText, "%d", histNum);
                    cv::putText(histImg, charText, Point( k*5+5, 512-intensity-10 ), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(0, 255, 0), 1);
                    char charText2[100];
                    sprintf(charText2, "%d", histNum2);
                    cv::putText(histImg, charText2, Point( k*5+5, 512-intensity  ), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(0, 255, 0), 1);
                }
                // imshow("calcHist", histImg );
                // waitKey(0);
            }
        }


    imshow("calcHist", histImg );
    waitKey(0);

    
    //////////////////////////////////////////////////////////////
    // ///////////////////////////////////////////////////////////////

    // DIR *dirp;
    // struct dirent *dirt;
    // char file_name[50];
    // int file_count=1;      //file_count为计数.
    // cv::Mat img;

    // if ((dirp = opendir(argv[1])) ==NULL){
    //     printf ("open dir %s failed\n",argv[1]);
    //     return 0;
    // }
    
    // chdir(argv[1]);
    // int countRed=0, count=0;
    // while ( (dirt = readdir(dirp)) != NULL){
    //     if ((strcmp(dirt->d_name,"..") !=0) && (strcmp(dirt->d_name,".")!=0)){

    //         img = cv::imread(dirt->d_name);
    //         if(!img.data){
    //             std::cout<<dirt->d_name<<" not a img file"<<std::endl;
    //             // printf("not a img file!\n");
    //             continue;
    //         }



    //         // cv::imshow("img", img);
    //         ShowHistogram2(img);
    //         // if(sumRed >10)
    //         //     countRed++;
    //         // count++;
    //         // std::cout<<"sumRed "<<sumRed<<" count "<<count<<std::endl;
    //         // sumRed = 0;
    //         // cv::waitKey(0);

    //         // memset(file_name, 0, sizeof(file_name));
    //         // ///strcpy(file_name, dirt->d_name);
    //         // ///sprintf(file_name + strlen(dirt->d_name), "_%d", file_count++);
    //         // sprintf(file_name, "(%d)", file_count++);///write : pointer of dir_name+strlen of dir_name, then number
    //         // if (rename(dirt->d_name, file_name) < 0){  //file_name 为新的命名
    //         //     printf("rename failed\n");
    //         //     return 0;
    //         // }
    //     }
    // }
    // std::cout<<"countRed "<<countRed<<" count "<<count<<" percentage "<<countRed*100/count<<"%"<<std::endl;
    return 0;
}