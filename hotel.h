#include <iostream>
#include <vector>
#include "costumer.h"
#include "room.h"

using namespace std;

class Hotel {       

public:

    vector <Costumer> costumers;
    vector <Room> rooms;            

    Hotel(vector <Costumer> co, vector <Room> ro);
};