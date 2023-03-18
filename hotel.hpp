#include <iostream>
#include <vector>
#include "user.hpp"
#include "room.hpp"

using namespace std;


class Hotel {       

public:

    vector <User> users;
    vector <Room> rooms;            

    Hotel(vector <User> usr, vector <Room> ro);
    void decode_users();
    void decode_rooms();
    void decode_server();
    void rooms_information(bool admin);
    void users_information();
    int signup_signin_handler(string input);
    void start_print(int fd);
    bool signin(string username , string password);
    bool signup(string username,User* logged_user);
    bool check_user_signup(string username);
    void reservation_page();
    int handle_reservation_page(vector <string> command,User *logged_user);
    void logout();
    void logged_user_information(User *logged_user);
    bool edit_information(User *logged_user);
    void room_handler(vector <string> commands);
    bool add_room(string room_number, int max_capacity,int price);
    bool modify_room(string room_number,int max_capacity,int price);
    bool remove_room(string room_number);
    void leave_room(vector <string> commands ,User *logged_user);
    void procces();
    
};

struct server 
{
    int fd;
    string host;
    int port; 
};

int setupServer(int port);
int acceptClient(int server_fd);
bool decode_server(struct server* server);
