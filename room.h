#include <iostream>
#include <vector>

using namespace std;
class Reservation{

public:

    int id;
    int numOfBeds;
    string reserveDate;
    string checkoutDate;
    
    Reservation(int in_id,int in_nob,string rd, string cd);

};
class Room {

public:

    string number;
    int status;
    int price;
    int maxCapacity;
    int capacity;
    vector <Reservation> users;

    Room(string num , int stat, int in_price, int max_cap, int cap, vector <Reservation> usrs);

};

