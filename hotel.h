#include <iostream>
#include <vector>
#include "user.h"
#include "room.h"

using namespace std;

class Hotel {       

public:

    vector <User> costumers;
    vector <Room> rooms;            

    Hotel(vector <User> co, vector <Room> ro);
};