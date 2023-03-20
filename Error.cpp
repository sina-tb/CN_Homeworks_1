#include <iostream>

using namespace std;

void err_101(int fd)
{
    cout<<"101: The desired room was not found"<<endl;   
    write(fd,"101: The desired room was not found",36);
}
void err_102(int fd)
{
    cout<<"102: Your Reservation was not found"<<endl;   
    write(fd,"102: Your Reservation was not found",36);
}
void err_104(int fd)
{
    cout<<"104: Successfully added."<<endl;  
    write(fd,"104: Successfully added.",25); 
}
void err_105(int fd)
{
    cout<<"105: Successfully modified."<<endl;   
    write(fd,"105: Successfully modified.",28);
}
void err_106(int fd)
{
    cout<<"106: Successfully deleted."<<endl;   
    write(fd,"106: Successfully deleted.",27);
}
void err_108(int fd)
{
    cout<<"108: Your account balance is not enough"<<endl;  
    write(fd,"108: Your account balance is not enough",40); 
}
void err_109(int fd)
{
    cout<<"109: The room capacity is full"<<endl;   
    write(fd,"109: The room capacity is full",31);
}
void err_110(int fd)
{
    cout<<"110: Successfully done."<<endl;   
    write(fd,"110: Successfully done.",24);
}
void err_111(int fd)
{
    cout<<"111: This room already exists"<<endl;
    write(fd,"111: This room already exists",30);
}
void err_201(int fd)
{
    cout<<"201: User logged out successfully."<<endl;   
    write(fd,"201: User logged out successfully.\n",36);
}
void err_230(int fd)
{
    cout<<"230: User logged in."<<endl; 
    write(fd,"230: User logged in.",21);  
}
void err_231(int fd)
{
    cout<<"231: User successfully signed up."<<endl; 
    write(fd,"231: User successfully signed up.",34);  
}
void err_311(int fd)
{
    cout<<"311: User Signed up. Enter your password, purse, phone and address."<<endl;  
    write(fd,"311: User Signed up. Enter your password, purse, phone and address.",68); 
}
void err_312(int fd)
{
    cout<<"312: Information was changed successfully."<<endl;
    write(fd,"312: Information was changed successfully.",43);   
}
void err_401(int fd)
{
    cout<<"401: Invalid value!"<<endl;   
    write(fd,"401: Invalid value!",20);
}
void err_403(int fd)
{
    cout<<"403: Access denied!"<<endl;   
    write(fd,"403: Access denied!",20);
}
void err_412(int fd)
{
    cout<<"412: Invalid capacity value!"<<endl;   
    write(fd,"412: Invalid capacity value!",29);
}
void err_413(int fd)
{
    cout<<"413: successfully Leaving."<<endl;  
    write(fd,"413: successfully Leaving.",27); 
}
void err_430(int fd)
{
    cout<<"430: Invalid username or password."<<endl;   
    write(fd,"430: Invalid username or password.",35);
}
void err_451(int fd)
{
    cout<<"451: User already existed!"<<endl;  
    write(fd,"451: User already existed!",27); 
}

void err_503(int fd)
{
    cout<<"503: Bad sequence of commands"<<endl;   
    write(fd,"503: Bad sequence of commands\n",31);
}

