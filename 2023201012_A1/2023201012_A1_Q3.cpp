#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
// flag values are defined inside this library
#include<iostream>
using namespace std;
bool isReverse(char *new_file_path,char *old_file_path)
{
    int chunksize=10000;
    // <.....................opening files..........................>
    const char* path1=new_file_path;
    const char* path2=old_file_path;
    int new_fd= open(path1,O_RDWR);
    int old_fd=open(path2,O_RDWR);
    //fd is file descriptor that uniquely determines the files
    //the fd for the first file is generally 3 as 0,1,2 are reserved
    if(new_fd==-1 || old_fd==-1)
    {
        perror("opening error");
        exit(1);
    }
    char* buffer1=(char*)calloc(chunksize,sizeof(char));
    //allocating some memory to read the file
    char* buffer2=(char*)calloc(chunksize,sizeof(char));

    loff_t file_size=lseek64(new_fd,0,SEEK_END);
    loff_t cur_pos=0;
    bool flag=true;
    while(cur_pos<file_size-chunksize && file_size>=chunksize)
    {
    cur_pos+=chunksize;
    lseek64(new_fd,-cur_pos,SEEK_END);
    //reading 10000 character from last everytime
    read(new_fd,buffer1,chunksize);
    read(old_fd,buffer2,chunksize);
    for(int i=0;i<chunksize;i++)
    {
        if(buffer1[i]!=buffer2[chunksize-i-1]) 
        {
            flag=false;
            break;
        }
    }
    //again setting the pointer in the position
    lseek64(old_fd,cur_pos,SEEK_SET);
    }
    chunksize=file_size%chunksize;
    if(chunksize!=0)
    {
    cur_pos+=chunksize;
    lseek64(new_fd,-cur_pos,SEEK_END);
    //reading 10000 character from last everytime
    read(new_fd,buffer1,chunksize);
    read(old_fd,buffer2,chunksize);
    for(int i=0;i<chunksize;i++)
    {
        if(buffer1[i]!=buffer2[chunksize-i-1]) 
        {
            flag=false;
            break;
        }
    }
    //again setting the pointer in the position
    lseek64(old_fd,cur_pos,SEEK_SET);
    }
    // // <.....................closing files..............................>

    int new_fd_close=close(new_fd);
    int old_fd_close=close(old_fd);
    if(new_fd_close==-1 || old_fd_close==-1)
    {
        perror("closing error");
        exit(1);
    }
    return flag;
}
int main(int argc,char *argv[])
{
    char *new_file_path=argv[1];
    char *old_file_path=argv[2];
    char *dir_path=argv[3];
    struct stat old_file;
    struct stat dir;
    struct stat new_file;
    stat(old_file_path,&old_file);
    stat(dir_path,&dir);
    stat(new_file_path,&new_file);
    int oldper=old_file.st_mode;
    int dirper=dir.st_mode;
    int newper=new_file.st_mode;
    //..........................for new file.........................................
    cout<<"Directory is created: "<<((S_ISDIR(dirper))?"Yes":"No")<<endl;
    cout<<"Whether file contents are reversed in newfile: "<<((isReverse(new_file_path,old_file_path))?"Yes":"No")<<endl;
    cout<<"User has read permissions on newfile: "<<((newper & S_IRUSR)?"Yes":"No")<<endl;
    cout<<"User has write permission on newfile: "<<((newper & S_IWUSR)?"Yes":"No")<<endl;
    cout<<"User has execute permission on newfile: "<<((newper & S_IXUSR)?"Yes":"No")<<endl;
    cout<<"Group has read permissions on newfile: "<<((newper & S_IRGRP)?"Yes":"No")<<endl;
    cout<<"Group has write permission on newfile: "<<((newper & S_IWGRP)?"Yes":"No")<<endl;
    cout<<"Group has execute permission on newfile: "<<((newper & S_IXGRP)?"Yes":"No")<<endl;
    cout<<"Others has read permissions on newfile: "<<((newper & S_IROTH)?"Yes":"No")<<endl;
    cout<<"Others has write permission on newfile: "<<((newper & S_IWOTH)?"Yes":"No")<<endl;
    cout<<"Others has execute permission on newfile: "<<((newper & S_IXOTH)?"Yes":"No")<<endl;
    // ..................for old file...........................................................
    cout<<"User has read permissions on oldfile: "<<((oldper & S_IRUSR)?"Yes":"No")<<endl;
    cout<<"User has write permission on oldfile: "<<((oldper & S_IWUSR)?"Yes":"No")<<endl;
    cout<<"User has execute permission on oldfile: "<<((oldper & S_IXUSR)?"Yes":"No")<<endl;
    cout<<"Group has read permissions on oldfile: "<<((oldper & S_IRGRP)?"Yes":"No")<<endl;
    cout<<"Group has write permission on oldfile: "<<((oldper & S_IWGRP)?"Yes":"No")<<endl;
    cout<<"Group has execute permission on oldfile: "<<((oldper & S_IXGRP)?"Yes":"No")<<endl;
    cout<<"Others has read permissions on oldfile: "<<((oldper & S_IROTH)?"Yes":"No")<<endl;
    cout<<"Others has write permission on oldfile: "<<((oldper & S_IWOTH)?"Yes":"No")<<endl;
    cout<<"Others has execute permission on oldfile: "<<((oldper & S_IXOTH)?"Yes":"No")<<endl;
    //...............................for directory.............................................
    cout<<"User has read permissions on directory: "<<((dirper & S_IRUSR)?"Yes":"No")<<endl;
    cout<<"User has write permission on directory: "<<((dirper & S_IWUSR)?"Yes":"No")<<endl;
    cout<<"User has execute permission on directory: "<<((dirper & S_IXUSR)?"Yes":"No")<<endl;
    cout<<"Group has read permissions on directory: "<<((dirper & S_IRGRP)?"Yes":"No")<<endl;
    cout<<"Group has write permission on directory: "<<((dirper & S_IWGRP)?"Yes":"No")<<endl;
    cout<<"Group has execute permission on directory: "<<((dirper & S_IXGRP)?"Yes":"No")<<endl;
    cout<<"Others has read permissions on directory: "<<((dirper & S_IROTH)?"Yes":"No")<<endl;
    cout<<"Others has write permission on directory: "<<((dirper & S_IWOTH)?"Yes":"No")<<endl;
    cout<<"Others has execute permission on directory: "<<((dirper & S_IXOTH)?"Yes":"No")<<endl;
}