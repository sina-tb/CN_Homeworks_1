#include <iostream>

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

    User(int in_id,string in_name,string in_password,bool in_admin,int in_purse,string in_pn,string in_adr);

};