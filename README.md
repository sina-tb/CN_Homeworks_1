# CN_Homeworks_1

## Classes and Structs
  Hotel:  a class to manage every action of users and every option of rooms <br />
  
   * vector <User> users ->    a list of every users signed up to hotel service server<br />
   * vector <User> pre_user ->    a list of every room in hotel <br />
   * vector <Room> rooms ->     a list of users who are in signup or signin section<br />
  
  User: a class for every user in hotel service ( admin and regular user)<br />
   * int fd ->    fd of server for every user
   * int id
   * string name
   * string password
   * bool admin
   * int purse
   * string phoneNumber
   * string address
  
  Room: a class for every room in hotel service <br />
   * string name
   * int status -> for knowing it is full or not
   * int price
   * int maxCapacity
   * int capacity 
   * vector <Reservation> users -> list of every users that are in room
  
  Reservation: a class to save reservation of users
   * int id
   * int numOfBeds
   * string reserveDate
   * string chekcoutDate
  
  server: a struct for create socketing between client and server
   * int fd
   * string host
   * int port
  
  Error: some functions for every error and messages that may occur during hotel signup and reservation process
  
  
## Functions
  Hotel:
   * decode_users : a func to decode users information from json file
   * decode_rooms : a func to decode rooms information from json file
   * encode_users : a func to encode users information to jason file
   * encode_rooms : a func to encode rooms information to jason file
   * decode_server: a func to decode server information from json file
   * setupServer : a func to setup a server in tcp/ip protocol
   * acceptClient : a func to accept a user that wants to join to server
   * 
   * start_print : a func to show a message for every users when they connect or hotel server
   * find_pre_user : a func to find user information before signup or signin based on the given fd by server
   * find_user : a func to find user information after signup or signin based on the given fd by server
   * singup_signin_handler : a func to handle process of signup and signin
   * signup : a func for process of signup
   * signin : a func for process of signin
   * check_user_signup : a func that checks the user existance in users list for signup purpuse
   * reservation_page : a func to show reservation list for every users connected to hotel user
   * handle_reservation_page : a func to handle every options in reservation page
   * logged_user_information : a func to print logged user information
   * rooms_information : a func to print rooms information for users and admins
   * users_information : a func to print users information for admins
   * edit_information : a func that allows the user or admin to edit its information
   * room_handler : a func to handle admin options to manage rooms (add, modify, remove)
   * add_room : a func that allows admin to add room to list of rooms
   * modify_room : a func that allows admin to modify room information
   * remove_room : a func that allows admin to remove room from list of rooms
   * leave_room : a func for letting users to checkout earlier than their fixed checkout date
   * cancel_reserve : a func for letting users to cancel their reservations
   * show_reserve : a func for printing reservations of the user
   * logout : a func that allows users and admin to leave the hotel service server

User:
   * connectServer: a func that lets user to connect to hotel server
   * decode_server: a func that decodes the server information from json file
   * sign_user: a func to manage process of signup from user side

Errors: All errors and messages the server may send to users. each error has a function to send the error message to user.

## Code verify

### User:
#### signup part:

![1](https://user-images.githubusercontent.com/88041997/226328386-f2410ce7-a84d-4d1f-adc5-2258e14af768.png)

#### other commands of signup:

![2](https://user-images.githubusercontent.com/88041997/226328400-90918d1b-64bb-42eb-9f16-ea92040fb441.png)

#### view user information:

![3](https://user-images.githubusercontent.com/88041997/226328412-22d1a7a6-075b-4d83-ab98-9d97575ad2e8.png)

#### view all users: 

![4](https://user-images.githubusercontent.com/88041997/226328425-466f479a-5910-422b-a527-c88794c50d7d.png)

#### view rooms information:

![5](https://user-images.githubusercontent.com/88041997/226328442-dfe2d5ef-6180-4dcf-98ff-27a09cb43b85.png)

### Admin:

#### signin part:

![6](https://user-images.githubusercontent.com/88041997/226328987-6b0e0bf1-f84a-449d-80a8-32277a2197a8.png)

#### view user information:

![7](https://user-images.githubusercontent.com/88041997/226328999-78e52566-7d28-44fe-9b55-32958a3af895.png)

#### view all users:

![8](https://user-images.githubusercontent.com/88041997/226329029-67cbd851-a64e-4a55-8232-a8cc76615eb5.png)

#### view rooms information:

![9](https://user-images.githubusercontent.com/88041997/226329053-1334ac09-dc50-48ba-ac1c-88b9c8346cc5.png)
