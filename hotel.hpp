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
    void rooms_information(bool admin);
    void users_information();
    bool signup_signin_handler(vector <string> words,User *logged_user);
    void start_print();
    bool signin(string username , string password,User *logged_user);
    bool signup(string username,User* logged_user);
    bool check_user_signup(string username);
    void reservation_page();
    int handle_reservation_page(vector <string> command,User *logged_user);
    void logout();
    void logged_user_information(User *logged_user);
    bool edit_information(User *logged_user);
};