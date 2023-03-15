#include <iostream>
#include <vector>
#include "room.h"

using namespace std;
class Reservation{

public:

    int id;
    int numOfBeds;
    string reserveDate;
    string checkoutDate;
    
    Reservation(int in_id,int in_nob,string rd, string cd)
    {
        id = in_id;
        numOfBeds = in_nob;
        reserveDate = rd;
        checkoutDate = cd;
    }

};
class Room {

public:

    string number;
    int status;
    int price;
    int maxCapacity;
    int capacity;
    vector <Reservation> users;

    Room(string num , int stat, int in_price, int max_cap, int cap, vector <Reservation> usrs)
    {  
        number = num;
        status = stat;
        price = in_price;
        maxCapacity = max_cap;
        capacity = cap;
        users = usrs;
    }

};
