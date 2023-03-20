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
void Hotel::encode_users()
{
    string filename = "jsons/UsersInfo.json";
    ifstream file(filename);
    Json::Value root;
    for ( int index = 0; index < users.size(); ++index )
    {
        Json::Value elements;
        elements["id"] = users[index].id;
        elements["user"] = users[index].name;
        elements["password"] = users[index].password;
        elements["admin"] = users[index].admin;
        if( users[index].admin == false)
        {
            elements["purse"] = users[index].purse;
            elements["phoneNumber"] = users[index].phoneNumber;
            elements["address"] = users[index].address;
        }

        root["users"].append(elements);
    }

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "   ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ofstream outputFileStream(filename);
    writer -> write(root, &outputFileStream);
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
        new_user.fd = -1;
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
void Hotel::encode_rooms()
{
    string filename = "jsons/RoomsInfo.json";
    ifstream file(filename);
    Json::Value root;

    for ( int index = 0; index < rooms.size(); ++index )
    {
        Json::Value elements;
        elements["number"] = rooms[index].number;
        elements["status"] = rooms[index].status;
        elements["price"] = rooms[index].price;
        elements["maxCapacity"] = rooms[index].maxCapacity;
        elements["capacity"] = rooms[index].capacity;
        for ( int i = 0; i < rooms[index].users.size(); ++i)
        {
            Json::Value user_in_room;
            user_in_room["id"] = rooms[index].users[i].id;
            user_in_room["numOfBeds"] = rooms[index].users[i].numOfBeds;
            user_in_room["reserveDate"] = rooms[index].users[i].reserveDate;
            user_in_room["checkoutDate"] = rooms[index].users[i].checkoutDate;
            elements["users"].append(user_in_room);
        }
        root["rooms"].append(elements);
    }

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "   ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ofstream outputFileStream(filename);
    writer -> write(root, &outputFileStream);
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
bool Hotel::signin(string username, string password,int fd)
{
    for ( int index = 0 ; index < users.size() ; index ++ )
    {
        if ( users[index].name == username && users[index].password == password)
        {   
            users[index].fd = fd;
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
        bool signin_check = signin(username , password, fd);
        if ( signin_check == true)
        {
            err_230(fd);
            reservation_page(fd);
            return 2;
        }
        if ( signin_check == false)
        {   
            err_430(fd);
            return 0;
        }
    }
    else if( words.size() == 2 && command == "signup")
    {
        string username = words[1];
        bool signup_check = check_user_signup(username);
        if (signup_check == true)
        {
            err_451(fd);
            return 0;
        }
        if ( signup_check == false)
        {
            User new_user;
            new_user.name = username;
            new_user.fd = fd;
            pre_user.push_back(new_user);
            err_311(fd);
            return 1;
        }
    }
    else
    {
        err_503(fd);
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
    for (int i = 0; i < pre_user.size() ; i++)
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
    if( info.size() != 4 )
    {
        err_503(fd);
        return 0;
    }
    int p = stoi(info[1]);

    User new_user;
    new_user.fd = fd;
    new_user.id = users.size();
    new_user.admin = false;
    new_user.name = username;
    new_user.password = info[0];
    new_user.purse = p;
    new_user.address = info[3];
    new_user.phoneNumber = info[2];
    users.push_back(new_user);
    
    pre_user.erase(pre_user.begin() + pre_index);
    encode_users();
    err_231(fd);
    reservation_page(fd);
    return 2;
}
void Hotel::logout(int fd)
{
    int index=find_user(fd);
    users[index].fd = -1;
    err_201(fd);
    return; 
}
int Hotel::edit_information(string buffer,int fd)
{
    int index = find_user(fd);
    vector <string> words(string_split(buffer));
    if (users[index].admin == false)
    {

        if (words.size() != 3)
        {
            err_503(fd);
            return 2;
        }

        users[index].password = words[0];
        users[index].address = words[2];
        users[index].phoneNumber = words[1];
        
        encode_users();
        err_312(fd);
        return 2;
    }
    else
    {
        if ( words.size() != 1)
        {
            err_503(fd);
            return 2;
        }

        users[index].password = words[0];
        encode_users();
        err_312(fd);
        return 2;
    }

}
int Hotel::leave_room(string input_str,int fd)
{   
    int index = find_user(fd);
    vector <string> commands(string_split(input_str));
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
                        encode_rooms();
                        err_413(fd);
                        return 2;
                    }
                }
                err_102(fd);
                return 2;
            }
        }
        err_503(fd);
        return 2;
    }

    err_503(fd);
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
            err_111(fd);
        }
        else
        {
            encode_rooms();
            err_104(fd);
        }
    }
    else if( commands.size() == 4 && commands[0] == "modify")
    {
        bool stat = modify_room(commands[1],stoi(commands[2]),stoi(commands[3]));
        if (stat == false)
        {
            err_101(fd);
        }
        else
        {
            encode_rooms();
            err_105(fd);
        }
    }
    else if( commands.size() == 2 && commands[0] == "remove")
    {
        bool stat = remove_room(commands[1]);
        if ( stat == false)
        {
            err_101(fd);
        }
        else
        {
            encode_rooms();
            err_106(fd);
        }
    }
    else
    {
        err_503(fd);
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
        reservation_page(fd);
        return 2;
    }
    else if( commands.size() == 1 && number == "2" )
    {
        if(users[index].admin == true)
        {
            users_information(fd);
        }
        else
        {
            err_403(fd);
        }
        reservation_page(fd);
        return 2;
    }
    else if( commands.size() == 1 && number == "3" )
    {
        cout<<"1- empty rooms"<<endl;
        cout<<"2- all rooms"<<endl;
        return 12;
    }
    else if( commands.size() == 1 && number == "4" )
    {
        return 2;
    }
    else if( commands.size() == 1 && number == "5" )
    {
        return 5;
    }
    else if( commands.size() == 1 && number == "6" )
    {
        return 2;
    }
    else if( commands.size() == 1 && number == "7" )
    {
        if(users[index].admin == true)
            write(fd,"please enter your new password\n",32);
        else 
            write(fd,"please enter your new password,phone and address\n",50);
        return 7;
    }
    else if( commands.size() == 1 && number == "8" )
    {
        write(fd,"please enter room number\n",26);
        return 8;
    }
    else if( commands.size() == 1 && number == "9" )
    {
        if(users[index].admin == false)
        {
            err_403(fd);
            reservation_page(fd);
            return 2;
        }
        write(fd,"enter your command\n",20);
        return 9;
    }
    else if( commands.size() == 1 && number == "0" )
    {
        logout(fd);
        return 10;
    }
    else
    {
        err_503(fd);
        reservation_page(fd);
        return 2;
    }
    return 2;
}
void Hotel::show_reserve(int fd)
{
    int index = find_user(fd);
    for ( int i = 0 ; i< rooms.size() ; i ++)
    {
        for ( int j = 0 ; j< rooms[i].users.size() ; j++)
        {
            if ( rooms[i].users[j].id == users[index].id)
            {
                cout<< "reserved number "<<rooms[i].number << " with "<<rooms[i].users[i].numOfBeds<< " beds"<<endl;
            }
        }
    }
}
int Hotel::cancel_reserve(string buffer, int fd)
{
    int index = find_user(fd);
    vector <string> commands(string_split(buffer));
    if ( commands[0] == "cancel" && commands.size() == 3)
    {
        int numOfBeds = stoi(commands[2]);
        for( int i = 0; i< rooms.size(); i++)
        {
            if (rooms[i].number == commands[1])
            {
                for (int j = 0; j< rooms[i].users.size(); j++)
                {
                    if ( rooms[i].users[j].id == users[index].id && rooms[i].users[j].numOfBeds >= numOfBeds)
                    {
                        users[index].purse = users[index].purse + (rooms[i].price/2);
                        rooms[i].users.erase(rooms[i].users.begin() + j);
                        err_110(fd);
                        return 2;
                    }
                    else
                    {
                        err_102(fd);
                        return 2;
                    }
                }
            }
        }
        err_101(fd);
        return 2;
    }
    else
    {
        err_401(fd);
        return 2;
    }
    return 2;

}
void Hotel::users_information(int fd)
{
    string data = "";
    for ( int i = 0 ; i < users.size() ; i++)
    {
        char admin;
        if(users[i].admin)
            admin = '1';
        else 
            admin = '0';
        data = data + "\n";
        data = data + "id: " + to_string(users[i].id) + "\n";
        data = data + "name: " + users[i].name + "\n";
        data = data + "admin: " + admin + "\n";
        data = data + "password: " + users[i].password + "\n";
        if (users[i].admin == false)
        {
            data = data + "purse: " + to_string(users[i].purse) + "\n";
            data = data + "phoneNumber: " + users[i].phoneNumber + "\n";
            data = data + "address: " + users[i].address + "\n";
        }
    }
    char temp [8192];
    for(int i = 0;i < data.size();i++)
        temp[i] = data[i];
    write(fd,temp,data.size());
}
void Hotel::rooms_information(bool admin,int fd)
{
    string data = "";
    for ( int i = 0 ; i < rooms.size() ; i++)
    {
        data = data + "\n";
        data = data + "number : " + rooms[i].number + "\n";
        data = data + "status : " + to_string(rooms[i].status) + "\n";
        data = data + "price : " + to_string(rooms[i].price) + "\n";
        data = data + "maxCapacity : " + to_string(rooms[i].maxCapacity) + "\n";
        data = data + "capacity : " + to_string(rooms[i].capacity) + "\n";
        if (admin == true)
        {
            for ( int j = 0; j< rooms[i].users.size(); j++)
            {
                data = data + "\n";
                data = data + "   id : " + to_string(rooms[i].users[j].id) + "\n";
                data = data + "   numbIfBeds : " + to_string(rooms[i].users[j].numOfBeds) + "\n";
                data = data + "   reserveDate : " + rooms[i].users[j].reserveDate + "\n";
                data = data + "   checkoutDate : " + rooms[i].users[j].checkoutDate + "\n";
            }
        }
    }
    char temp [8192];
    for(int i = 0;i < data.size();i++)
        temp[i] = data[i];
    write(fd,temp,data.size());
}
void Hotel::empty_rooms_information(bool admin,int fd)
{
    string data = "";
    for ( int i = 0 ; i < rooms.size() ; i++)
    {
        if (rooms[i].status == 0)
        {
            data = data + "\n";
            data = data + "number : " + rooms[i].number + "\n";
            data = data + "status : " + to_string(rooms[i].status) + "\n";
            data = data + "price : " + to_string(rooms[i].price) + "\n";
            data = data + "maxCapacity : " + to_string(rooms[i].maxCapacity) + "\n";
            data = data + "capacity : " + to_string(rooms[i].capacity) + "\n";
            if (admin == true)
            {
                for ( int j = 0; j< rooms[i].users.size(); j++)
                {
                    data = data + "\n";
                    data = data + "   id : " + to_string(rooms[i].users[j].id) + "\n";
                    data = data + "   numbIfBeds : " + to_string(rooms[i].users[j].numOfBeds) + "\n";
                    data = data + "   reserveDate : " + rooms[i].users[j].reserveDate + "\n";
                    data = data + "   checkoutDate : " + rooms[i].users[j].checkoutDate + "\n";
                }
            }
        }
    }
    char temp [8192];
    for(int i = 0;i < data.size();i++)
        temp[i] = data[i];
    write(fd,temp,data.size());
}
int Hotel::room_information_handler(string buffer, int fd)
{
    int index = find_user(fd);
    vector <string> commands(string_split(buffer));
    if (commands[0] == "1" && commands.size()== 1)
    {
        empty_rooms_information(users[index].admin,fd);
    }
    else if ( commands[0] == "2" && commands.size() == 1)
    {
        rooms_information(users[index].admin,fd);
    }
    else
    {
        err_503(fd);
    }   
    return 2;
}
void Hotel:: logged_user_information(int fd)
{
    int index = find_user(fd);
    char admin;
    if(users[index].admin)
        admin = '1';
    else 
        admin = '0';
    string data;
    data = "Your information:\n";
    data = data + "id: " + to_string(users[index].id) + "\n";
    data = data + "name: " + users[index].name + "\n";
    data = data + "admin: " + admin + "\n";
    data = data + "password: " + users[index].password + "\n";
    if (users[index].admin == false)
    {
        data = data + "purse: " + to_string(users[index].purse) + "\n";
        data = data + "phoneNumber: " + users[index].phoneNumber + "\n";
        data = data + "address: " + users[index].address + "\n";
    }
    char temp [1024];
    for(int i = 0;i < data.size();i++)
        temp[i] = data[i];
    write(fd,temp,data.size());
}
void Hotel:: reservation_page(int fd)
{
    sleep(0.5);
    send(fd,"###############################\n1. View user information\n2. View all users\n3. View rooms information\n4. Booking\n5. Canceling\n6. Pass day\n7. Edit infromation\n8. Leaving room\n9. Rooms\n0. Logout\n###############################\n",225,0);
}
string clear_junk(string input,int size)
{
    while(input.size() > size)
        input.pop_back();
    return input;
}

//up done // down bug




int main(int argc, char const *argv[]) {
    vector <User> users;
    vector <Room> rooms;
    vector <User> pre_user;
    Hotel myhotel(users,rooms,pre_user);
    User logged_user;
    myhotel.decode_users();
    myhotel.decode_rooms();

    struct server server;
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
                    buffer = clear_junk(buffer,bytes_received);

                    if (bytes_received == 0) { // EOF
                        printf("client fd = %d closed\n", i);
                        close(i);
                        FD_CLR(i, &master_set);
                        continue;
                    }

                    if( sections[i] == 0 )
                    {
                        sections[i] = myhotel.signup_signin_handler(buffer,i);
                    }
                    else if(sections[i] == 1)
                    { 
                        sections[i] = myhotel.signup(buffer,i);
                    }
                    else if(sections[i] == 2)
                    {
                        sections[i] = myhotel.handle_reservation_page(buffer,i);
                    }
                    else if(sections[i] == 5)
                    {
                        myhotel.show_reserve(i);
                        sections[i] = myhotel.cancel_reserve(buffer,i);
                    }
                    else if(sections[i] == 7)
                    {
                        sections[i] = myhotel.edit_information(buffer,i);
                        myhotel.reservation_page(i);
                    }
                    else if(sections[i] == 8)
                    {
                        sections[i] = myhotel.leave_room(buffer,i);
                        myhotel.reservation_page(i);
                    }
                    else if(sections[i] == 9)
                    {
                        sections[i] = myhotel.room_handler(buffer,i);
                        myhotel.reservation_page(i);
                    }
                    // cout << "client" << i << " said:" << buffer;
                    buffer = "";
                    memset(temp_buff,0,2048);
                }
            }
        }
    }
    return 0;
}