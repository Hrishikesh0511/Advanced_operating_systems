#include "main.hpp"
void pinfo(string &token)
{
    string line;
    string procPath="/proc/"+token+"/status";
    char exePath[4096]; 
    string exeLinkPath="/proc/"+token+"/exe";
    int fd=open(procPath.c_str(),O_RDONLY);
    if (fd==-1) {
        cerr << "Process with PID " <<token << " does not exist." <<endl;
        return;
    }
    // Read and print process information
    vector<string> info;
    char buffer[300];
    ssize_t bytes_read;
    string current_line;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            char current_char = buffer[i];
            if (current_char == '\n') {
                info.push_back(current_line);
                current_line.clear();
            } else {
                current_line += current_char;
            }
        }
    }
    close(fd);
    int terminal_fd = open("/dev/tty", O_RDONLY);
    pid_t current_pgid = getpgid(stoi(token));
   // process group of terminal
    pid_t terminal_pgid = tcgetpgrp(terminal_fd);
    close(terminal_fd);
    cout<<"Pid -- "<<token<<endl;
    for(auto it:info)
    {
        if(it.compare(0,6,"State:")==0)
        {
            string stats=it.substr(7,1);
            if(current_pgid==terminal_pgid)
            {
                stats=stats+"+";
            }
            cout<<"Process Status -- "<<stats<<endl;
        }
        if(it.compare(0,7,"VmSize:")==0)
        {
            cout<<"Memory -- "<<it.substr(8)<<endl;
        }
    }
    ssize_t len = readlink(exeLinkPath.c_str(), exePath, sizeof(exePath) - 1);

    if (len != -1)
    {
        exePath[len] = '\0';
        cout<<"Executable Path -- "<<exePath <<endl;
    } 
    else 
    {
        perror("readlink");
        return;
    }
}