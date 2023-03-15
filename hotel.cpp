#include <iostream>
#include <vector>
#include "hotel.h"
#include "user.h"
#include "room.h"

using namespace std;

class Hotel {       

public:

    vector <User> costumers;
    vector <Room> rooms;            

    Hotel(vector <User> co, vector <Room> ro)
    {
        costumers = co;
        rooms = ro;
    }        
};