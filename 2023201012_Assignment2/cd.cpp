#include "main.hpp"

void change_dir(string &token,string &home_address,string &prev_address)
{
    char curr[100];
    
    if(chdir(token.c_str())==-1)
    {
        if(token=="~")
        {
            chdir(home_address.c_str());
            prev_address=home_address;
        }
        else if(token=="")
        {
            chdir(home_address.c_str());
            prev_address=home_address;
        }
        else if(token=="-")
        {
            if(prev_address=="")
            {
                prev_address=home_address;
                cout<<"bash: cd: OLDPWD not set"<<endl;
            }
            else{
                getcwd(curr,100);
                chdir(prev_address.c_str());
                cout<<prev_address<<endl;
                prev_address=curr;
            }
        //need to implement
        }
        else{
        cout<<"bash: cd: No such file or directory"<<endl;
        }
        return;
    }
    else{
    prev_address=token; 
    }
    
}
