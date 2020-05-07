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
    if(argc != 3)                                           //check for IP and port as parameters
    {
        cerr << "Please, type the adress and port like './file IP port'" << endl; 
        exit(0); 
    }
    char *server_ip = argv[1]; 
    int portnum = atoi(argv[2]);                            //assign IP and port
    char message[500];                                      //message buffer
    struct hostent* host = gethostbyname(server_ip);        //setup a socket other important stuff 
    sockaddr_in s_socket_address;   
    bzero((char*)&s_socket_address, sizeof(s_socket_address)); 
    s_socket_address.sin_family = AF_INET; 
    s_socket_address.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    s_socket_address.sin_port = htons(portnum);
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    int status = connect(client_socket, (sockaddr*) &s_socket_address, sizeof(s_socket_address));
    if(status < 0)
    {
        cerr<<"Error while connecting to socket!"<<endl;
        exit(0);
    }
    cout << "Connection with server established!" << endl;
    cout << "Session started." << endl;
    while(1)
    {
        cout << ">";
        string gl_message;
        getline(cin, gl_message);
        memset(&message, 0, sizeof(message));               //clear buffer
        strcpy(message, gl_message.c_str());
        if(gl_message == "exit")
        {
            send(client_socket, (char*)&message, strlen(message), 0);
            break;
        }
        send(client_socket, (char*)&message, strlen(message), 0);
        cout << "Waiting for server response..." << endl;
        memset(&message, 0, sizeof(message));               //clear buffer
        recv(client_socket, (char*)&message, sizeof(message), 0);
        if(!strcmp(message, "exit"))
        {
            cout << "Server has quit the session" << endl;
            break;
        }
        cout << "Server: " << message << endl;
    }
    close(client_socket);
    cout << "Session finished." << endl;
    return 0;    
}
