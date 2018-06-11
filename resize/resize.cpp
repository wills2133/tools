#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

//usage  ./test.exe dirname height width

int main(int argc, char **argv)
{
    DIR *dirp;
    struct dirent *dirt;
    char file_name[50];
    int file_count=1;      //file_count为计数.
    cv::Mat img;

    printf("resize file in %s to %dx%d\n", argv[1], atoi(argv[2]),  atoi(argv[3]));
    // printf("argument %d\n", argc);

    if (argc < 4){
        printf("argument miss\n");
        return 0;
    }

    if ((dirp = opendir(argv[1])) ==NULL){
        printf ("open dir %s failed\n",argv[1]);
        return 0;
    }
    
    chdir(argv[1]);

    while ( (dirt = readdir(dirp)) != NULL){
        if ((strcmp(dirt->d_name,"..") !=0) && (strcmp(dirt->d_name,".")!=0)){

            img = cv::imread(dirt->d_name);
            if(!img.data){
                printf("not a img file!\n");
                exit(0);
            }
            cv::resize(img, img, cv::Size(atoi(argv[2]), atoi(argv[3])), 0, 0);
            cv::imwrite(dirt->d_name, img);

            // memset(file_name, 0, sizeof(file_name));
            // ///strcpy(file_name, dirt->d_name);
            // ///sprintf(file_name + strlen(dirt->d_name), "_%d", file_count++);
            // sprintf(file_name, "(%d)", file_count++);///write : pointer of dir_name+strlen of dir_name, then number
            // if (rename(dirt->d_name, file_name) < 0){  //file_name 为新的命名
            //     printf("rename failed\n");
            //     return 0;
            // }
        }
    }
    printf("resize done!\n");
    return 0;
}