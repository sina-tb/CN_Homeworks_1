#include <iostream>
#include "costumer.h"

using namespace std;

class Costumer {

public:
        
    int id;
    string name;
    string password;
    bool admin;
    string purse;
    string phoneNumber;
    string address;
    
    Costumer(int in_id,string in_name,string in_password,bool in_admin,string in_purse,string in_pn,string in_adr)
    {
        id = in_id;
        name = in_name;
        password = in_password;
        admin = in_admin;
        purse = in_purse;
        phoneNumber = in_pn;
        address = in_adr;
    }

};