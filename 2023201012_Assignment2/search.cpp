#include "main.hpp"
bool search(string &file_name,char path[])
{
    
    vector<string> list=listfiles(path);
    for(auto it:list)
    {
        char resultant_path[PATH_MAX];
        if(it!="." && it!="..")
        {
            strcpy(resultant_path,path);
            strcat(resultant_path,"/");
            strcat(resultant_path,it.c_str());
        }
        // cout<<resultant_path<<endl;
        struct stat fileStat;
        if(stat(resultant_path,&fileStat)==-1)
        {
            // cout<<"error,getting file info"<<resultant_path<<endl;
            continue;
        }
        if(S_ISDIR(fileStat.st_mode))
        {
            //if folder also should be returned true then
            if(file_name==it) return true;
            if(search(file_name,resultant_path))
            return true;
        }
        else
        {
            if(file_name==it)
            return true;
        }
    }
    return false;
}