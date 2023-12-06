#include "main.hpp"

void pwd(string &home_address)
{
    char path[100];
    string absolute_address=getcwd(path,100);
    cout<<absolute_address;
    // stringstream buffer;
    // streambuf* oldstdout=cout.rdbuf();
    // cout.rdbuf(buffer.rdbuf());
    // display_address(home_address);
    // cout.rdbuf(oldstdout);
    // string op=buffer.str();
    // string output=home_address+op.substr(1,op.size()-3);
    cout<<endl;
    //in c/cpp any i/o is not directly performed instead it is first stored in some buffer and then it is performed 
    //when it is required.. So now we use this to our advantage 
}