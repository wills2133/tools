#include <opencv/highgui.h>
#include <stdio.h> 
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"


#define nWidth 352
#define nHeight 240
#define FrameSize nWidth*nHeight*3/2


#define ZIMU_HEIGHT 25
#define ZIMU_WIDTH  16

typedef struct cPointInfo{
    int x;
    int y;
}PointInfo;

typedef struct cRectInfo{
    PointInfo Top; //顶点
    int High;
    int Width;
}RectInfo;


typedef enum{
    SET_RED =0,
    SET_GREEN = 1
}ColorType;

const unsigned char  Bmp[11][ZIMU_HEIGHT*ZIMU_WIDTH/8]={
{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 0
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0x07,0xE0,0x0F,0xF8,0x1F,0xFC,0x3C,0x3C,0x78,0x1E,0x70,0x0E,0x70,0x0E,0x70,0x07,
0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,
0xE0,0x07,0xE0,0x07,0x70,0x0E,0x70,0x0E,0x78,0x1E,0x3C,0x3C,0x3F,0xF8,0x1F,0xF0,
0x07,0xE0,
},

{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 1
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0x00,0xC0,0x1F,0xC0,0x7F,0xC0,0x7F,0xC0,0x61,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,
0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,
0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x7F,0xFE,0x7F,0xFE,
0x7F,0xFE,
},

{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 2
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0x07,0xE0,0x1F,0xF8,0x3F,0xFC,0x78,0x3C,0x60,0x1E,0x40,0x0E,0x00,0x0E,0x00,0x0E,
0x00,0x0E,0x00,0x0E,0x00,0x1C,0x00,0x1C,0x00,0x38,0x00,0x70,0x00,0xE0,0x01,0xE0,
0x03,0xC0,0x07,0x80,0x0E,0x00,0x1C,0x00,0x38,0x00,0x70,0x00,0xFF,0xFE,0xFF,0xFE,
0xFF,0xFE,
},


{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 3
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0x0F,0xC0,0x3F,0xF0,0x7F,0xF8,0x70,0x78,0x40,0x3C,0x00,0x1C,0x00,0x1C,0x00,0x1C,
0x00,0x1C,0x00,0x38,0x00,0xF0,0x1F,0xE0,0x1F,0xC0,0x1F,0xF8,0x00,0x7C,0x00,0x1C,
0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x80,0x1E,0xE0,0x7C,0xFF,0xF8,0x7F,0xF0,
0x1F,0xC0,
},


{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 4
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0x00,0x3C,0x00,0x7C,0x00,0x7C,0x00,0xFC,0x01,0xDC,0x01,0xDC,0x03,0x9C,0x07,0x1C,
0x06,0x1C,0x0E,0x1C,0x1C,0x1C,0x38,0x1C,0x38,0x1C,0x70,0x1C,0xE0,0x1C,0xE0,0x1C,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,
0x00,0x1C,
},

{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 5
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0x3F,0xFC,0x3F,0xFC,0x3F,0xFC,0x38,0x00,0x38,0x00,0x38,0x00,0x38,0x00,0x38,0x00,
0x38,0x00,0x3F,0xC0,0x3F,0xF0,0x3F,0xF8,0x00,0x7C,0x00,0x1C,0x00,0x1E,0x00,0x0E,
0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x1E,0x40,0x1C,0x70,0x7C,0x7F,0xF8,0x3F,0xF0,
0x1F,0xC0,
},

{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 6
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0x01,0xF8,0x07,0xFC,0x0F,0xFC,0x1E,0x04,0x3C,0x00,0x38,0x00,0x70,0x00,0x70,0x00,
0x60,0x00,0xE3,0xF0,0xE7,0xF8,0xEF,0xFC,0xFC,0x1E,0xF0,0x0E,0xE0,0x07,0xE0,0x07,
0xE0,0x07,0xE0,0x07,0x60,0x07,0x70,0x0F,0x78,0x0E,0x3C,0x3E,0x3F,0xFC,0x1F,0xF8,
0x07,0xE0,
},

{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 7
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0x00,0x1C,0x00,0x3C,0x00,0x38,0x00,0x38,0x00,0x78,
0x00,0x70,0x00,0x70,0x00,0xF0,0x00,0xE0,0x00,0xE0,0x01,0xC0,0x01,0xC0,0x03,0xC0,
0x03,0x80,0x03,0x80,0x07,0x00,0x07,0x00,0x07,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,
0x1C,0x00,
},

{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 8
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0x07,0xE0,0x1F,0xF0,0x3F,0xF8,0x3C,0x3C,0x78,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,
0x38,0x38,0x3C,0x78,0x1E,0xF0,0x0F,0xE0,0x0F,0xE0,0x3E,0xF8,0x78,0x3C,0x70,0x1C,
0xE0,0x0E,0xE0,0x0E,0xE0,0x0E,0xE0,0x0E,0xF0,0x1E,0x78,0x3C,0x7F,0xFC,0x3F,0xF8,
0x0F,0xE0,
},

{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : 9
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0x07,0xC0,0x1F,0xF0,0x3F,0xF8,0x78,0x78,0x70,0x1C,0xF0,0x1C,0xE0,0x0E,0xE0,0x0E,
0xE0,0x0E,0xE0,0x0E,0xE0,0x0E,0x70,0x1E,0x78,0x3E,0x3F,0xFE,0x1F,0xEE,0x0F,0x8E,
0x00,0x0C,0x00,0x1C,0x00,0x1C,0x00,0x3C,0x00,0x38,0x60,0xF0,0x7F,0xE0,0x7F,0xC0,
0x3F,0x00,
},

{
/*------------------------------------------------------------------------------
;  源文件 / 文字 : .
;  宽×高（像素）: 16×25
------------------------------------------------------------------------------*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x80,0x03,0xC0,0x03,0xC0,0x01,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,
}
};

// void SetPixelYUV(unsigned char *img, int imgWidth, int imgHeight, int x, int y, int cy, int cu, int cv)
// {
//     int imgSize = imgWidth;
//     memset(img+x*imgWidth+y, cy, sizeof(char));
//     memset(img+imgSize+(x*imgWidth+y)/4, cu, sizeof(char));
//     memset(img+imgSize+(x*imgWidth+y)/4, cv, sizeof(char))
// }

void DrawLine(unsigned char *img, int imgWidth, int imgHeight, int x1, int y1, int x2, int y2, int r, int g, int b, int k)
{

    if(x1<0||x2<0||y1<0||y2<0||x1>=imgWidth||x2>=imgWidth||y1>=imgHeight||y2>>imgHeight){
        printf("error, draw coordiante is out of boundary\n");
    }

    long int imgHeadU = imgWidth*imgHeight;
    long int imgHeadV = imgWidth*imgHeight*5/4;
        ///rgb to yuv
    // Y = 0.299R + 0.587G + 0.114B
    // U = -0.147R - 0.289G + 0.436B
    // V = 0.615R - 0.515G - 0.100B
    int cy = 0.299*r + 0.587*g + 0.114*b;
    int cu = -0.147*r - 0.289*g + 0.436*b;
    int cv = 0.615*r - 0.515*g - 0.1*b;

    int dx = x2 - x1;
    int dy = y2 - y1;
    int ux = ((dx > 0) << 1) - 1;//x的增量方向，取1或-1, >1=1,<1=-1
    int uy = ((dy > 0) << 1) - 1;//y的增量方向，取1或-1
    int x = x1, y = y1, eps;//eps为累加误差

    eps = 0;dx = abs(dx); dy = abs(dy);
    if(dy==0){
        printf("dy=0\n");
        //set draw piexel
        
        memset(&img[y*imgWidth+x], cy, dx);
        memset(&img[(y-1)*imgWidth+x], cy, dx);
        int ycuv = y/2;
        std::cout<<"ycuv "<<ycuv<<std::endl;

        memset(&img[imgHeadU+ycuv*(imgWidth)/2+x/2], cu, dx/2);
        memset(&img[imgHeadV+ycuv*(imgWidth)/2+x/2], cv, dx/2);

        //draw thickness of line
        for (int i = 0; i < k; ++i)
        {
            memset(&img[(y-1-2*i+1)*imgWidth+x], cy, dx);
            memset(&img[(y-1-2*i)*imgWidth+x], cy, dx);
            memset(&img[(y+2*i)*imgWidth+x], cy, dx);
            memset(&img[(y+2*i-1)*imgWidth+x], cy, dx);
            memset(&img[imgHeadU+(ycuv-i)*(imgWidth)/2+x/2], cu, dx/2);
            memset(&img[imgHeadU+(ycuv+i)*(imgWidth)/2+x/2], cu, dx/2);
            memset(&img[imgHeadV+(ycuv-i)*(imgWidth)/2+x/2], cv, dx/2);
            memset(&img[imgHeadV+(ycuv+i)*(imgWidth)/2+x/2], cv, dx/2);
        }


    }else if(dx==0){
        printf("dx=0\n");
        for (y = y1; y != y2+uy; y += uy){
            //set draw piexel
            img[y*imgWidth+x] = cy;
            int ycuv = y/2;
            img[imgHeadU+(ycuv*(imgWidth)/2+x/2)] = cu;
            img[imgHeadV+(ycuv*(imgWidth)/2+x/2)] = cv;
        }
    }else if (dx > dy){
        for (x = x1; x != x2+ux; x += ux){
            //set draw piexel
            img[y*imgWidth+x] = cy;
            int ycuv = y/2;
            img[imgHeadU+(ycuv*(imgWidth)/2+x/2)] = cu;
            img[imgHeadV+(ycuv*(imgWidth)/2+x/2)] = cv;

            eps += dy;
            if ((eps << 1) >= dx){
                y += uy; eps -= dx;
            }
        }
    }else{
        for (y = y1; y != y2+uy; y += uy){
            //set draw piexel
            img[y*imgWidth+x] = cy;
            int ycuv = y/2;
            img[imgHeadU+(ycuv*(imgWidth)/2+x/2)] = cu;
            img[imgHeadV+(ycuv*(imgWidth)/2+x/2)] = cv;

            eps += dx;
            if ((eps << 1) >= dy){
                x += ux; eps -= dy;
            }
        }
    }             
}

void DrawRect(unsigned char *img, int imgWidth, int imgHeight, int topx, int topy, int width, int height, int r, int g, int b)
{
    //draw 4 lines
    DrawLine(img, imgWidth, imgHeight, topx, topy, topx+width, topy, r, g, b, 1);
    DrawLine(img, imgWidth, imgHeight, topx, topy+height, topx+width, topy+height, r, g, b, 1);
    DrawLine(img, imgWidth, imgHeight, topx, topy, topx, topy+height, r, g, b, 1);
    DrawLine(img, imgWidth, imgHeight, topx+width, topy, topx+width, topy+height, r, g, b, 1);
}

void DrawNumber(unsigned char *img, int imgWidth, int imgHeight,const unsigned char* str, RectInfo rect, ColorType color)
{
    if((img == NULL) || (str == NULL))
    {
        return;
    }
    if(((rect.Top.x+rect.Width) > imgWidth) || ((rect.Top.y+rect.High) > imgHeight))
    {
        printf("error, draw coordiante is out of boundary\n");
        return;
    }
    int cy =0,cu =0,cv =0,i,j;
    long int imgHeadU = imgWidth*imgHeight;
    long int imgHeadV = imgWidth*imgHeight*5/4;
    int x = rect.Top.x;
    int y = rect.Top.y;
    switch(color)
    {
        case SET_GREEN:
            cy = 255;
            cu = 0;
            cv = 0;
            break;
        case SET_RED:
            cy = 29;
            cu = 111;
            cv = -26;
            break;
        default:break;

    }
    for(i = 0;i < rect.High;i++)
    {
        int k = 0;
        for(j = 0; j < rect.Width; j++)
        {
            if(((str[i*(rect.Width/8) + j/8] << (j%8)) & 0x80) != 0)
            {
                img[(y+i)*imgWidth+(x+j)] = cy;
                // if(k%2)
                {
                    img[imgHeadU+(y+i)/2*(imgWidth)/2+(x+j)/2] = cu;
                    img[imgHeadV+(y+i)/2*(imgWidth)/2+(x+j)/2] = cv;
                }
                k++;
            }
        }
    }
    return;
}

void PrintNumber(unsigned char *img, int imgWidth, int imgHeight,PointInfo pos,ColorType color,char* string)
{
    int i,len;
    RectInfo rect;
    rect.High = ZIMU_HEIGHT;
    rect.Width = ZIMU_WIDTH;
    rect.Top.x = pos.x;
    rect.Top.y = pos.y;

    if(string == NULL)
    {
        return;
    }

    len = strlen(string);

    for(i = 0;i < len;i++)
    {
        if(string[i] != '.')
        {
            DrawNumber(img,imgWidth,imgHeight,Bmp[string[i]-'0'],rect,color);
        }
        else
        {
            DrawNumber(img,imgWidth,imgHeight,Bmp[10],rect,color);
        }
        rect.Top.x += (ZIMU_WIDTH +2);//设置字间隔
    }
    return;
}




// void DrawNum(unsigned char *img, int imgWidth, int imgHeight, int x, int y, int r, int g, int b, float num)
// {

//     long int imgHeadU = imgWidth*imgHeight;
//     long int imgHeadV = imgWidth*imgHeight*5/4;

//     int cy = 0.299*r + 0.587*g + 0.114*b;
//     int cu = -0.147*r - 0.289*g + 0.436*b;
//     int cv = 0.615*r - 0.515*g - 0.1*b;

//     int matx=2, maty=2;
//     matHead = (y - maty)* imgWidth + x;
//     printf("%d\n", mat[0][0]);
//     for (int j=0; j<maty; j++){
//         for(int i=0; i<matx; i++){
//             img[j*width+matHead+i] = cy;
//             int ycuv = y/2;
//             img[imgHeadU+(ycuv*(imgWidth)/2+x/2)] = cu;
//             img[imgHeadV+(ycuv*(imgWidth)/2+x/2)] = cv;
//         }
//     }
// }

int main(int argc, char* argv[])
{

    /*IplImage *img = cvLoadImage("D:\\workspace\\cv_yun\\DSC_1598.JPG");
    cvNamedWindow("a");
    cvShowImage("a",img);
    cvWaitKey(0);
    */

    FILE *f ;
    if(!(f = fopen("sequence_000.raw","rb")))
    {
        printf("file open error!");
    }


    // calculate the frame num
    fseek(f, 0, SEEK_END);
    int frame_count = 0;
    // long file_size = 0;
    // frame_count = (int) ((int)ftell(f)/((nWidth * nHeight * 3) / 2));  // ftell 用于求文件大小
    printf("frame num is %d \n", frame_count);
    // printf("file length is   %d",ftell(f));


    fseek(f, 0, SEEK_SET);//文件内位置定位到文件头
    //IplImage *img = cvCreateImage(cvSize(352,288),IPL_DEPTH_8U,1);
    //IplImage *grey;
    IplImage *yimg = cvCreateImage(cvSize(nWidth, nHeight),IPL_DEPTH_8U,1);
    //IplImage *uimg = cvCreateImageHeader(cvSize(nWidth/2, nHeight/2),IPL_DEPTH_8U,1);  
    //IplImage *vimg = cvCreateImageHeader(cvSize(nWidth/2, nHeight/2),IPL_DEPTH_8U,1);




    //----------------------------------------------------------------------实现读取一个文件 显示照片
    unsigned char *pBuf = new unsigned char[nWidth*nHeight*3/2];
    fread(pBuf, 1, (nWidth * nHeight * 3) / 2, f);// nWidth*nHeight*3/2  means 本程序使用 yuv:4:2:0 yuv分为 Y Cb Cr 三部分
    //cvSetData(yimg, pBuf, nWidth);


    cv::Mat frameYUV(nHeight*3/2, nWidth, CV_8UC1);
    cv::Mat frameRGB;

    

    frameYUV.data = pBuf;

    cv::cvtColor(frameYUV, frameRGB, CV_YUV2BGR_I420);

    cv::cvtColor(frameRGB, frameYUV, CV_BGR2YUV_I420);

    DrawRect(pBuf, nWidth, nHeight, 50, 50, 50, 50, 255, 0, 255);
    DrawLine(pBuf, nWidth, nHeight, 150, 150, 201, 201, 255, 255, 255, 1);

    PointInfo pos={100,100};
    char num[] = "8.9";
    PrintNumber(pBuf, nWidth, nHeight, pos, SET_RED, num); 
    cv::cvtColor(frameYUV, frameRGB, CV_YUV2BGR_I420);
    // bool YV12ToBGR24_OpenCV(unsigned char* pYUV,unsigned char* pBGR24,int width,int height)
    // {
    //     if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL)
    //         return false;
    //     Mat dst(height,width,CV_8UC3,pBGR24);
    //     Mat src(height + height/2,width,CV_8UC1,pYUV);
    //     cvtColor(src,dst,CV_YUV2BGR_YV12);
    //     return true;
    // }

    cv::imshow("frameRGB", frameRGB);
    cv::waitKey(0);
    // cvNamedWindow("a");
    // cvShowImage("a", yimg);
    // cvWaitKey( 0 );




    //----------------------------------------------------------------------读取yuv文件的y部分 类似于播放器效果 
    /*unsigned char *pBuf = new unsigned char[nWidth*nHeight*3/2];
    int pos = 0;
    for(int i = 0; i<frame_count-1; i++ )
    {
    fseek(f, pos, SEEK_SET);
    fread(pBuf,1 , FrameSize, f );
    cvSetData(yimg, pBuf, nWidth);
    cvNamedWindow("a");
    cvShowImage("a", yimg);
    cvWaitKey( 33 );
    pos += FrameSize;
    }
    */


    // cvDestroyWindow("a");
    cvReleaseImage(&yimg);
    delete []pBuf;

    fclose(f);
    return 0;
}
