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
    void decode_users(string filename);
    void decode_rooms(string filename);
    void print_rooms();
    void print_users();
};