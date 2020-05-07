#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2)                                                       //check for port as parameter
    {
        cerr << "Please, type the port like './file port'" << endl;
        exit(0);
    }
    int portnum = atoi(argv[1]);                                        //grab the port number
    char message[500];                                                  //message buffer
    sockaddr_in server_address;                                         //setup a socket other important stuff
    bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(portnum);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);                //socket created
    if(server_socket < 0)                                               //check socket
    {
        cerr << "Error while establishing the server socket" << endl;
        exit(0);
    }
    int bind_status = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)); //bind socket to its address
    if(bind_status < 0)                                                 //check binding status
    {
        cerr << "Error while binding socket to local address" << endl;
        exit(0);
    }
    cout << "Waiting for a client connection..." << endl;
    listen(server_socket, 3);                                           //listen for up to 3 requests
    sockaddr_in n_socket_address;                                       //socket for a client in code
    socklen_t n_socket_address_size = sizeof(n_socket_address);         //get address size
    int new_socket = accept(server_socket, (sockaddr *)&n_socket_address, &n_socket_address_size);     //create a new socket descriptor for the new connection with client
    if(new_socket < 0)                                                  //check socket
    {
        cerr << "Error while accepting request from client!" << endl;
        exit(0);
    }
    cout << "Connection with client established!" << endl;
    cout << "Session started." << endl;
    while(1)
    {
        cout << "Waiting for client response..." << endl;
        memset(&message, 0, sizeof(message));                           //clear buffer
        recv(new_socket, (char*)&message, sizeof(message), 0);          //receive message from client
        if(!strcmp(message, "exit"))
        {
            cout << "Client has quit the session" << endl;
            break;
        }
        cout << "Client: " << message << endl;
        cout << ">";
        string gl_message;
        getline(cin, gl_message);
        memset(&message, 0, sizeof(message));                           //clear buffer
        strcpy(message, gl_message.c_str());
        if(gl_message == "exit")
        {
            send(new_socket, (char*)&message, strlen(message), 0);      //send to client that server closed the connection
            break;
        }
        send(new_socket, (char*)&message, strlen(message), 0);          //send the message to client
    }
    close(new_socket);
    close(server_socket);
    cout << "Session finished." << endl;
    return 0;   
}
