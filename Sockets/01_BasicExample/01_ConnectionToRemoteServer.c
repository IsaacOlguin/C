#include<stdio.h>
#include<string.h>
#include<sys/socket.h> //Library required for sockets
#include<arpa/inet.h> //inet_addr
#include<unistd.h> //Required for closing the socket_desc

int main(int argc, char **argv) {

    int socket_desc; //Socket descriptor
    struct sockaddr_in server;
    char *message; //For sending a message
    char server_reply[2000]; //For receiving a reply

    /*
    ###########################################################################
    ###########################################################################
    ####            CREATION OF A SOCKET
    ###########################################################################
    ###########################################################################
    */

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    /*
        Parameters:
            1   -   It is the Address family. Possible values are:
                        AF_INET (which is IP version 4)
                        AF_INET6 (which is IP version 6)
            2   -   It is the type of connection
                        SOCK_STREAM (connection oriented TCP Protocol)
                        SOCK_DGRAM (UDP protocol, not connection oriented protocol)
            3   -   Protocol or IPPROTO_IP. i.e. the IP
    */

    printf("Value of socket_desc %i\n", socket_desc);

    if (socket_desc == -1){
        printf("It was not possible to create the socket.\n");
    }

    /*
    ###########################################################################
    ###########################################################################
    ####            SET TO ATTRIBUTES OF THE SOCKET_ADDR_IN
    ###########################################################################
    As a remainder
    // IPv4 AF_INET sockets:
    struct sockaddr_in {
        short            sin_family;   // e.g. AF_INET, AF_INET6
        unsigned short   sin_port;     // e.g. htons(3490)
        struct in_addr   sin_addr;     // see struct in_addr, below
        char             sin_zero[8];  // zero this if you want to
    };

    struct in_addr {
        unsigned long s_addr;          // load with inet_pton()
    };

    struct sockaddr {
        unsigned short    sa_family;    // address family, AF_xxx
        char              sa_data[14];  // 14 bytes of protocol address
    };
    ###########################################################################
    ###########################################################################
    */
    server.sin_addr.s_addr = inet_addr("172.217.22.206"); //Google's IP address
    server.sin_family = AF_INET;
    server.sin_port = htons( 80 );

    /*
    ###########################################################################
    ###########################################################################
    ####            CONNECTION TO THE SERVER (REMOTE)
    ###########################################################################
    ###########################################################################
    */
    if ( connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0 ){
        puts("Error in connection to the server.");
        return 1;
    }
    puts("Verbindung...\n");

    /*
    ###########################################################################
    ###########################################################################
    ####            SENDING DATA TO THE SERVER
    ###########################################################################
    ###########################################################################
    */
    message = "GET / HTTP/1.1\r\n\r\n"; //This is an HTTP command to fetch the mainpage of a website
    if( send(socket_desc, message, strlen(message), 0) < 0){
        puts("An error ocurred when the message was sent!");
        return 1;
    }
    puts("Sendung...\n");

    /*
    ###########################################################################
    ###########################################################################
    ####            SENDING DATA TO THE SERVER
    ###########################################################################
    ###########################################################################
    */
    if( recv(socket_desc, server_reply, 2000, 0) < 0 ){
        puts("Error in reply from the server");
    }
    puts("Message bekommt...");
    puts(server_reply);

    /*
    ###########################################################################
    ###########################################################################
    ####            CLOSING THE SOCKET_DESC
    ###########################################################################
    ###########################################################################
    */
    close(socket_desc);
    
    return 0;
}