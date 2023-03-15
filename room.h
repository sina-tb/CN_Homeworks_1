#include <iostream>
#include <vector>

using namespace std;
class User{

public:

    int id;
    int numOfBeds;
    string reserveDate;
    string checkoutDate;
    
    User(int in_id,int in_nob,string rd, string cd);

};
class Room {

public:

    string number;
    int status;
    int price;
    int maxCapacity;
    int capacity;
    vector <User> users;

    Room(string num , int stat, int in_price, int max_cap, int cap, vector <User> usrs);

};

