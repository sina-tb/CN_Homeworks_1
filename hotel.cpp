#include <iostream>
#include <fstream>
#include <vector>
#include "hotel.hpp"
#include "Error.cpp"
#include <jsoncpp/json/json.h>
#include <string.h>
#include <bits/stdc++.h> 

using namespace std;

vector <string> string_split(string input_str)
{ 
    vector <string> words;
    string str;
    stringstream ss(input_str); 
    while (getline(ss, str, ' ')) 
        words.push_back(str);
    return words;
}

Hotel::Hotel(vector <User> usr, vector <Room> ro)
{
    users = usr;
    rooms = ro;
}

void Hotel::start_print()
{
    cout<<"Welcome"<<endl<<"Please signup or signin to hotel first"<<endl;
}
void Hotel::decode_users()
{
    string filename = "jsons/UsersInfo.json";
    ifstream file(filename);
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( file, root );
    if ( !parsingSuccessful )
    {
        cout << "Error parsing the string" << endl;
    }

    const Json::Value elements = root["users"];
    for ( int index = 0; index < elements.size(); ++index )
    {
        User new_user;
        new_user.id = elements[index]["id"].asInt();
        new_user.name = elements[index]["user"].asString();
        new_user.password  = elements[index]["password"].asString();
        new_user.admin = elements[index]["admin"].asBool();
        if( new_user.admin == false)
        {
            new_user.purse = elements[index]["purse"].asInt();
            new_user.phoneNumber = elements[index]["phoneNumber"].asString();
            new_user.address = elements[index]["address"].asString();
        }

        users.push_back(new_user);
    }
}
void Hotel::decode_rooms()
{
    string filename = "jsons/RoomsInfo.json";
    ifstream file(filename);
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( file, root );
    if ( !parsingSuccessful )
    {
        cout << "Error parsing the string" << endl;
    }

    const Json::Value elements = root["rooms"];

    for ( int index = 0; index < elements.size(); ++index )
    {
        Room new_room;
        new_room.number = elements[index]["number"].asString();
        new_room.status = elements[index]["status"].asInt();
        new_room.price = elements[index]["price"].asInt();
        new_room.maxCapacity = elements[index]["maxCapacity"].asInt();
        new_room.capacity = elements[index]["capacity"].asInt();
        const Json::Value user_in_room = elements[index]["users"];
        for ( int i = 0; i < user_in_room.size(); ++i)
        {
            Reservation new_reservation;
            new_reservation.id = user_in_room[i]["id"].asInt();
            new_reservation.numOfBeds = user_in_room[i]["numOfBeds"].asInt();
            new_reservation.reserveDate = user_in_room[i]["reserveDate"].asString();
            new_reservation.checkoutDate = user_in_room[i]["checkoutDate"].asString();
            new_room.users.push_back(new_reservation);
        }
        rooms.push_back(new_room);
    }
}
void Hotel::rooms_information(bool admin)
{
    for ( int i = 0 ; i < rooms.size() ; i++ )
    {
        cout<<endl;
        cout<< "number : " << rooms[i].number<<endl;
        cout<< "status : " << rooms[i].status<<endl;
        cout<< "price : " << rooms[i].price<<endl;
        cout<< "maxCapacity : " << rooms[i].maxCapacity<<endl;
        cout<< "capacity : " << rooms[i].capacity<<endl;
        if ( admin == true)
        {
            for ( int j = 0; j< rooms[i].users.size(); j++)
            {
                cout<<endl;
                cout<< "   id : " << rooms[i].users[j].id<<endl;
                cout<< "   numbOfBeds : " << rooms[i].users[j].numOfBeds<<endl;
                cout<< "   reserveDate : " << rooms[i].users[j].reserveDate<<endl;
                cout<< "   checkoutDate : " << rooms[i].users[j].checkoutDate<<endl;
            }
        }
    }
}  
void Hotel::users_information()
{
    for ( int i = 0 ; i < users.size() ; i++)
    {
        cout<< endl;
        cout<< "id : " << users[i].id<<endl;
        cout<< "name : " << users[i].name<<endl;
        cout<< "password : " << users[i].password<<endl;
        cout<< "admin : " << users[i].admin<<endl;
        if ( users[i].admin == false)
        {
            cout<< "purse : " << users[i].purse<<endl;
            cout<< "phoneNumber : " << users[i].phoneNumber<<endl;
            cout<< "address : " << users[i].address<<endl;
        }
    }
}
bool Hotel::check_user_signup(string username)
{
    for ( int index = 0 ; index < users.size() ; index ++ )
    {
        if ( users[index].name == username)
        {
            return true;
        }
    } 
    return false;
}
bool Hotel::signup(string username,User *logged_user)
{
    string password, purse, phone, address; 
    getline(cin , password);
    getline(cin , purse);
    getline(cin , phone);
    getline(cin , address);
    vector <string> pass(string_split(password));
    vector <string> pur(string_split(purse));
    vector <string> ph(string_split(phone));

    if ( pass.size() != 1 || pur.size() != 1 || ph.size() != 1 || address.size() == 0)
    {
        return false;
    }

    int p = stoi(purse);

    User new_user;
    new_user.id = users.size();
    new_user.admin = false;
    new_user.name = username;
    new_user.password = password;
    new_user.purse = p;
    new_user.address = address;
    new_user.phoneNumber = phone;
    logged_user->name = new_user.name;
    logged_user->id = new_user.id;
    logged_user->admin = new_user.admin;
    logged_user->password = new_user.password;
    logged_user->phoneNumber = new_user.phoneNumber;
    logged_user->address = new_user.address;
    logged_user->purse = new_user.purse;
    users.push_back(new_user);

    return true;
}
bool Hotel::signin(string username, string password, User *logged_user)
{
    for ( int index = 0 ; index < users.size() ; index ++ )
    {
        if ( users[index].name == username && users[index].password == password)
        {   
            logged_user->name = users[index].name;
            logged_user->id = users[index].id;
            logged_user->admin = users[index].admin;
            logged_user->password = users[index].password;
            logged_user->phoneNumber = users[index].phoneNumber;
            logged_user->address = users[index].address;
            logged_user->purse = users[index].purse;
            return true;
        }
    } 
    return false;
}
bool Hotel:: signup_signin_handler(vector <string> words,User *logged_user)
{
    string command = words[0];
    if( words.size() == 3 && command == "signin" )
    {
        string username = words[1];
        string password = words[2];
        bool signin_check = signin(username , password, logged_user);
        if ( signin_check == true)
        {
            err_230();
            return true;
        }
        if ( signin_check == false)
        {   
            err_430();
            return false;
        }
    }
    else if( words.size() == 2 && command == "signup")
    {
        string username = words[1];
        bool signup_check = check_user_signup(username);
        if (signup_check == true)
        {
            err_451();
            return false;
        }
        if ( signup_check == false)
        {
            err_311();
            bool signup_or_not = signup(username,logged_user);
            if (signup_or_not == true)
            {
                err_231();
                return true;
            }
            if (signup_or_not == false)
            {
                err_503();
                return false;
            }
        }
    }
    else
    {
        err_503();
        return false;
    }
}
void Hotel::logout()
{
    err_201();
    return; 
}
int Hotel:: handle_reservation_page(vector <string> commands,User *logged_user)
{
    string number = commands[0];
    if( commands.size() == 1 && number == "1" )
    {
        logged_user_information(logged_user);
        return false;
    }
    else if( commands.size() == 1 && number == "2" )
    {
        if(logged_user->admin == true)
        {
            users_information();
        }
        else
        {
            err_403();
        }
        return false;
    }
    else if( commands.size() == 1 && number == "3" )
    {
        rooms_information(logged_user->admin);
        return false;
    }
    else if( commands.size() == 1 && number == "4" )
    {
        
    }
    else if( commands.size() == 1 && number == "5" )
    {
        
    }
    else if( commands.size() == 1 && number == "6" )
    {
        
    }
    else if( commands.size() == 1 && number == "7" )
    {
        bool edit = edit_information(logged_user);
        if ( edit == false)
        {
            err_503();
        }
        else
        {
            err_312();
        }
        return false;
    }
    else if( commands.size() == 1 && number == "8" )
    {
        
    }
    else if( commands.size() == 1 && number == "9" )
    {
        
    }
    else if( commands.size() == 1 && number == "0" )
    {
        logout();
        return true;
    }
    else
    {
        err_503();
        return false;
    }
}
void Hotel:: logged_user_information(User *logged_user)
{
    cout<<"*******************************"<<endl;
    cout<< "Your information:"<<endl;
    cout<< "id: "<<logged_user->id<<endl;
    cout<< "name: "<<logged_user->name<<endl;
    cout<< "admin: "<<logged_user->admin<<endl;
    cout<< "password: "<<logged_user->password<<endl;
    if (logged_user->admin == false)
    {
        cout<< "purse: "<<logged_user->purse<<endl;
        cout<< "phoneNumber: "<<logged_user->phoneNumber<<endl;
        cout<< "address: "<<logged_user->address<<endl;
    }
    cout<<"*******************************"<<endl;
}
void Hotel:: reservation_page()
{
    cout<< "###############################"<<endl;
    cout<< "1. View user information"<<endl;
    cout<< "2. View all users"<<endl;
    cout<< "3. View rooms information"<<endl;
    cout<< "4. Booking"<<endl;
    cout<< "5. Canceling"<<endl;
    cout<< "6. Pass day"<<endl;
    cout<< "7. Edit infromation"<<endl;
    cout<< "8. Leaving room"<<endl;
    cout<< "9. Rooms"<<endl;
    cout<< "0. Logout"<<endl;
    cout<< "###############################"<<endl;
}

bool Hotel::edit_information(User *logged_user)
{
    int index = logged_user->id;
    string password;
    if (logged_user->admin == false)
    {
        string  phone, address;

        getline(cin , password);
        getline(cin , phone);
        getline(cin , address);

        vector <string> ph(string_split(phone));
        vector <string> pass(string_split(password));

        if ( pass.size() != 1 || ph.size() != 1 || address.size() == 0)
        {
            return false;
        }

        users[index].password = password;
        logged_user->password = password;
        users[index].address = address;
        logged_user->address = address;
        users[index].phoneNumber = phone;
        logged_user->phoneNumber = phone;
        return true;
    }
    else
    {
        getline(cin , password);
        vector <string> pass(string_split(password));
        if ( pass.size() != 1)
        {
            return false;
        }

        users[index].password = password;
        logged_user->password = password;
        return true;
    }

}

int main()
{
    vector <User> users;
    vector <Room> rooms;
    Hotel myhotel(users,rooms);
    User logged_user;
    bool signup_signin_level = false;
    bool reservation_level = false;
    myhotel.decode_users();
    myhotel.decode_rooms();
    myhotel.start_print();
    while(signup_signin_level != true)
    {
        string input_str;
        getline(cin, input_str);
        vector <string> words(string_split(input_str));
        signup_signin_level = myhotel.signup_signin_handler(words,&logged_user);
    }
    while(reservation_level != true)
    {
        string input_str;
        myhotel.reservation_page();
        getline(cin, input_str);
        vector <string> words(string_split(input_str));
        reservation_level = myhotel.handle_reservation_page(words,&logged_user);
    }
}