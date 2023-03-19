#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
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
Hotel::Hotel(vector <User> usr, vector <Room> ro,vector <User> pre)
{
    users = usr;
    rooms = ro;
    pre_user = pre;
}
void Hotel::start_print(int fd)
{
    write(fd,"Welcome\nPlease signup or signin to hotel first\n",48);
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
bool Hotel::signin(string username, string password)
{
    for ( int index = 0 ; index < users.size() ; index ++ )
    {
        if ( users[index].name == username && users[index].password == password)
        {   
            return true;
        }
    } 
    return false;
}
int Hotel:: signup_signin_handler(string input, int fd)
{
    vector <string> words(string_split(input));
    string command = words[0];
    if( words.size() == 3 && command == "signin" )
    {
        string username = words[1];
        string password = words[2];
        cout << username.size() << "," << password.size() ;
        bool signin_check = signin(username , password);
        if ( signin_check == true)
        {
            err_230();
            return 2;
        }
        if ( signin_check == false)
        {   
            err_430();
            return 0;
        }
    }
    else if( words.size() == 2 && command == "signup")
    {
        string username = words[1];
        bool signup_check = check_user_signup(username);
        if (signup_check == true)
        {
            err_451();
            return 0;
        }
        if ( signup_check == false)
        {
            User new_user;
            new_user.name = username;
            new_user.fd = fd;
            pre_user.push_back(new_user);
            err_311();
            return 1;
        }
    }
    else
    {
        err_503();
        return 0;
    }
    return 0;
}
int setupServer(int port) 
{
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int connected = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if(connected == -1)
        return -1;
    
    listen(server_fd, 10);

    return server_fd;
}
int acceptClient(int server_fd)
{
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);
    client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*) &address_len);

    return client_fd;
}
bool decode_server(struct server* server)
{
    string filename = "jsons/config.json";
    ifstream file(filename);
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( file, root );
    if ( !parsingSuccessful )
    {
        cout << "Error parsing the string" << endl;
    }
    bool status = root["connected"].asBool();
    if(status)
    {
        cout << "this port is already taken try another one" << endl;
        // root["connected"] = true;
        return false;
    }
    server->host = root["hostName"].asString();
    server->port = root["commandChannelPort"].asInt();
    return true;
}
int Hotel::find_pre_user(int fd)
{
    for (int i = 0; i < users.size() ; i++)
    {
        if(pre_user[i].fd == fd)
        {
            return i;
        }
    }
}
int Hotel::find_user(int fd)
{
    for (int i = 0; i < users.size() ; i++)
    {
        if(users[i].fd == fd)
        {
            return i;
        }
    }
}
int Hotel::signup(string buffer,int fd)
{
    int pre_index = find_pre_user(fd); 
    string username = pre_user[pre_index].name;
    vector <string> info(string_split(buffer));
    if( buffer.size() != 4 )
    {
        err_503();
        return 0;
    }

    int p = stoi(info[1]);

    User new_user;
    new_user.id = users.size();
    new_user.admin = false;
    new_user.name = username;
    new_user.password = info[0];
    new_user.purse = p;
    new_user.address = info[3];
    new_user.phoneNumber = info[2];
    users.push_back(new_user);
    
    pre_user.erase(pre_user.begin() + pre_index);
    return 2;
}
void Hotel::logout()
{
    err_201();
    return; 
}
bool Hotel::edit_information(string buffer,int fd)
{
    int index = find_user(fd);
    vector <string> words(string_split(buffer));
    if (users[index].admin == false)
    {

        if (words.size() != 3)
        {
            err_503();
            return 2;
        }

        users[index].password = words[0];
        users[index].address = words[2];
        users[index].phoneNumber = words[1];

        err_312();
        return 2;
    }
    else
    {
        if ( words.size() != 1)
        {
            err_503();
            return 2;
        }

        users[index].password = words[0];
        return 2;
    }

}
int Hotel::leave_room(string input_str,int fd)
{   
    int index = find_user(fd);
    vector <string> commands(string_split(input_str));

/*     if (users[index].admin == true && commands.size() == 2 && commands[0] == "room")
    {
        for ( int index = 0; index < rooms.size(); index ++)
        {
            if (rooms[index].number == commands[1] )
            {
                return 11;
            }
        }
        err_101();
        return 2;
    } */
    if (users[index].admin == false && commands.size() == 2 && commands[0] == "room")
    {
        for (int index = 0; index < rooms.size(); index ++)
        {
            if(commands[1] == rooms[index].number )
            {
                for(int j = 0; j< rooms[index].users.size(); j++)
                {
                    if( rooms[index].users[j].id == users[index].id)
                    {
                        rooms[index].capacity = rooms[index].capacity + rooms[index].users[j].numOfBeds;
                        rooms[index].users.erase(rooms[index].users.begin() + j);
                        err_413();
                        return 2;
                    }
                }
                err_102();
                return 2;
            }
        }
        err_503();
        return 2;
    }

    err_503();
    return 2;
    
}
bool Hotel::modify_room(string room_number,int max_capacity,int price)
{
    for ( int index = 0; index < rooms.size(); index ++)
    {
        if ( rooms[index].number == room_number)
        {   
            if ( max_capacity > rooms[index].maxCapacity)
            {
                rooms[index].status = 0;
            }
            rooms[index].maxCapacity = max_capacity;
            rooms[index].price = price;
            return true;
        }
    }
    return false;

}
bool Hotel::add_room(string room_number, int max_capacity,int price)
{
    for ( int index = 0 ; index < rooms.size(); index ++)
    {
        if( rooms[index].number == room_number)
        {
            return false;
        }
    }

    Room new_room;
    vector <Reservation> new_res;
    new_room.maxCapacity = max_capacity;
    new_room.number = room_number;
    new_room.capacity = max_capacity;
    new_room.price = price;
    new_room.status = 0;
    rooms.push_back(new_room);
    return true;
}
bool Hotel::remove_room(string room_number)
{
    for ( int index = 0; index < rooms.size(); index ++)
    {
        if ( rooms[index].number == room_number)
        {  
            rooms.erase(rooms.begin()+ index);
            return true;
        }
    }
    return false; 
}
int Hotel::room_handler(string buffer, int fd)
{
    int index = find_user(fd);
    vector <string> commands(string_split(buffer));

    if ( commands.size()  == 4 && commands[0] == "add")
    {
        bool stat = add_room(commands[1],stoi(commands[2]),stoi(commands[3]));
        if ( stat == false )
        {
            err_111();
        }
        else
        {
            err_104();
        }
    }
    else if( commands.size() == 4 && commands[0] == "modify")
    {
        bool stat = modify_room(commands[1],stoi(commands[2]),stoi(commands[3]));
        if (stat == false)
        {
            err_101();
        }
        else
        {
            err_105();
        }
    }
    else if( commands.size() == 2 && commands[0] == "remove")
    {
        bool stat = remove_room(commands[1]);
        if ( stat == false)
        {
            err_101();
        }
        else
        {
            err_106();
        }
    }
    else
    {
        err_503();
    }
    return 2;
}
int Hotel:: handle_reservation_page(string input_str, int fd)
{
    int index = find_user(fd);
    vector <string> commands(string_split(input_str));
    string number = commands[0];
    if( commands.size() == 1 && number == "1" )
    {
        logged_user_information(fd);
        return 2;
    }
    else if( commands.size() == 1 && number == "2" )
    {
        if(users[index].admin == true)
        {
            users_information();
        }
        else
        {
            err_403();
        }
        return 2;
    }
    else if( commands.size() == 1 && number == "3" )
    {
        rooms_information(users[index].admin);
        return 2;
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
        return 7;
    }
    else if( commands.size() == 1 && number == "8" )
    {
        return 8;
    }
    else if( commands.size() == 1 && number == "9" )
    {
        return 9;
    }
    else if( commands.size() == 1 && number == "0" )
    {
        logout();
        return 10;
    }
    else
    {
        err_503();
        return 2;
    }
    return 2;
}

//up done // down bug

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
void Hotel:: logged_user_information(int fd)
{
    int index = find_user(fd);
    cout<<"*******************************"<<endl;
    cout<< "Your information:"<<endl;
    cout<< "id: "<<users[index].id<<endl;
    cout<< "name: "<<users[index].name<<endl;
    cout<< "admin: "<<users[index].admin<<endl;
    cout<< "password: "<<users[index].password<<endl;
    if (users[index].admin == false)
    {
        cout<< "purse: "<<users[index].purse<<endl;
        cout<< "phoneNumber: "<<users[index].phoneNumber<<endl;
        cout<< "address: "<<users[index].address<<endl;
    }
    cout<<"*******************************"<<endl;
}
void Hotel:: reservation_page(int fd)
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

int main(int argc, char const *argv[]) {
    vector <User> users;
    vector <Room> rooms;
    vector <User> pre_user;
    Hotel myhotel(users,rooms,pre_user);
    User logged_user;
    myhotel.decode_users();
    myhotel.decode_rooms();

    struct server server;
    // if(!decode_server(&server))
    //     return 0;
    decode_server(&server);
    int new_socket, max_sd;
    char temp_buff [2048];
    fd_set master_set, working_set;
    server.fd = setupServer(server.port);
    if(server.fd == -1)
    {
        cout << "this port is already in use"<< endl;
        return 0;
    }   
    FD_ZERO(&master_set);
    max_sd = server.fd;
    FD_SET(server.fd, &master_set);

    int sections [100];

    //set date 


    while (1) {
        working_set = master_set;
        select(max_sd + 1, &working_set, NULL, NULL, NULL);

        for (int i = 0; i <= max_sd; i++) {
            if (FD_ISSET(i, &working_set)) {
                
                if (i == server.fd) {  // new clinet
                    new_socket = acceptClient(server.fd);
                    FD_SET(new_socket, &master_set);
                    if (new_socket > max_sd)
                        max_sd = new_socket;
                    myhotel.start_print(new_socket);
                    sections[new_socket] = 0;
                    cout << "New client connected(fd =" << new_socket << ")" << endl;
                }
                
                else { // client sending msg
                    int bytes_received = 0;
                    bytes_received = recv(i , temp_buff , sizeof(temp_buff), 0);
                    string buffer = temp_buff;
                    // buffer.pop_back();
                    // cout << buffer << "," << buffer.size();
                    // cout << bytes_received << endl;

                    if (bytes_received == 0) { // EOF
                        printf("client fd = %d closed\n", i);
                        close(i);
                        FD_CLR(i, &master_set);
                        continue;
                    }
                    
                    if(sections[i] == 0)
                        sections[i] = myhotel.signup_signin_handler(buffer,i);
                    else if(sections[i] == 1)
                    {
                        sections[i] = myhotel.signup(buffer,i);
                    }
                    else if(sections[i] == 2)
                    {
                        myhotel.reservation_page(i);
                        sections[i] = myhotel.handle_reservation_page(buffer,i);
                    }
                    else if(sections[i] == 7)
                    {
                        sections[i] = myhotel.edit_information(buffer,i);
                    }
                    else if(sections[i] == 8)
                    {
                        sections[i] = myhotel.leave_room(buffer,i);
                    }
                    else if(sections[i] == 9)
                    {
                        sections[i] = myhotel.room_handler(buffer,i);
                    }
                    cout << "client" << i << " said:" << buffer;
                    write(new_socket, "hello", 6);
                    buffer = "";
                    memset(temp_buff,0,2048);
                }
            }
        }
    }
    return 0;
}