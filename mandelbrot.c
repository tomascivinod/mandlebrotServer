#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "pixelColor.h"
#include "mandelbrot.h"

#define WIDTH 512
#define LENGTH 512
#define LIMITMINUSX -2
#define LIMITMINUSY -1.4
#define MAX_ITERATIONS 256
#define X_INCREMENT 0.005859375
#define Y_INCREMENT 0.00540625
#define RADIUS 2

#define SIMPLE_SERVER_VERSION 1.0
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 1917
#define NUMBER_OF_PAGES_TO_SERVE 10

int waitForConnection (int serverSocket);
int makeServerSocket (int portno);
void serveBMP (int socket);

// NOT NEEDED AS IT IS INCLUDED IN pixelColor.h FILE
//unsigned char shadesOfGrey (int steps);
//unsigned char stepsToRed (int steps);
//unsigned char stepsToGreen (int steps);
//unsigned char stepsToBlue (int steps);

// after serving this many pages the server will halt

int main (int argc, char *argv[]) {
      
   printf ("************************************\n");
   printf ("Starting simple server %f\n", SIMPLE_SERVER_VERSION);
   printf ("Serving bmps since 2012\n");   
   
   int serverSocket = makeServerSocket (DEFAULT_PORT);   
   printf ("Access this server at http://localhost:%d/\n", DEFAULT_PORT);
   printf ("************************************\n");
   
   char request[REQUEST_BUFFER_SIZE];
   
   int numberServed = 0;
   while (numberServed < NUMBER_OF_PAGES_TO_SERVE) {
      
      printf ("*** So far served %d pages ***\n", numberServed);
      
      int connectionSocket = waitForConnection (serverSocket);
      // wait for a request to be sent from a web browser, open a new
      // connection for this conversation
      
      // read the first line of the request sent by the browser  
      int bytesRead;
      bytesRead = read (connectionSocket, request, (sizeof request)-1);
      assert (bytesRead >= 0); 
      // were we able to read any data from the connection?
            
      // print entire request to the console 
      printf (" *** Received http request ***\n %s\n", request);
      
      //send the browser a simple html page using http
      printf (" *** Sending http response ***\n");
      serveBMP(connectionSocket);
      
      // close the connection after sending the page- keep aust beautiful
      close(connectionSocket);
      
      numberServed++;
   } 
   
   // close the server connection after we are done- keep aust beautiful
   printf ("** shutting down the server **\n");
   close (serverSocket);
   
   return EXIT_SUCCESS; 
}
 
void serveBMP (int socket) {
   char* message;
   
   // first send the http response header
       
   // (if you write stings one after another like this on separate
   // lines the c compiler kindly joins them togther for you into
   // one long string)
   message = "HTTP/1.0 200 OK\r\n"
                "Content-Type: image/bmp\r\n"
                "\r\n";
   printf ("about to send=> %s\n", message);
   write (socket, message, strlen (message));
   
   // now send the BMP

    unsigned char bmp[786554] = {   
    0x42,0x4d,0x7a,0x00,0x0c,0x00,0x00,0x00,
    0x00,0x00,0x7a,0x00,0x00,0x00,0x6c,0x00,
    0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,
    0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0c,0x00,0x13,0x0b,
    0x00,0x00,0x13,0x0b,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x47,
    0x52,0x73,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00};

    int i = 122;
    int j = 0;
    int k = 0;
    double x = LIMITMINUSX;
    double y = LIMITMINUSY;

    int steps; 

    while (k<LENGTH) {

        x = LIMITMINUSX;
        j = 0;

        while (j<WIDTH) {

            steps =  escapeSteps(x,y);
  
            bmp[i] = stepsToRed(steps);
            bmp[i+1] = stepsToGreen(steps);
            bmp[i+2] = stepsToBlue(steps);
            
            x += X_INCREMENT;

            j++;
            i += 3;
        }

        y += Y_INCREMENT;
        k++;
    }
 
   write (socket, bmp, sizeof(bmp));
}

int escapeSteps (double x, double y) {

    int steps = 1;
    int isInside = 0;
    double c = x;
    double d = y;
    double xTemp = 0.0;

    while ((x*x + y*y) < RADIUS*RADIUS && steps < MAX_ITERATIONS) {
        xTemp = x*x - y*y + c;
        y = 2*x*y + d;
        x = xTemp;      
        steps++;
    }

    isInside = steps;
    
    return isInside;
}

/*
unsigned char shadesOfGrey (int steps) {
    unsigned char grey;
    grey = steps;
    return grey;
}

unsigned char stepsToRed (int steps) {
    unsigned char red;
    red = shadesOfGrey(steps);
    return red;
}

unsigned char stepsToGreen (int steps) {
    unsigned char green;
    green = shadesOfGrey(steps);
    return green;
}

unsigned char stepsToBlue (int steps) {
    unsigned char blue;
    blue = shadesOfGrey(steps);
    return blue;
}
*/

// start the server listening on the specified port number
int makeServerSocket (int portNumber) { 
   
   // create socket
   int serverSocket = socket (AF_INET, SOCK_STREAM, 0);
   assert (serverSocket >= 0);   
   // error opening socket
   
   // bind socket to listening port
   struct sockaddr_in serverAddress;
   bzero ((char *) &serverAddress, sizeof (serverAddress));
   
   serverAddress.sin_family      = AF_INET;
   serverAddress.sin_addr.s_addr = INADDR_ANY;
   serverAddress.sin_port        = htons (portNumber);
   
   // let the server start immediately after a previous shutdown
   int optionValue = 1;
   setsockopt (
      serverSocket,
      SOL_SOCKET,
      SO_REUSEADDR,
      &optionValue, 
      sizeof(int)
   );
 
   int bindSuccess = 
      bind (
         serverSocket, 
         (struct sockaddr *) &serverAddress,
         sizeof (serverAddress)
      );
   
   assert (bindSuccess >= 0);
   // if this assert fails wait a short while to let the operating 
   // system clear the port before trying again
   
   return serverSocket;
}
 
// wait for a browser to request a connection,
// returns the socket on which the conversation will take place
int waitForConnection (int serverSocket) {
   // listen for a connection
   const int serverMaxBacklog = 10;
   listen (serverSocket, serverMaxBacklog);
   
   // accept the connection
   struct sockaddr_in clientAddress;
   socklen_t clientLen = sizeof (clientAddress);
   int connectionSocket = 
      accept (
         serverSocket, 
         (struct sockaddr *) &clientAddress, 
         &clientLen
      );
   
   assert (connectionSocket >= 0);
   // error on accept
   
   return (connectionSocket);
}
