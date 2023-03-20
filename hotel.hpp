#include <iostream>
#include <vector>
#include "user.hpp"
#include "room.hpp"

using namespace std;


class Hotel {       

public:

    vector <User> users;
    vector <Room> rooms;
    vector <User> pre_user;          

    Hotel(vector <User> usr, vector <Room> ro,vector <User> pre);

    void decode_users();
    void decode_rooms();
    void decode_server();
    void rooms_information(bool admin,int fd);
    void users_information(int fd);
    int signup_signin_handler(string input,int fd);
    int find_pre_user(int fd);
    int find_user(int fd);
    bool signin(string username , string password,int fd);
    int signup(string buffer,int fd);
    bool check_user_signup(string username);
    void users_information();
    void reservation_page(int fd);
    int handle_reservation_page(string buffer, int fd);
    void logout(int fd);
    void logged_user_information(int fd);
    int edit_information(string buffer,int fd);
    int room_handler(string buffer,int fd);
    bool add_room(string room_number, int max_capacity,int price);
    bool modify_room(string room_number,int max_capacity,int price);
    bool remove_room(string room_number);
    int leave_room(string input_str,int fd);
    void procces();
    int admin_leave_room(string buffer,int fd);
    int cancel_reserve(string buffer, int fd);
    int show_reserve(int fd);
    void encode_users();
    void encode_rooms();
    void start_print(int fd);
    void empty_rooms_information(bool admin,int fd);
    int room_information_handler(string buffer,int fd);
    
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
