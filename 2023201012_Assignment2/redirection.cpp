#include "main.hpp"
void redirection(string &cmd)
{
    char* token=strtok(cmd.data()," ");
    vector<string>  tokens;
    while(token!=NULL)
    {
    tokens.push_back(token);
    token=strtok(NULL," ");
    }
    string command="";
    int i;
    for(i=0;i<tokens.size();i++)
    {
    if(tokens[i]=="<" || tokens[i]==">" || tokens[i]==">>")
    {
    int out=dup(1);
    int in=dup(0);
    if(tokens[i]==">")
    {
    if(i+1==tokens.size()) continue;
    string fname=tokens[i+1];
    int fd=open(fname.c_str(),O_WRONLY|O_CREAT|O_TRUNC);
    chmod(fname.c_str(),00644);
    dup2(fd,1);
    execute(command);
    close(fd);
    dup2(out,1);
    }
    if(tokens[i]=="<")
    {
        if(i+1==tokens.size()) continue;
        string fname=tokens[i+1];
        int fd=open(fname.c_str(),O_RDONLY);
        if(fd==-1) 
        {
        cout<<"No file exists"<<endl;
        break;
        }
        dup2(fd,0);
        execute(command);
        close(fd);
        dup2(in,0);
    }
    if(tokens[i]==">>")
    {
        if(i+1==tokens.size()) continue;
        string fname=tokens[i+1];
        int fd=open(fname.c_str(),O_WRONLY|O_CREAT|O_APPEND);
        dup2(fd,1);
        execute(command);
        close(fd);
        dup2(out,1);
    }
    }
    else{
    command=command+" "+tokens[i];
    }
}
}