#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <io.h>

using namespace std;

void reNameFiles(string path)
{
    //文件句柄
    long   hFile   =   0;
    //文件信息
    struct _finddata_t fileinfo;
    static int i=1;
    string p;
    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
    {
        do
        {
            //如果是目录,迭代之

            if((fileinfo.attrib &  _A_SUBDIR))
            {
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
                    getFiles( p.assign(path).append("\\").append(fileinfo.name));
            }
            //如果不是,重命名
            else
            {
                //cout<<fileinfo.name<<endl;
                stringstream ss;
                ss<<i;
                i++;
                string new_name = p.assign(path).append("\\")+string(ss.str()+".jpg");
                //cout<<new_name<<endl;
                rename(p.assign(path).append("\\").append(fileinfo.name).c_str(), new_name.c_str());
            }
        }
        while(_findnext(hFile, &fileinfo)  == 0);
        _findclose(hFile);
    }
}
int main()
{
    string path;
    cin >> path;
    reNameFiles(path);
    return 0;
}
