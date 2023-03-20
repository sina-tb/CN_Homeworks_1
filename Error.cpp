#include <iostream>
#include <fstream>

using namespace std;

void err_101(int fd,fstream& serverLog)
{
    serverLog <<"101: The desired room was not found"<<endl;   
    write(fd,"101: The desired room was not found\n",37);
}
void err_102(int fd,fstream& serverLog)
{
    serverLog <<"102: Your Reservation was not found"<<endl;   
    write(fd,"102: Your Reservation was not found\n",37);
}
void err_104(int fd,fstream& serverLog)
{
    serverLog <<"104: Successfully added."<<endl;  
    write(fd,"104: Successfully added\n.",26); 
}
void err_105(int fd,fstream& serverLog)
{
    serverLog <<"105: Successfully modified."<<endl;   
    write(fd,"105: Successfully modified.\n",29);
}
void err_106(int fd,fstream& serverLog)
{
    serverLog <<"106: Successfully deleted."<<endl;   
    write(fd,"106: Successfully deleted.\n",28);
}
void err_108(int fd,fstream& serverLog)
{
    serverLog <<"108: Your account balance is not enough"<<endl;  
    write(fd,"108: Your account balance is not enough\n",41); 
}
void err_109(int fd,fstream& serverLog)
{
    serverLog <<"109: The room capacity is full"<<endl;   
    write(fd,"109: The room capacity is full\n",32);
}
void err_110(int fd,fstream& serverLog)
{
    serverLog <<"110: Successfully done."<<endl;   
    write(fd,"110: Successfully done.\n",25);
}
void err_111(int fd,fstream& serverLog)
{
    serverLog <<"111: This room already exists"<<endl;
    write(fd,"111: This room already exists\n",31);
}
void err_201(int fd,fstream& serverLog)
{
    serverLog <<"201: User logged out successfully."<<endl;   
    write(fd,"201: User logged out successfully.\n",36);
}
void err_230(int fd,fstream& serverLog)
{
    serverLog <<"230: User logged in."<<endl; 
    write(fd,"230: User logged in.\n",22);  
}
void err_231(int fd,fstream& serverLog)
{
    serverLog <<"231: User successfully signed up."<<endl; 
    write(fd,"231: User successfully signed up.\n",35);  
}
void err_311(int fd,fstream& serverLog)
{
    serverLog <<"311: User Signed up. Enter your password, purse, phone and address."<<endl;  
    write(fd,"311: User Signed up. Enter your password, purse, phone and address.\n",69); 
}
void err_312(int fd,fstream& serverLog)
{
    serverLog <<"312: Information was changed successfully."<<endl;
    write(fd,"312: Information was changed successfully.\n",44);   
}
void err_401(int fd,fstream& serverLog)
{
    serverLog <<"401: Invalid value!"<<endl;   
    write(fd,"401: Invalid value!\n",21);
}
void err_403(int fd,fstream& serverLog)
{
    serverLog <<"403: Access denied!"<<endl;   
    write(fd,"403: Access denied!\n",21);
}
void err_412(int fd,fstream& serverLog)
{
    serverLog <<"412: Invalid capacity value!"<<endl;   
    write(fd,"412: Invalid capacity value!\n",30);
}
void err_413(int fd,fstream& serverLog)
{
    serverLog <<"413: successfully Leaving."<<endl;  
    write(fd,"413: successfully Leaving.\n",28); 
}
void err_430(int fd,fstream& serverLog)
{
    serverLog <<"430: Invalid username or password."<<endl;   
    write(fd,"430: Invalid username or password.\n",36);
}
void err_451(int fd,fstream& serverLog)
{
    serverLog <<"451: User already existed!"<<endl;  
    write(fd,"451: User already existed!\n",28); 
}

void err_503(int fd,fstream& serverLog)
{
    serverLog <<"503: Bad sequence of commands"<<endl;   
    write(fd,"503: Bad sequence of commands\n",31);
}
