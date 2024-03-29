#include <iostream>
#include "user.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <string.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <sstream>
#include<sys/stat.h>
using namespace std;

struct server 
{
    int fd;
    string host;
    int port; 
    bool connected;
};

int connectServer(struct server server) {
    int fd;
    struct sockaddr_in server_address;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(server.port); 
    server_address.sin_addr.s_addr = inet_addr(server.host.c_str());

    if (connect(fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { // checking for errors
        printf("Error in connecting to server\n");
    }

    return fd;
}


void decode_server(struct server* server)
{
    string filename = "jsons/config.json";
    ifstream file(filename);
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( file, root );
    if ( !parsingSuccessful )
    {
        cout << "Error parsing the string" << endl;
    }
    server->connected = root["connected"].asBool();
    server->host = root["hostName"].asString();
    server->port = root["commandChannelPort"].asInt();
}

vector <string> string_split(string input_str)
{ 
    vector <string> words;
    string str;
    stringstream ss(input_str); 
    while (getline(ss, str, ' ')) 
        words.push_back(str);
    return words;
}

int sign_user(int fd,string & username)
{
    char serverbuff [1024];
    char buff[1024];
    memset(buff,0,1024);
    read(0,buff,1024);
    string temp = buff;
    temp.pop_back();
    send(fd,buff, strlen(buff) - 1,0);
    recv(fd,serverbuff,1024,0);
    vector <string> input(string_split(temp));
    string s_buff = serverbuff;
    cout << s_buff;
    vector <string> sr_b(string_split(s_buff));  //
    if(input[0] == "signin")
    {
        if(sr_b[0] == "230:")
        {
            username = input[1];
            return 1;
        }
    }
    else if (input[0] == "signup")
    {
        if(sr_b[0] == "311:")
        {
            string data;
            char userdata[1024];
            for(int i = 0;i < 4; i++)
            {
                cin >> temp; 
                if(i == 0)
                    data = temp;
                else
                    data = data + " " + temp;
            }
            for(int i = 0; i < data.size();i++)
                userdata[i] = data[i];
            send(fd,userdata,data.size(),0);
            memset(serverbuff,0,1024);
            recv(fd,serverbuff,1024,0);
            string s_buff(serverbuff);
            cout << s_buff << endl;
            vector <string> sr_b(string_split(s_buff));
            if(sr_b[0] == "231:")
            {
                username = input[1];
                return 1;
            }
        }
    }
    return 0;
}

int edit_info(int fd,bool admin,fstream& userLog)
{
    string data, temp;
    char userdata[1024];
    if(admin)
        cin >> data;
    else     
        for(int i = 0;i < 3; i++)
        {
            cin >> temp; 
            if(i == 0)
                data = temp;
            else
                data = data + " " + temp;
        }
    for(int i = 0; i < data.size();i++)
        userdata[i] = data[i];
    send(fd,userdata,data.size(),0);
    userLog << data << endl;
    char serverbuff[1024];
    recv(fd,serverbuff,1024,0);
    cout << serverbuff << endl;
    userLog << serverbuff;
    return 1;
}



int main(int argc, char const *argv[]) {
    struct server server;
    decode_server(&server);
    int state = -1;
    bool admin;
    string username;

    int fd;
    char buff[1024] = {0};
    char serverbuff[1024] = {0};
    char menubuff[8192] = {0};

    fstream userLog;
    mkdir("usersLog",0777);

    fd = connectServer(server);

    while(1)
    {
        memset(buff,0,1024);
        memset(serverbuff,0,1024);
        memset(menubuff,0,1024);
        if(state == -1)
        {   
            recv(fd,serverbuff,1024,0);
            cout << serverbuff;
            state = 0;
            continue;
        }
        else if(state == 0)
        {
            state = sign_user(fd,username);
            if(state == 1)
            {
                string filename = "usersLog/" + username + ".txt";
                userLog.open(filename,fstream::out| fstream::app);
            }
            continue;
        }
        else if(state == 1)
        {
            recv(fd,serverbuff,1024,0); //recive menu
            cout << serverbuff;
            read(0,buff,1024);
            send(fd, buff, strlen(buff) - 1, 0);
            userLog << buff;
            recv(fd,menubuff,8192,0);
            cout << menubuff ;
            userLog << menubuff;
            if(buff[0] == '0' && strlen(buff) == 2)
                return 0;
            else if(buff[0] == '3' && strlen(buff) == 2)
            {
                state = 3;
            }
            else if(buff[0] == '5' && strlen(buff) == 2)
            {
                if(menubuff[0] != '1')
                    state = 5;
            }
            else if(buff[0] == '7' && strlen(buff) == 2)
            {
                if(strlen(menubuff) == 31)
                    admin = true;
                else 
                    admin = false; 
                state = 7;
            }
            else if(buff[0] == '8' && strlen(buff) == 2)
                state = 8;
            else if(buff[0] == '9' && strlen(buff) == 2)
            {
                if(menubuff[0] != '4')
                    state = 9;
            }
        }
        else if(state == 3)
        {
            read(0,buff,1024);
            send(fd, buff, strlen(buff) - 1, 0);
            userLog << buff;
            recv(fd,menubuff,8192,0);
            cout << menubuff ;
            userLog << menubuff;
            state = 1;
        }
        else if(state == 5)
        {
            read(0,buff,1024);
            send(fd, buff, strlen(buff) - 1, 0);
            userLog << buff;
            recv(fd,menubuff,8192,0);
            cout << menubuff ;
            userLog << menubuff;
            state = 1;
        }
        else if(state == 7)
        {
            edit_info(fd, admin, userLog);
            state = 1;
        }
        else if(state == 8)
        {
            read(0,buff,1024);
            send(fd, buff, strlen(buff) - 1, 0);
            userLog << buff;
            recv(fd,menubuff,8192,0);
            cout << menubuff ;
            userLog << menubuff;
            state = 1;
        }
        else if(state == 9)
        {
            read(0,buff,1024);
            send(fd, buff, strlen(buff) - 1, 0);
            userLog << buff;
            recv(fd,menubuff,8192,0);
            cout << menubuff ;
            userLog << menubuff;
            state = 1;
        }
    }
    return 0;
}