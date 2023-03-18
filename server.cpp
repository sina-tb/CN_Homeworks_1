#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "server.hpp"
#include <jsoncpp/json/writer.h>

using namespace std;

int setupServer(int port) {
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    
    listen(server_fd, 10);

    return server_fd;
}

int acceptClient(int server_fd) {
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);
    client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*) &address_len);

    return client_fd;
}

bool decode_server(struct server* server)
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
    bool status = root["connected"].asBool();
    if(status)
    {
        cout << "this port is already taken try another one" << endl;
        root["connected"] = true;
        return false;
    }
    server->host = root["hostName"].asString();
    server->port = root["commandChannelPort"].asInt();
    return true;
}


// void blocking_port()
// {
//     string filename = "jsons/config.json";
//     ifstream file(filename);
//     Json::Value event;
//     event["connected"] = true;
//     cout << event << endl;
// }

