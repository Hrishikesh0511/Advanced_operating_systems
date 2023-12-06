#pragma once
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string>
#include<sstream>
#include<dirent.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<libgen.h>
#include<iomanip>
#include<fcntl.h>
#include<sys/wait.h>
#include<algorithm>

using namespace std;
void change_dir(string &token,string &home_address,string &prev_address);
void display_address(string &home_address);
void display_user();
void echo(vector<string> &words);
void pwd(string &home_address);
void ls(char *path);
void ls(string &token,char *path);
void ls(string &token1,string &token2,char *path);
vector<string> listfiles(char path[]);
void print_longlist(string &file,string fname);
bool search(string &filename,char path[]);
void pinfo(string &token);
void redirection(string &cmd);
void execute(string cmd);
void showhistory(vector<string> &hist,int num);
void add_to_history(string &cmd,vector<string> &hist);
void load_from_file(vector<string> &hist);
void load_to_file(vector<string> &hist);
