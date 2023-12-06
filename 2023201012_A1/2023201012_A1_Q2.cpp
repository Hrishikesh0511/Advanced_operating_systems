#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
// flag values are defined inside this library
#include<iostream>
#define chunksize 100000
using namespace std;
int main(int argc,char *argv[])
{
    // <.....................opening files..........................>
    const char* path1=argv[1];
    int read_fd= open(path1,O_RDWR);
    //fd is file descriptor that uniquely determines the files
    //the fd for the first file is generally 3 as 0,1,2 are reserved
    if(read_fd==-1)
    {
        perror("opening error");
        exit(1);
    }
    char* buffer=(char*)calloc(chunksize,sizeof(char));
    //alloacting some memory to read the file
    mkdir("write_file_q2",00700);
    //making a directory called write_file
    const char* path2="write_file_q2/b.txt";
    int write_fd=open(path2,O_RDWR|O_CREAT);
    //so it basically creates a file at the specified position if it doesnot exist
    chmod(path2,00600);
    //it gives all the permissions to the user
    loff_t file_size=lseek64(read_fd,0,SEEK_END);
    loff_t cur_pos_write=0;
    loff_t cur_pos_read;
    int indx1,indx2;
    cin>>indx1>>indx2;
    if(indx1>indx2 || indx1<0 || indx2>=file_size) 
    {
        cout<<"Enter valid indices."<<endl;
        exit(1);
    }

    char*small_buffer=(char*)calloc(1,sizeof(char));
    cur_pos_read=file_size-indx1;

    // <................till indx1....................>

    if(indx1>=chunksize)
    {
    while(cur_pos_write+chunksize<indx1)
    {
        cur_pos_read+=chunksize;
        lseek64(read_fd,-cur_pos_read,SEEK_END);
        read(read_fd,buffer,chunksize*sizeof(char));
        int start=0,end=chunksize-1;
        while(start<end)
        {
            swap(buffer[start++],buffer[end--]);
        }
        lseek64(write_fd,cur_pos_write,SEEK_SET);
        cur_pos_write+=chunksize;
        write(write_fd,buffer,chunksize*sizeof(char));
        cout<<"\r";
        cout<<((double)cur_pos_write/(double)file_size)*100.0<<"%";
        cout.flush();
    }
    }
    else{
    while(cur_pos_write<indx1)
    {
    cur_pos_read++;
    lseek64(read_fd,-cur_pos_read,SEEK_END);
    read(read_fd,small_buffer,sizeof(char));
    lseek64(write_fd,cur_pos_write,SEEK_SET);
    cur_pos_write++;
    write(write_fd,small_buffer,sizeof(char));
    cout<<"\r";
        cout<<((double)cur_pos_write/(double)file_size)*100.0<<"%";
        cout.flush();
    }
    }

    //remaining ones
    for(long long int i=cur_pos_write;i<=indx1;i++)
    {
        lseek64(read_fd,-(file_size-indx1+i),SEEK_END);
        read(read_fd,small_buffer,sizeof(char));
        write(write_fd,small_buffer,sizeof(char));  
        int temp=lseek64(write_fd,(i),SEEK_SET);
        cout<<"\r";
        cout<<((double)i/(double)file_size)*100.0<<"%";
        cout.flush();
    }
//  <......................from indx1 to indx2.................>
    long long int i;
    if(indx2-indx1+1>=chunksize)
    { 
        for(i=indx1;i+chunksize<=indx2;i+=chunksize)
        {
        lseek64(read_fd,i,SEEK_SET);
        read(read_fd,buffer,chunksize*sizeof(char));
        lseek64(write_fd,i,SEEK_SET);
        write(write_fd,buffer,chunksize*sizeof(char));
        cout<<"\r";
        cout<<((double)cur_pos_write/(double)file_size)*100.0<<"%";
        cout.flush();
        }
    }

    else{
    for(long long int i=indx1;i<=indx2;i++)
    {
        lseek64(read_fd,i,SEEK_SET);
        read(read_fd,small_buffer,sizeof(char));
        lseek64(write_fd,i,SEEK_SET);
        write(write_fd,small_buffer,sizeof(char));
        cout<<"\r";
        cout<<((double)(i+1)/(double)file_size)*100.0<<"%";
        cout.flush();
    }
    }
    for(long long int j=i;j<=indx2;j++)
    {
        lseek64(read_fd,j,SEEK_SET);
        read(read_fd,small_buffer,sizeof(char));
        lseek64(write_fd,j,SEEK_SET);
        write(write_fd,small_buffer,sizeof(char));
        cout<<"\r";
        cout<<((double)(j+1)/(double)file_size)*100.0<<"%";
        cout.flush();
    }
//  <..........................indx2 to EOF...................>
    cur_pos_read=0;
    cur_pos_write=indx2+1;
    if(file_size-indx2>=chunksize)
    {
    while(cur_pos_write+chunksize<file_size)
    {
        cur_pos_read+=chunksize;
        lseek64(read_fd,-cur_pos_read,SEEK_END);
        read(read_fd,buffer,chunksize*sizeof(char));
        int start=0,end=chunksize-1;
        while(start<end)
        {
            swap(buffer[start++],buffer[end--]);
        }
        lseek64(write_fd,cur_pos_write,SEEK_SET);
        cur_pos_write+=chunksize;
        write(write_fd,buffer,chunksize*sizeof(char));
        cout<<"\r";
        cout<<((double)cur_pos_write/(double)file_size)*100.0<<"%";
        cout.flush();
    }
    }

    else{
    while(cur_pos_write<file_size)
    {
    cur_pos_read++;
    lseek64(read_fd,-cur_pos_read,SEEK_END);
    read(read_fd,small_buffer,sizeof(char));
    lseek64(write_fd,cur_pos_write,SEEK_SET);
    cur_pos_write++;
    write(write_fd,small_buffer,sizeof(char));
    cout<<"\r";
    cout<<((double)cur_pos_write/(double)file_size)*100.0<<"%";
    cout.flush();
    }
    }
    for(long long int i=cur_pos_write;i<file_size;i++)
    {
    lseek64(read_fd,-(i-indx2),SEEK_END);
    read(read_fd,small_buffer,sizeof(char));
    lseek64(write_fd,i,SEEK_SET);
    write(write_fd,small_buffer,sizeof(char));
    cout<<"\r";
    cout<<((double)(i+1)/(double)file_size)*100.0<<"%";
    cout.flush();
    }
// // <.....................closing files..............................>

    int read_fd_close=close(read_fd);
    int write_fd_close=close(write_fd);
    if(read_fd_close==-1 || write_fd_close==-1)
    {
        perror("closing error");
        exit(1);
    }
    return 0;
}