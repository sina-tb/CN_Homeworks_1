#include <iostream>
#include <vector>

using namespace std;
class Reservation{

public:

    int id;
    int numOfBeds;
    string reserveDate;
    string checkoutDate;
    

};
class Room {

public:

    string number;
    int status;
    int price;
    int maxCapacity;
    int capacity;
    vector <Reservation> users;


};

