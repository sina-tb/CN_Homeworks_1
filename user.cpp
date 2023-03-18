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



int main(int argc, char const *argv[]) {
    struct server server;
    decode_server(&server);

    int fd;
    char buff[1024] = {0};
    char serverbuff[1024] = {0};

    fd = connectServer(server);

    while(1)
    {
        recv(fd,serverbuff,1024,0);
        printf("server said:%s\n",serverbuff);
        read(0,buff,1024);
        send(fd, buff, strlen(buff), 0);

    }
    return 0;
}