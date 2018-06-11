#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char **argv)
{
    DIR *dirp;
    struct dirent *dirt;
    char dir_name[100];
    int file_count=1;      //file_count为计数.

    if (argc != 2){
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
            memset(dir_name, 0, sizeof(dir_name));
            ///strcpy(dir_name, dirt->d_name);
            strcpy(dir_name, "yuv");
            ///sprintf(dir_name + strlen(dirt->d_name), "_%d", file_count++);
            sprintf(dir_name + strlen(dir_name), "_%d", file_count++);///write : pointer of dir_name+strlen of dir_name, then number
            if (rename(dirt->d_name, dir_name) < 0){  //dir_name 为新的命名
                printf("rename failed\n");
                return 0;
            }
        }
    }

    return 0;
}