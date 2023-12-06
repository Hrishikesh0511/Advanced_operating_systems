#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <unordered_map>
#include<algorithm>
#define SERV_BACKLOG 5
using namespace std;
//;;;;;;;password issue to handle;;;;;;;;;;;;;;
unordered_map<string, string> registered_users;     //<<user_id>,<password>>
unordered_map<string, string> user_socket_info;     //<<user_id>,<Ip:Port>>
unordered_map<string,bool> loggedIn;                //<<user_id>,<status>>
class group_info{
    public:
    string admin;
    vector<string> group_members;
    vector<string> pending_requests;
    vector<string> files;
};
class file_info{
    public:
    string hash;
    int num_of_chunks;
    vector<string> users;
};
unordered_map<string,group_info> groups;       //<<group_id>,<group_info>>
unordered_map<string,file_info> files;         //<<file_name>,<file_info>>


void error(const char *msg) {
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
string handle_register(vector<string> &args){
    string response;
    if(args.size()<3) response="create_user <user_id> <passwd>";
    else
    if(registered_users.find(args[1])!=registered_users.end()){
                response="User Already Exists!! Please Login :)";
            }
            else{
                registered_users[args[1]]=args[2];
                response="Registered Succesfully!!";
            }
            return response;
}
string handle_login(vector<string> &args,string &user_id){
    string response;
    if(args.size()<3) response="login <user_id> <passwd>";
    else
     if(registered_users.find(args[1])==registered_users.end()){
                response="Invalid UserID (or) Password, Try Registering :)";
            }
            else if(registered_users[args[1]]==args[2]){
                if(loggedIn[args[1]]){
                    response="Already Logged In";
                }
                else{
                response="Logged In Succesfully !!";
                loggedIn[args[1]]=true;
                user_id=args[1];
                user_socket_info[user_id]=args[4];  //port_number
                }
            }
            return response;
}
string handle_create_group(vector<string> args,string &user_id){
    string response;
    if(args.size()<2) response="create_group <group_id>";
    else
        if(groups.find(args[1])!=groups.end()){
            response="Group already exists!!";
        }
        else {
            groups[args[1]].group_members.push_back(user_id);
            groups[args[1]].admin=user_id;
            response="Group created succesfully!!";
        }
    return response;
}
string handle_join_group(vector<string> args,string &user_id){
    string response;
    if(args.size()<2) response="join_group <group_id>";
    else
    if(groups.find(args[1])==groups.end()){
        response="Such group doesn't Exists!!";
    }
    else{
        bool ingrp=false;
        bool inreq=false;
        for(auto it:groups[args[1]].group_members){
            if(it==user_id){
                ingrp=true;
                break;
            }
        }
        if(ingrp){
             response="You are already in the group";
        }
        else{
            for(auto it:groups[args[1]].pending_requests){
                if(it==user_id){
                    inreq=true;
                    break;
                }
            }
            if(inreq){
                response="You have already made a request";
            }
            else{
                groups[args[1]].pending_requests.push_back(user_id);
                response="group join request sent successfully!!";
            }
        }
    }
    return response;
}
string handle_leave_group(vector<string> args,string &user_id){
    string response;
    if(args.size()<2) response="leave_group <group_id>";
    else
    if(groups.find(args[1])==groups.end()){
        response="Such group doesn't Exists!!";
    }
    else{
         bool ingrp=false;
        for(auto it:groups[args[1]].group_members){
            if(it==user_id){
                ingrp=true;
                break;
            }
        }
        if(!ingrp){
             response="You are not a member of this group";
        }
        else if(groups[args[1]].group_members.size()==1){
            groups.erase(args[1]);
            response="As you were the only member of group,Deleting the group also!!";
        }
        else{
            auto indx=find(groups[args[1]].pending_requests.begin(),groups[args[1]].pending_requests.end(),args[2]);
            groups[args[1]].group_members.erase(indx);
            response="Succesfully Left the group!!";
        }
    }
    return response;
}
string handle_list_requests(vector<string> &args,string &user_id){
    string response;
    if(args.size()<2) response="list_requests <group_id>";
    else
    if(groups.find(args[1])==groups.end()){
        response="Such group doesn't Exists!!";
    }
    else{
        if(user_id!=groups[args[1]].admin){
            response="You are not owner of the group!!";
        }
        else if(groups[args[1]].pending_requests.size()==0){
            response="Currently, there are no requests to show!!";
        }
        else{
            for(auto it:groups[args[1]].pending_requests){
                response+=("\n "+it);
            }
        }
    }
    return response;
}
string handle_accept_requests(vector<string> &args,string &user_id){
    string response;
    if(args.size()<3) response="accept_request <group_id> <user_id>";
    else
    if(groups.find(args[1])==groups.end()){
        response="Such group doesn't Exists!!";
    }
    else{
        auto indx=find(groups[args[1]].pending_requests.begin(),groups[args[1]].pending_requests.end(),args[2]);
        if(user_id!=groups[args[1]].admin){
            response="You are not owner of the group!!";
        }
        else if(indx==groups[args[1]].pending_requests.end()){
            response="There is no such request with the UserID!!";
        }
        else {
            groups[args[1]].pending_requests.erase(indx);
            groups[args[1]].group_members.push_back(args[2]);
            response="Request Accepted!!";
        }
    }
    return response;
}
string handle_list_all_groups(){
    string response="List of all groups";
    for(auto it:groups){
        response+=("\n "+it.first);
    }
    return response;
}
string handle_list_files(vector<string> &args,string &user_id){
    string response="";
    if(args.size()<2) response="list_files <group_id>";
    else
    if(groups.find(args[1])==groups.end()){
        response="Such group doesn't Exists!!";
    }
    else{
    bool ingrp=false;
        for(auto it:groups[args[1]].group_members){
            if(it==user_id){
                ingrp=true;
                break;
            }
        }
        if(!ingrp){
             response="You are not a member of this group";
        }
        else if(groups[args[1]].files.size()==0){
            response="Currently there are no files to show!!";
        }
        else {
            for(auto it:groups[args[1]].files){
                response+=("\n "+it);
            }
        }
    }
    return response;
}
string handle_upload_file(vector<string> &args,string &user_id){
    string response="";
    if(args.size()<3) response="upload_file <file_path> <group_id>";
    else
    {
    string file_name=args[1];
    string hash=args[3];
    string grpID=args[2].substr(0,args[2].size()-11);
    cout<<grpID<<endl;
    if(groups.find(grpID)==groups.end()){
        response="Such group doesn't Exists!!";
    }
    else{
    bool ingrp=false;
        for(auto it:groups[grpID].group_members){
            if(it==user_id){
                ingrp=true;
                break;
            }
        }
        if(!ingrp){
             response="You are not a member of this group";
        }
        else {
            groups[grpID].files.push_back(file_name);
            files[file_name].hash=hash;
            // files[file_name].num_of_chunks=stoi(args[4]);
            files[file_name].users.push_back(user_id);
            response="File has been uploaded succesfully!!";
        }
    }
    }
    return response;
}
string handle_download_file(vector<string> &args,string &user_id){
        string grp_id=args[1];
        string file_name=args[2];
        string response="";
        if(args.size()<4) response="download_file <group_id> <file_name> <destination_path>";
        else
        if(groups.find(grp_id)==groups.end()){
            response="Such group doesn't Exists!!";
        }
        else{
            bool ingrp=false;
        for(auto it:groups[args[1]].group_members){
            if(it==user_id){
                ingrp=true;
                break;
            }
        }
        if(!ingrp){
             response="You are not a member of this group";
        }
        else{
            bool fileExists=false;
        for(auto it:groups[grp_id].files){
            if(file_name==it){
                fileExists=true;
            }
    }
            if(!fileExists){
                response="No such file exists in the group";
            }
            else{
                for(auto user:files[file_name].users){
                    if(loggedIn[user]){
                        response+=(user_socket_info[user]+" ");
                    }
                }
                response+=to_string((files[file_name].num_of_chunks));
                }
        }
        }
        return response;
}
string handle_logout(vector<string> &args,string &user_id){
        string response="";
        user_id = "";
        loggedIn[user_id] =false;
        response = "Logged out!!";
        return response;
}
void *handle_clients(void* cli_sockfd){
    int cli_socket=*(static_cast<int*>(cli_sockfd));
    char cli_msg[1024];
    string user_id="";
    while(1){
        bzero(&cli_msg, 1024);
        if(recv(cli_socket, cli_msg,1024,0)<=0){
            cout<<"didn't recieve data!"<<endl;
            break;
        }
        string response;
        vector<string> args=tokenize(cli_msg);        
        if(args[0]=="create_user")
        {
            response=handle_register(args);
        }
        else if(args[0]=="login"){
           response=handle_login(args,user_id);
        }
        else if(!loggedIn[user_id] || user_id.size()==0){
            response="Please login First!!";
        }
        else if(args[0]=="create_group"){
            response=handle_create_group(args,user_id);
        }
        else if(args[0]=="join_group" ){
            response=handle_join_group(args,user_id);
        }
        else if(args[0]=="leave_group" ){
            response=handle_leave_group(args,user_id);
        }
        else if(args[0]=="list_requests" ){
            response=handle_list_requests(args,user_id);
        }
        else if(args[0]=="accept_request" ){
            response=handle_accept_requests(args,user_id);
        }
        else if(args[0]=="list_groups" ){
            response=handle_list_all_groups();
        }
        else if(args[0]=="list_files"){
            response=handle_list_files(args,user_id);
        }
        else if(args[0]=="upload_file"){
            response=handle_upload_file(args,user_id);
        }
        else if(args[0]=="download_file"){
            response=handle_download_file(args,user_id);
        }
        else if(args[0]=="logout"){
            response=handle_logout(args,user_id);
        }
        send(cli_socket,response.data(),1024,0);
    }
    close(cli_socket);
    return nullptr;
}

int main(int argc, char *argv[]) {
    

    int serv_sockfd, cli_sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;
    int file_size,indx;

    //getting tracker port and ip info
    char* tracker_info_path=argv[1];
    int fd_tracker_info=open(tracker_info_path,O_RDONLY);
    if(fd_tracker_info<0) error("Couldn't open tracker_info.txt");
    char buffer[100];
    int bytes_read=read(fd_tracker_info,buffer,100);
    string tracker_info(buffer,bytes_read);
    indx=tracker_info.find(':');
    int tracker_port = stoi(tracker_info.substr(indx+1));
    close(fd_tracker_info);

    // socket()
    serv_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sockfd < 0) {
        error("failed to open socket");
    }

    // server address structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(tracker_port);

    int enable = 1;
    if (setsockopt(serv_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    perror("setsockopt(SO_REUSEADDR) failed");
    // Handle the error
    }

    // Bind socket
    if (bind(serv_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("failed binding socket");
    }

    // Listen for connections
    if(listen(serv_sockfd, SERV_BACKLOG)<0){
        error("failed to listen");
    }

    
    while (1)
    {
        cout << "Waiting for connections..." << endl;

        cli_len = sizeof(cli_addr);
        cli_sockfd = accept(serv_sockfd, (struct sockaddr *)&cli_addr, &cli_len);
        if(cli_sockfd<0){
              error("failed to connect");
        }

        cout << "Connected!" << endl;
        pthread_t t;
        pthread_create(&t, NULL, handle_clients, &cli_sockfd);
    }
    close(serv_sockfd);
    return 0;
}
