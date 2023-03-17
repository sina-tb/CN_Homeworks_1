#include <iostream>
#include <fstream>
#include <vector>
#include "hotel.hpp"
#include <jsoncpp/json/json.h>

using namespace std;

     

Hotel::Hotel(vector <User> usr, vector <Room> ro)
{
    users = usr;
    rooms = ro;
}

void Hotel::decode_users(string filename)
{
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
void Hotel::decode_rooms(string filename)
{
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
void Hotel::print_rooms()
{
    for ( int i = 0 ; i < rooms.size() ; i++ )
    {
        cout<< "number : " << rooms[i].number<<endl;
        cout<< "status : " << rooms[i].status<<endl;
        cout<< "price : " << rooms[i].price<<endl;
        cout<< "maxCapacity : " << rooms[i].maxCapacity<<endl;
        cout<< "capacity : " << rooms[i].capacity<<endl;
        for ( int j = 0; j< rooms[i].users.size(); j++)
        {
            cout<< "   id : " << rooms[i].users[j].id<<endl;
            cout<< "   numbOfBeds : " << rooms[i].users[j].numOfBeds<<endl;
            cout<< "   reserveDate : " << rooms[i].users[j].reserveDate<<endl;
            cout<< "   checkoutDate : " << rooms[i].users[j].checkoutDate<<endl;
        }
    }
}  
void Hotel::print_users()
{
    for ( int i = 0 ; i < users.size() ; i++)
    {
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

int main()
{
    vector <User> users;
    vector <Room> rooms;
    Hotel myhotel(users,rooms);
    myhotel.decode_rooms("jsons/RoomsInfo.json");
    myhotel.print_rooms();
}