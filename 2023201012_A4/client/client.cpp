#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <pthread.h>
#include <vector>
#include <openssl/sha.h>
#include <algorithm>
#include <sys/stat.h>
#include <cmath>
using namespace std;
#define MAX_BUFFER_SIZE 512
#define CHUNK_SIZE 512*1024
#define SERV_BACKLOG 100

unordered_map<string,string> file_path;         //<<file_name>,<file_path>>
class chunk_info{
    public:
    string file_name;
    string num_of_chunks;
    string file_path;
    vector<bool> chunks;
};
unordered_map<string,chunk_info> cli_chunkDetails;       //<<SHA>,<chunk_info>>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}
vector<string> tokenize(char cli_msg[]){
    char *token = strtok(cli_msg, " "); 
    vector<string> res;
    while (token != NULL) {
        res.push_back(token);
        token = strtok(NULL, " "); 
    }
    return res;
}
string findFileName(string &filepath){
    int indx=filepath.find_last_of('/');
    if(indx!=string::npos){
        string filename=filepath.substr(indx+1);
        return filename;
    }
    else{
        return filepath;
    }
}
int findFileSize(string &filepath){
    int fd=open(filepath.c_str(),O_RDONLY);
    struct stat file_info;
    if(fstat(fd,&file_info)==-1){
        error("Couldn't get file size");
    }
    long long int file_size = file_info.st_size;
    return file_size;
}
string Hash(string &file){
    const char *file_path=file.c_str();
    int fd=open(file_path,O_RDONLY);
    if(fd<0){
        return "";
    }
    SHA_CTX sha1Context;
    SHA1_Init(&sha1Context);

    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        SHA1_Update(&sha1Context, buffer, bytesRead);
    }

    unsigned char result[SHA_DIGEST_LENGTH];
    SHA1_Final(result, &sha1Context);

    close(fd);

    char hashString[41]; 

    for (int i = 0; i < 20; i++) {
        snprintf(hashString + (i * 2), 3, "%02x", result[i]);
    }

    hashString[40] = '\0';

    return hashString;
    close(fd);

}
unordered_map<int,vector<string>> chunk_details;     
void send_file(int cli_socket,string &filepath){
    int fd=open(filepath.c_str(),O_RDONLY);
    if(fd<0) cout<<"error!opening file"<<endl;
    char buffer[CHUNK_SIZE];
    int bytes_sent=0;
    while(1){
        bzero(buffer,CHUNK_SIZE);
        int read_count=read(fd,buffer,CHUNK_SIZE);
        if(read_count<=0) break;
        bytes_sent+=send(cli_socket,buffer,read_count,0);
        if(bytes_sent<=0) {
            break;
        }
    }
    cout<<"BytesSent: "<<bytes_sent<<endl;
}
void recieve_file(int cli_socket,string &destination_path,string &filename,unordered_map<int,vector<string> > &chunk_to_port){
    string res_file=destination_path+"/"+filename;
    int fd=open(res_file.c_str(),O_RDWR|O_CREAT|O_TRUNC,0777);
    char buffer[CHUNK_SIZE];
    int bytes_written=0;
    cout<<"downloading.."<<endl;
    while(1){
        bzero(buffer,CHUNK_SIZE);
        int bytes_recieved=recv(cli_socket,buffer,CHUNK_SIZE,0);
        if(bytes_recieved<=0) break;
        bytes_written+=write(fd,buffer,bytes_recieved);
        cout<<"Bytes Written: "<<bytes_written<<endl;
        cout<<"Download Completed!!"<<endl;
    }
}
void *handle_sending(void *cli_sockfd){
    int cli_socket=*(static_cast<int*>(cli_sockfd));
    char peer_msg[1024];
    bzero(&peer_msg,1024);
    recv(cli_socket, peer_msg, 1024,0);
    vector<string> msgs=tokenize(peer_msg);
    if(msgs[0]=="send_file"){
        string filepath=file_path[msgs[1]];
        send_file(cli_socket,filepath);
    }
    close(cli_socket);
    return nullptr;
}
void *handle_peers(void *port){
    //acts as a server
    int cli_port=*(static_cast<int*>(port));
    int serv_sockfd, cli_sockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;

    // socket()
    serv_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sockfd < 0) {
        error("failed to open socket");
    }

    // server address structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(cli_port);

    // Bind ip and port
    if (bind(serv_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("failed binding socket");
    }

    if(listen(serv_sockfd, SERV_BACKLOG)<0){
        error("failed to listen");
    }

    while (1)
    {
        cli_len = sizeof(cli_addr);
        //cli_sockfd will consists connected client socket file_descriptor
        cli_sockfd = accept(serv_sockfd, (struct sockaddr *)&cli_addr, &cli_len);
        if(cli_sockfd<0){
              error("failed to connect");
        }
        pthread_t t;
        pthread_create(&t,NULL,handle_sending,&cli_sockfd);
    }
    return nullptr;

}
int main(int argc, char *argv[])
{
    
    // Create socket
    int cli_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_sockfd < 0)
    {
        error("failed to open socket");
    }

    //getting client port and ip info
    string arguments=argv[1];
    int indx=arguments.find(':');
    string cli_ip=arguments.substr(0,indx);
    int cli_port = stoi(arguments.substr(indx+1));

    //It also acts as a server for peer thread that keeps on listening for any requests
    pthread_t t;
    pthread_create(&t,NULL,handle_peers,&cli_port);

    //getting tracker port and ip info
    char* tracker_info_path=argv[2];
    int fd_tracker_info=open(tracker_info_path,O_RDONLY);
    if(fd_tracker_info<0) error("Couldn't open tracker_info.txt");
    char buffer[100];
    int bytes_read=read(fd_tracker_info,buffer,100);
    string tracker_info(buffer,bytes_read);
    indx=tracker_info.find(':');
    int tracker_port = stoi(tracker_info.substr(indx+1));
    close(fd_tracker_info);

    // server address structure
    struct sockaddr_in serv_addr;
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(tracker_port);   
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // // Connect to the server
    if (connect(cli_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("failed connecting to server");
    }

    char serv_response[1024];
    while(1){
        string s;
        getline(cin,s);         //read the input from the client
        string req=s;
        vector<string> cmds=tokenize(&s[0]); 
        if(cmds[0]=="login"){
            //add cli_ip, cli_port to it
            req+=(" "+(cli_ip)+" "+to_string(cli_port));
        }
        else if(cmds[0]=="upload_file"){
            string h=Hash(cmds[1]);
            if(h.size()==0){
                cout<<"Invalid File Path! please recheck the file path:)"<<endl;
                continue;
            }
            string filename=findFileName(cmds[1]);
            file_path[filename]=cmds[1];
            long long int fileSize=findFileSize(cmds[1]);
            int number_of_chunks=ceil(fileSize/CHUNK_SIZE);
            req+=(cmds[0]+" "+filename+" "+cmds[2]+" "+h+" "+to_string(number_of_chunks));
            cli_chunkDetails[h].file_name=filename;
            cli_chunkDetails[h].num_of_chunks=number_of_chunks;
            cli_chunkDetails[h].file_path=cmds[1];
            for(int i=0;i<number_of_chunks;i++){
            cli_chunkDetails[h].chunks.push_back(true);
            }
        }
        send(cli_sockfd,req.c_str(),1024,0);
        bzero(serv_response,1024);
        recv(cli_sockfd,serv_response,1024,0);
        if(cmds[0]=="download_file"){
            if(serv_response=="") {
                cout<<"Currently there are no users to send the file!!"<<endl;
            }
            else{
                if(serv_response!="Please login First!!" && serv_response!="Such group doesn't Exists!!" && serv_response!="You are not a member of this group" && serv_response!="No such file exists in the group" && serv_response!="download_file <group_id> <file_name> <destination_path>")
                {
                vector<string> port_nums=tokenize(serv_response);
                unordered_map<int,vector<string>> chunk_to_port;        //<<chunk_num>,<port>>
                int num_of_chunks=stoi(port_nums[port_nums.size()-1]);
                for(int i=0;i<port_nums.size()-1;i++){
                    for(int j=0;j<num_of_chunks;j++){
                    chunk_to_port[j].push_back(port_nums[i]);
                    }
                    int port_num=stoi(port_nums[i]);
                    struct sockaddr_in serv_addr;
                    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
                    if (sockfd < 0) {
                        error("failed to open socket");
                    }

                    // server address structure
                    bzero((char *)&serv_addr, sizeof(serv_addr));
                    serv_addr.sin_family = AF_INET;
                    serv_addr.sin_addr.s_addr = INADDR_ANY;
                    serv_addr.sin_port = htons(port_num);   

                    int connect_res=connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
                    if(connect_res!=-1){
                        string msg="send_file "+cmds[2];
                        send(sockfd,msg.c_str(),1024,0);
                        recieve_file(sockfd,cmds[3],cmds[2],chunk_to_port);
                        bzero(serv_response,sizeof(serv_response));
                    }
                }
            }
            }
        }
        cout<<"tracker: "<<serv_response<<endl;
    }
    return 0;
}
