#include "main.hpp"

void display_user()
{
    char *user_name=getlogin();
    char host_name[_SC_HOST_NAME_MAX];
    gethostname(host_name,_SC_HOST_NAME_MAX);
    printf("%s@%s:",user_name,host_name);
}
void display_address(string &home_address)
{
    char path[100];
    string absolute_address=getcwd(path,100);
    int indx=absolute_address.find(home_address);
    string relative_address;
    if(absolute_address.size()<home_address.size())
    {
        cout<<absolute_address<<"> ";
    }
    else{
    relative_address="~";
    relative_address+=absolute_address;
    relative_address.erase(indx+1,home_address.size());
    cout<<relative_address<<"> ";
    }
}