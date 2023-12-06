#include "main.hpp"
void ls(char *path)
{
   vector<string> files=listfiles(path);
    for(auto it:files)
    {
        if(it[0]=='.')
        {
            continue;
        }
        cout<<it<<"  ";
    }
    cout<<endl;
}
void ls(string &token,char *path)
{
     if(token=="-a")
     {
      vector<string> files=listfiles(path);
      for(auto file:files)
      {
        cout<<file<<"  ";
      }
      cout<<endl;
     }
     else if(token=="-al" || token=="-la")
     {
        vector<string> files=listfiles(path);
        int total=0;
        for(auto it:files)
        {
            char resultant_path1[PATH_MAX];
            strcpy(resultant_path1,path);
            strcat(resultant_path1,"/");
            strcat(resultant_path1,it.c_str());
            string res1(resultant_path1);
            struct stat filestat;
            char address[100];
            strcpy(address,res1.c_str());
            stat(address,&filestat);
            total+=filestat.st_blocks;
        }
        cout<<"total "<<total/2<<endl;
        for (auto it:files)
        {
            char resultant_path[PATH_MAX];
            strcpy(resultant_path,path);
            strcat(resultant_path,"/");
            strcat(resultant_path,it.c_str());
            string res(resultant_path);
            print_longlist(res,it);
        }
        cout<<endl;
     }
     else if(token==".")
     {
        ls(path);
     }
     else if(token=="..")
     {
        char parent_dir[PATH_MAX];
        strcpy(parent_dir,dirname(path));
        vector<string> files=listfiles(path);
        for(auto it:files)
        {
        if(it[0]=='.')
        {
            continue;
        }
        cout<<it<<"  ";
        }
        cout<<endl;
     }
     else if(token=="-l")
     {
        int total =0;
        vector<string> files=listfiles(path);
        for(auto it:files)
        {
            if(it[0]=='.')
        {
            continue;
        }
        else{
            char resultant_path1[PATH_MAX];
            strcpy(resultant_path1,path);
            strcat(resultant_path1,"/");
            strcat(resultant_path1,it.c_str());
            string res1(resultant_path1);
            struct stat filestat;
            char address[100];
            strcpy(address,res1.c_str());
            stat(address,&filestat);
            total+=filestat.st_blocks;
        }
        }
        cout<<"total "<<total/2<<endl;
        for(auto it:files)
        {
        if(it[0]=='.')
        {
            continue;
        }
        else{
            char resultant_path[PATH_MAX];
            strcpy(resultant_path,path);
            strcat(resultant_path,"/");
            strcat(resultant_path,it.c_str());
            string res(resultant_path);
            print_longlist(res,it);
        }
        }
        cout<<endl;
     }
     else
     {
        char address[100];
        strcpy(address,token.c_str());
        vector<string> files=listfiles(address);
        for(auto it:files)
        {
            if(it[0]!='.')
            cout<<it<<endl;
        }
    }
}
void ls(string &token1,string &token2,char *path)
{
    if((token1=="-l" && token2=="-a") || (token1=="-a" && token2=="-l"))
    {
        string cmd="-la";
        ls(cmd,path);
    }
    else if(token1=="-l" && token2[0]!='-')
    {
        string cmd="-l";
        char address[100];
        strcpy(address,token2.c_str());
        ls(cmd,address);
    }
    else if(token1=="-a" && token2[0]!='-')
    {
        string cmd="-a";
        char address[100];
        strcpy(address,token2.c_str());
        ls(cmd,address);
    }
    else if((token1=="-al" || token1=="-la") && token2[0]!='-')
    {
        string cmd="-al";
        char address[100];
        strcpy(address,token2.c_str());
        ls(cmd,address);
    }
}
vector<string> listfiles(char path[])
{
    
    vector<string> totalfiles;
    DIR *dir;
    //getting the directory format through structure
    struct dirent *entry;
    dir= opendir(path);
    if(dir==NULL)
    {
        // such path doesn't exist
        perror("opendir");
        return {};
    }
    //opening the directory if exists
    if(dir!=NULL);
    {
        //reads the directory till the end
        int len=0;
        while((entry=readdir(dir))!=NULL)
        {
            len+=entry->d_reclen;
            string s=entry->d_name;
            totalfiles.push_back(s);
        }
    }
    closedir(dir);
    return totalfiles;
}
void print_longlist(string &file,string fname)
{
    //total blocks
        
            struct stat fileStat;
            char address[100];
            strcpy(address,file.c_str());
            if((stat(address,&fileStat))==0)
            {
            cout<< ((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
            cout<< ((fileStat.st_mode & S_IRUSR) ? "r" : "-");
            cout<< ((fileStat.st_mode & S_IWUSR) ? "w" : "-");
            cout<< ((fileStat.st_mode & S_IXUSR) ? "x" : "-");
            cout<< ((fileStat.st_mode & S_IRGRP) ? "r" : "-");
            cout<< ((fileStat.st_mode & S_IWGRP) ? "w" : "-");
            cout<< ((fileStat.st_mode & S_IXGRP) ? "x" : "-");
            cout<< ((fileStat.st_mode & S_IROTH) ? "r" : "-");
            cout<< ((fileStat.st_mode & S_IWOTH) ? "w" : "-");
            cout<< ((fileStat.st_mode & S_IXOTH) ? "x" : "-");
            cout<<" ";
            cout<<setw(2)<<fileStat.st_nlink<<" ";
            struct passwd *pw_entry=getpwuid(fileStat.st_uid);
            struct group *gr_entry=getgrgid(fileStat.st_gid);
            // cout<<fileStat.st_uid<<" ";
            // cout<<fileStat.st_gid;
            // cout<<file<<endl;
            // fflush(stdout);
            cout<<setw(10)<<(pw_entry->pw_name)<<" ";
            cout<<setw(10)<<(gr_entry->gr_name)<<"  ";
            cout<<setw(8)<<fileStat.st_size<<" ";
            time_t modified_time=fileStat.st_mtime;
            struct tm *time=localtime(&modified_time);
            vector<string> months={
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
            };
            cout<<months[time->tm_mon]<<" "<<time->tm_mday<<" "<<time->tm_hour<<":"<<time->tm_min<<" ";
            cout<<fname;
            }
            else{
                perror("stat");
                return;
            }
            cout<<endl;
}
