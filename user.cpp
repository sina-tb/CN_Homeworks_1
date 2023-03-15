#include <iostream>
#include "user.h"

using namespace std;

class User {

public:
        
    int id;
    string name;
    string password;
    bool admin;
    int purse;
    string phoneNumber;
    string address;
    
    User(int in_id,string in_name,string in_password,bool in_admin,int in_purse,string in_pn,string in_adr)
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