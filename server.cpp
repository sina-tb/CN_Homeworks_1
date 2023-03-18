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

struct client
{
    int fd;
    int rule; 
    int state; 
};

struct server 
{
    int fd;
    string host;
    int port; 
    bool connected;
};

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
    // set connected to true
}



int main(int argc, char const *argv[]) {
    struct server server;
    decode_server(&server);
    int new_socket, max_sd;
    char temp_buff [2048];
    fd_set master_set, working_set;
    if(!server.connected)
        server.fd = setupServer(server.port);
    else 
    {
        cout << "already server connected on this port" << endl;
        return 0;
    }
   
    FD_ZERO(&master_set);
    max_sd = server.fd;
    FD_SET(server.fd, &master_set);

    vector <int> clients;
    cout << "server is running..." << endl;

    while (1) {
        working_set = master_set;
        select(max_sd + 1, &working_set, NULL, NULL, NULL);

        for (int i = 0; i <= max_sd; i++) {
            if (FD_ISSET(i, &working_set)) {
                
                if (i == server.fd) {  // new clinet
                    new_socket = acceptClient(server.fd);
                    FD_SET(new_socket, &master_set);
                    clients.push_back(new_socket);
                    if (new_socket > max_sd)
                        max_sd = new_socket;
                    write(new_socket, "hello", 6);
                    cout << "New client connected(fd =" << new_socket << ")" << endl;
                }
                
                else { // client sending msg
                    int bytes_received = 0;
                    bytes_received = recv(i , temp_buff , sizeof(temp_buff), 0);
                    
                    if (bytes_received == 0) { // EOF
                        printf("client fd = %d closed\n", i);
                        close(i);
                        FD_CLR(i, &master_set);
                        continue;
                    }
                    string buffer(temp_buff);
                    cout << "client" << i << " said:" << buffer;
                    write(new_socket, "hello", 6);
                    buffer = "";
                    memset(temp_buff,0,2048);
                }
            }
        }

    }

    return 0;
}