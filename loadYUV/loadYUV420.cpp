    #include <opencv/highgui.h>
    #include <stdio.h>



    //#include <>

    #define nWidth 352
    #define nHeight 240
    #define FrameSize nWidth*nHeight*3/2
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
    long file_size = 0;
    frame_count = (int) ((int)ftell(f)/((nWidth * nHeight * 3) / 2));  // ftell 用于求文件大小
    printf("frame num is %d \n", frame_count);
    printf("file length is   %d",ftell(f));


    fseek(f, 0, SEEK_SET);//文件内位置定位到文件头
    //IplImage *img = cvCreateImage(cvSize(352,288),IPL_DEPTH_8U,1);
    //IplImage *grey;
    IplImage *yimg = cvCreateImage(cvSize(nWidth, nHeight),IPL_DEPTH_8U,1);
    //IplImage *uimg = cvCreateImageHeader(cvSize(nWidth/2, nHeight/2),IPL_DEPTH_8U,1);  
    //IplImage *vimg = cvCreateImageHeader(cvSize(nWidth/2, nHeight/2),IPL_DEPTH_8U,1);


    //----------------------------------------------------------------------实现读取一个文件 显示照片
    unsigned char *pBuf = new unsigned char[nWidth*nHeight*3/2];
    fread(pBuf, 1, (nWidth * nHeight * 3) / 2, f);// nWidth*nHeight*3/2  means 本程序使用 yuv:4:2:0 yuv分为 Y Cb Cr 三部分
    cvSetData(yimg, pBuf, nWidth);

    cvNamedWindow("a");
    cvShowImage("a", yimg);
    cvWaitKey( 0 );


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


    cvDestroyWindow("a");
    cvReleaseImage(&yimg);
    delete []pBuf;

    fclose(f);
    return 0;
}
