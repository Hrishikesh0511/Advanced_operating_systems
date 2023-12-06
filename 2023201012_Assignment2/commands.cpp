#include "main.hpp"
void execute(string cmd)
{
    char home[100];
    string home_address=getcwd(home,100);
    string prev_address="";
    char* token=strtok(cmd.data()," ");
    vector<string>  tokens;
    string command="";
    while(token!=NULL)
    {
        tokens.push_back(token);
        command=command+" "+token;
        token=strtok(NULL," ");
    }
    if((command.find("<")!=string::npos)||(command.find(">")!=string::npos)||(command.find(">>")!=string::npos))
    {
        redirection(command);
        
        
        command="";
        return;
    }
    if(tokens[0]=="cd")
    {
        if(tokens.size()>2)
        {
            cout<<"Too many arguments"<<endl;
        }
        else if(tokens.size()==1)
        {
            //for handling the case with no arguments
            string cmd="";
            change_dir(cmd,home_address,prev_address);
        }
        else{
        change_dir(tokens[1],home_address,prev_address);
        }
        
        
        }
        else if(tokens[0]=="echo")
        {
            vector<string> words;
            for(int i=1;i<tokens.size();i++)
            {
                words.push_back(tokens[i]);
            }
            echo(words);
            
            
        }
        else if(tokens[0]=="pwd")
        {
            pwd(home_address);
            
            
        }
        else if(tokens[0]=="ls")
        {
            char path[100];
            getcwd(path,100);
            if(tokens.size()==1)
            {
            ls(path);
            }
            else if(tokens.size()==2){
            ls(tokens[1],path);
            }
            else if(tokens.size()>2)
            {
                if(tokens.size()==3)
                ls(tokens[1],tokens[2],path);
                if(tokens.size()==4)
                {
                    if((tokens[1]=="-l" && tokens[2]=="-a" && tokens[3][0]!='-')||(tokens[1]=="-a"&&tokens[2]=="-l" && tokens[3][0]!='-'))
                    {
                        string cmd="-al";
                        ls(cmd,tokens[3],path);
                    }
                }
            }
            
            
        }
        else if(tokens[0]=="search")
        {
            char path[100];
            getcwd(path,100);
            if(search(tokens[1],path))
            {
                cout<<"True"<<endl;
            }
            else {
                cout<<"False"<<endl;
            }
            
            
        }
        else if(!tokens.empty() && tokens.back()=="&")
        {
            //this implies that it should run in a background process
            //so we will use execvp to execute it 
            tokens.pop_back();
            vector<char*> args;
            for (const string& token : tokens) {
                args.push_back(const_cast<char*>(token.c_str()));
                }
            args.push_back(nullptr);
            //creating a child to execute excvp cmd
            pid_t pid=fork();
            if(pid==-1)
            {
                perror("fork");
            }
            else if(pid==0)
            {
                //it is a child process
                execvp(args[0],args.data());
                perror("execvp");
                exit(1);
            }
            else{
                cout<<"["<<pid<<"]"<<endl;
            }
            
            
        }
        else if(tokens[0]=="pinfo")
        {
            if(tokens.size()==1)
            {
                pid_t pid_shell=getpid();
                tokens.push_back(to_string(pid_shell));
            }
            pinfo(tokens[1]);
            
            
        }
        else if(tokens[0]=="exit")
        {
            return;
        }
        else{
            vector<char*> args;
            for (const string& token : tokens) {
                args.push_back(const_cast<char*>(token.c_str()));
                }
            args.push_back(nullptr);
            //creating a child to execute excvp cmd
            pid_t pid=fork();
            if(pid==-1)
            {
                perror("fork");
            }
            else if(pid==0)
            {
                //it is a child process
                execvp(args[0],args.data());
                perror("execvp");
                exit(1);
            }
            else{
            int status;
            waitpid(pid, &status, 0);
            }
            
            
        }
}