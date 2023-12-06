#include "main.hpp"
vector<string> hist;
int main()
{
    load_from_file(hist);
    char home[100];
    string home_address=getcwd(home,100);
    display_user();
    display_address(home_address);
    string prev_address="";
    while(true)
    {

    string cmds="";
    getline(cin,cmds);
    if(cmds=="") 
    {
        display_user();
        display_address(home_address);
        continue;
    }
    add_to_history(cmds,hist);
    char* tokent=strtok(cmds.data(),";");
    vector<string>  tokenst;
    while(tokent!=NULL)
    {
        tokenst.push_back(tokent);
        tokent=strtok(NULL,";");
    }
    for(auto cmd:tokenst)
    {
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
        display_user();
        display_address(home_address);
        command="";
        continue;
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
        display_user();
        display_address(home_address);
    }
        else if(tokens[0]=="echo")
        {
            vector<string> words;
            for(int i=1;i<tokens.size();i++)
            {
                words.push_back(tokens[i]);
            }
            echo(words);
            display_user();
            display_address(home_address);
        }
        else if(tokens[0]=="pwd")
        {
            pwd(home_address);
            display_user();
            display_address(home_address);
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
            display_user();
            display_address(home_address);
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
            display_user();
            display_address(home_address);
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
            if(pid>0)
            {
                cout<<"["<<pid<<"]"<<endl;
                // exit(EXIT_SUCCESS);
            }
                setsid();
                if(pid==0)
                {
                //it is a child process
                execvp(args[0],args.data());
                perror("execvp");
                }
            display_user();
            display_address(home_address);
        }
        else if(tokens[0]=="pinfo")
        {
            if(tokens.size()==1)
            {
                pid_t pid_shell=getpid();
                tokens.push_back(to_string(pid_shell));
            }
            pinfo(tokens[1]);
            display_user();
            display_address(home_address);
        }
        else if(tokens[0]=="history")
        {
            if(tokens.size()==1)
            showhistory(hist,10);
            if(tokens.size()==2)
            showhistory(hist,stoi(tokens[1]));
            display_user();
            display_address(home_address);
        }
        else if(tokens[0]=="exit")
        {
            break;
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
            display_user();
            display_address(home_address);
        }
    }
    if(tokenst.back()=="exit")
    {
        load_to_file(hist); 
        break;
    }
    }
     return 0;
    }