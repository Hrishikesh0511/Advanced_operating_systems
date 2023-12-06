#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
// flag values are defined inside this library
#include<iostream>
using namespace std;
int main(int argc,char *argv[])
{
    int chunksize=10000;
    // <.....................opening files..........................>
    const char* path1=argv[1];
    int read_fd= open(path1,O_RDONLY);
    //fd is file descriptor that uniquely determines the files
    //the fd for the first file is generally 3 as 0,1,2 are reserved
    if(read_fd==-1)
    {
        perror("opening error");
        exit(1);
    }
    char* buffer=(char*)calloc(chunksize,sizeof(char));
    //allocating some memory to read the file
    mkdir("write_file_q1", 	
00700);
//this gives all permissions to the user
    //making a directory called write_file
    const char* path2="write_file_q1/b.txt";
    int write_fd=open(path2,O_RDWR|O_CREAT);
    //so it basically creates a file at the specified position if it doesnot exist
    chmod(path2,00600);
    // this gives write permissions
    loff_t file_size=lseek64(read_fd,0,SEEK_END);
    loff_t cur_pos=0;
    while(cur_pos<=file_size-chunksize && file_size>=chunksize)
    {
    cur_pos+=chunksize;
    lseek64(read_fd,-cur_pos,SEEK_END);
    //reading 10000 character from last everytime
    read(read_fd,buffer,chunksize);
    int start=0,end=chunksize-1;
    while(start<end)
    {
        swap(buffer[start++],buffer[end--]);
    }
    write(write_fd,buffer,chunksize);
    //again setting the pointer in the position
    lseek64(write_fd,cur_pos,SEEK_SET);
    cout<<"\r";
    cout<<((double)cur_pos/(double)file_size)*100.0<<"%";
    cout.flush();
    }
    char *newbuffer=(char*)calloc(chunksize,sizeof(char));
    //remaining ones will be 
    chunksize=file_size%chunksize;
    cur_pos+=chunksize;
    lseek64(read_fd,-cur_pos,SEEK_END);
    //reading 10000 character from last everytime
    read(read_fd,newbuffer,chunksize);
    int start=0,end=chunksize-1;
    while(start<=end)
    {
        swap(newbuffer[start++],newbuffer[end--]);
    }
    write(write_fd,newbuffer,chunksize);
    //again setting the pointer in the position
    lseek64(write_fd,cur_pos,SEEK_SET);
    cout<<"\r";
    cout<<((double)(cur_pos)/(double)file_size)*100.0<<"%";
    cout.flush();
    // // <.....................closing files..............................>

    int read_fd_close=close(read_fd);
    int write_fd_close=close(write_fd);
    if(read_fd_close==-1 ||write_fd_close==-1)
    {
        perror("closing error");
        exit(1);
    }
    return 0;
}