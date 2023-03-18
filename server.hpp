#include<iostream>
#include<vector>
#include<string.h>

using namespace std;

struct server 
{
    int fd;
    string host;
    int port; 
};

int setupServer(int port);
int acceptClient(int server_fd);
bool decode_server(struct server* server);

