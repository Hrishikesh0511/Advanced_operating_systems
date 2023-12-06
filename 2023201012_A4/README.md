# Assignment 4

## Question 1 - Distributed File Sharing System

## Data Structures Used :
### Tracker side data structures : 
#### File Storage :
```c
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

```

## Overview (Approach) 
- Tracker which is guide for every peer will handle all the requests related to commands from peer
- Peer who itself act as client and server.
- Tracker and peer both have tracker_info.txt file which contain tracker IP and port information.
- Peer will run two threads one for serving requests from other peer and one for to do communication with tracker.
- Peer will communicate to tracker and perform all tasks
- When peer want to download a file, it will request different different peer for different different chunks


### Commands and its implementation :
- **Create User Account :**
    - Peer send request to tracker. Tracker will store this information in user data structure and ask peer to go for login. 
- **Login :**
    - Peer must do login to run any of the command. Tracker will check whether client's user id and password matches or not.
    - Tracker will not allow multiple login from one terminal.
- **To run all below command peer must need to do login**
- **Create Group :**
    - Traccker will store the information of group created by peer and make that peer admin.
    - only this peer is allowd to accept new requests to this group.
- **Join Group :**
    - Peer can request to join in any of group. Tracker will store this information on its end.
- **Leave Group :**
    - Tracker will remove user form group data structure.
- **List pending join :**
    - Only admin of the group can run this command.
    - It will show all the peers info who has requested to join the group.
- **Accept Group Joining Request :**
    - Only admin can accept thr request of peer to join group.
    - when admin accept the user, in tracker data structure it will be moved to accepted list.
    - Only after this any peer can contribute or take benifit of that group.
- **List All Group In Network :**
    - Shows the list of all groups.
- **List All sharable Files In Group :**
    - Shows all files shared by that group users
- **Upload File:** 
    - Peer need to be part of this group to upload file in a group.
    - When peer do upload its information will be stored in peer's data structure and tracker will also marks in its data stuctre that this peer is available to share this file.
    - Along with file info peer will also share sha of that file.



## Execution (To run this file)
```shell
Tracker Terminal :
1. cd to tracker
2. g++ tracker.cpp -o tracker
3. ./tracker tracker_info.txt 1

Peer Terminal :
1. cd to client
3. g++ client.cpp -o client
2. ./client <ip_address>:<port_no> tracker_info.txt
```

## Asumptions 
- Tracker is always up
- Whole file will be available in the network
- tracker_info.txt with tracker information should be available to client and tracker
- Data stored on peer and tracker is not persistent,if any of them goes down all the data will be misplaced.