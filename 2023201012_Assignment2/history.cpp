#include "main.hpp"
void add_to_history(string &cmd,vector<string> &hist)
{ 
    if(hist.size()==20)
    {
        hist.erase(hist.begin());
    }
    hist.push_back(cmd);
}
void load_from_file(vector<string> &hist)
{
    int fd=open("history.txt",O_RDONLY);
    char buffer[300];
    ssize_t bytes_read;
    string current_line;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            char current_char = buffer[i];
            if (current_char == '\n') {
                hist.push_back(current_line);
                current_line.clear();
            } else {
                current_line += current_char;
            }
        }
    }
    close(fd);
}
void showhistory(vector<string> & hist,int num)
{
    int displaySize=hist.size()-num;
    int startIndex=max(0,displaySize);
    for(int i=startIndex;i<hist.size();i++)
    {
        cout<<hist[i]<<endl;
    }
}
void load_to_file(vector<string> &hist)
{
    int fd=open("history.txt",O_WRONLY|O_TRUNC);
    for(auto line:hist)
    {
        write(fd,line.c_str(),line.size());
        write(fd, "\n", 1);
    }
    close(fd);
}