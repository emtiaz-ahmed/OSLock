/*
Name : Mohammed Emtiaz Ahmed
PS id: 1455599
Email: a.emtiaz@yahoo.com, mahmed24@uh.edu
*/
// Obligatory includes 
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <sstream>

using namespace std;

// Constants
#define ALPHA 0
#define BRAVO 5


#define PORT_NUMBER 3051            /* port number   */
#define BUFFER_SIZE 1024            /* Buffer size   */

struct sockaddr_in serverAddress;

/*
Assume, function ID for:

create_lock()   = 1
read_lock()     = 2
write_lock()    = 3
read_unlock()   = 4
write_unlock()  = 5
delete_lock()   = 6
kill_server()   = 7
*/


// Templates to be filled
int create_lock(int resource){
    int socketCreate;   
    char sendBuffer[BUFFER_SIZE], accBuffer[BUFFER_SIZE];

    socketCreate = socket(AF_INET, SOCK_STREAM, 0);

    if(socketCreate < 0){
      cout<<"Error establishing socket"<<endl;
      return 0;
     }

    cout<<"Client Socket created"<<endl;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUMBER);

    //connect with server
    if(connect(socketCreate, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
      close(socketCreate);
      return 0;
    }

    stringstream ss;
    ss <<resource<<","<<1<<","<<getpid();
    strcpy(sendBuffer,ss.str().c_str());  /* copy all stuffs */
  
    send(socketCreate, sendBuffer, BUFFER_SIZE,0);
    
    if(socketCreate < 0){
        cout<<"Error on sending to server.. "<<endl;
        return 0;
    }
    
    close(socketCreate);
 return 0;   
}



int read_lock(int resource){
 
    int socketCreate;
    char sendBuffer[BUFFER_SIZE], accBuffer[BUFFER_SIZE];

    socketCreate = socket(AF_INET, SOCK_STREAM, 0);

    if(socketCreate < 0){
      cout<<"Error establishing socket"<<endl;
      return 1;
     }

     cout<<"Client Socket created"<<endl;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUMBER);

    //connect with server
    if(connect(socketCreate, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
      close(socketCreate);
      return 1;
    }

    stringstream ss;
    ss <<resource<<","<<2<<","<<getpid();
    strcpy(sendBuffer,ss.str().c_str());  /* copy all stuffs */
    
    send(socketCreate, sendBuffer, BUFFER_SIZE,0);
    
    if(socketCreate < 0){
        cout<<"Error on sending message to server.. "<<endl;
        return 1;
    }
    
    recv(socketCreate, accBuffer, BUFFER_SIZE, 0);
    if(socketCreate < 0){
        cout<<"Error on receiving message from server"<<endl;
        return 1;
    }

    cout<<accBuffer<<endl;
    close(socketCreate);
  
  return 0;
} 

int write_lock(int resource){ 
    int socketCreate;
    char sendBuffer[BUFFER_SIZE], accBuffer[BUFFER_SIZE];

    socketCreate = socket(AF_INET, SOCK_STREAM, 0);

    if(socketCreate < 0){
      cout<<"Error establishing socket"<<endl;
      return 1;
     }

     cout<<"Client Socket created"<<endl;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUMBER);

    //connect with server
    if(connect(socketCreate, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
      close(socketCreate);
      return 1;
    }

    stringstream ss;
    ss <<resource<<","<<3<<","<<getpid();
    strcpy(sendBuffer,ss.str().c_str());  /* copy all stuffs */
   
    send(socketCreate, sendBuffer, BUFFER_SIZE,0);
    
    if(socketCreate < 0){
        cout<<"Error on sending message to server.. "<<endl;
        return 1;
    }
    
    recv(socketCreate, accBuffer, BUFFER_SIZE, 0);
    if(socketCreate < 0){
        cout<<"Error on receiving message from server"<<endl;
        return 1;
    }

    cout<<accBuffer<<endl;
    if(strcmp(accBuffer, "This resource not available (create it first)..")){
      return 1;
    }
    
    close(socketCreate);
   
  return 0;
}

int read_unlock(int resource){
    int socketCreate;
    char sendBuffer[BUFFER_SIZE], accBuffer[BUFFER_SIZE];

    socketCreate = socket(AF_INET, SOCK_STREAM, 0);

    if(socketCreate < 0){
      cout<<"Error establishing socket"<<endl;
      return 1;
     }

     cout<<"Client Socket created"<<endl;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUMBER);

    //connect with server
    if(connect(socketCreate, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
      close(socketCreate);
      return 1;
    }

    stringstream ss;
    ss <<resource<<","<<4<<","<<getpid();
    strcpy(sendBuffer,ss.str().c_str());  /* copy all stuffs */
    
    send(socketCreate, sendBuffer, BUFFER_SIZE,0);
    
    if(socketCreate < 0){
        cout<<"Error on sending message to server.. "<<endl;
        return 1;
    }
    
    recv(socketCreate, accBuffer, BUFFER_SIZE, 0);
    if(socketCreate < 0){
        cout<<"Error on receiving message from server"<<endl;
        return 1;
    }

    cout<<accBuffer<<endl;
    close(socketCreate);

return 0;
} 


int write_unlock(int resource){
    int socketCreate;
    char sendBuffer[BUFFER_SIZE], accBuffer[BUFFER_SIZE];

    socketCreate = socket(AF_INET, SOCK_STREAM, 0);

    if(socketCreate < 0){
      cout<<"Error establishing socket"<<endl;
      return 1;
     }

     cout<<"Client Socket created"<<endl;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUMBER);

    //connect with server
    if(connect(socketCreate, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
      close(socketCreate);
      return 1;
    }

    stringstream ss;
    ss <<resource<<","<<5<<","<<getpid();
    strcpy(sendBuffer,ss.str().c_str());  /* copy all stuffs */
    
    send(socketCreate, sendBuffer, BUFFER_SIZE,0);
    
    if(socketCreate < 0){
        cout<<"Error on sending message to server.. "<<endl;
        return 1;
    }
    
    recv(socketCreate, accBuffer, BUFFER_SIZE, 0);
    if(socketCreate < 0){
        cout<<"Error on receiving message from server"<<endl;
        return 1;
    }

    cout<<accBuffer<<endl;
    close(socketCreate);

return 0;
}

int delete_lock(int resource){
    int socketCreate;
    char sendBuffer[BUFFER_SIZE], accBuffer[BUFFER_SIZE];

    socketCreate = socket(AF_INET, SOCK_STREAM, 0);

    if(socketCreate < 0){
      cout<<"Error establishing socket"<<endl;
      return 1;
     }

     cout<<"Client Socket created"<<endl;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUMBER);

    //connect with server
    if(connect(socketCreate, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
      close(socketCreate);
      return 1;
    }

    stringstream ss;
    ss <<resource<<","<<6<<","<<getpid();
    strcpy(sendBuffer,ss.str().c_str());  /* copy all stuffs */
   
    send(socketCreate, sendBuffer, BUFFER_SIZE,0);
    
    if(socketCreate < 0){
        cout<<"Error on sending message to server.. "<<endl;
        return 1;
    }
    
    recv(socketCreate, accBuffer, BUFFER_SIZE, 0);
    if(socketCreate < 0){
        cout<<"Error on receiving message from server"<<endl;
        return 1;
    }

    cout<<accBuffer<<endl;
    close(socketCreate);

 return 0;
}

int kill_server(){
    int socketCreate;
    char sendBuffer[BUFFER_SIZE], accBuffer[BUFFER_SIZE];

    socketCreate = socket(AF_INET, SOCK_STREAM, 0);

    if(socketCreate < 0){
      cout<<"Error establishing socket"<<endl;
      return 1;
     }

     cout<<"Client Socket created"<<endl;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUMBER);

    //connect with server
    if(connect(socketCreate, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
      close(socketCreate);
      return 1;
    }

    stringstream ss;
    ss <<1<<","<<7<<","<<getpid();
    strcpy(sendBuffer,ss.str().c_str());  /* copy all stuffs */
    
    send(socketCreate, sendBuffer, BUFFER_SIZE,0);
    
    if(socketCreate < 0){
        cout<<"Error on sending message to server.. "<<endl;
        return 1;
    }
    
    recv(socketCreate, accBuffer, BUFFER_SIZE, 0);
    if(socketCreate < 0){
        cout<<"Error on receiving message from server"<<endl;
        return 1;
    }

    cout<<accBuffer<<endl;
    close(socketCreate);
    
return 0;
} 

main () {
  int pid; // child's pid

        // Before the fork
        cout << "Create lock ALPHA\n";
  create_lock(ALPHA);
        cout << "Create lock BRAVO\n";
  create_lock(BRAVO);
        cout << "Parent requests write permission on lock BRAVO\n";
  write_lock(BRAVO);
        cout << "Write permission on lock BRAVO was granted\n";
        cout << "Parent requests read permission on lock ALPHA\n";
  read_lock(ALPHA);
        cout << "Read permission on lock ALPHA was granted\n";
  sleep(1);
  
  // Fork a child
  if ((pid = fork()) == 0) {
    // Child process
          cout << "Child requests read permission on lock ALPHA\n";
    read_lock(ALPHA); // This permission should be granted
          cout << "Read permission on lock ALPHA was granted\n";
    sleep(1);
          cout << "Child releases read permission on lock ALPHA\n";
    read_unlock(ALPHA);
    sleep(1);
          cout << "Child requests write permission on lock BRAVO\n";
    write_lock(BRAVO); // Should wait until parent relases its lock
          cout << "Write permission on lock BRAVO was granted\n";
    sleep(1);
          cout << "Child releases write permission on lock BRAVO\n";
    write_unlock(BRAVO);
    cout << "Child terminates\n";
                _exit(0);
  } // Child

  // Back to parent
        cout << "Parent releases read permission on lock ALPHA\n";
  read_unlock(ALPHA);
        cout << "Parent requests write permission on lock ALPHA\n";
  write_lock(ALPHA); // Should wait until child removes its read lock
        cout << "Write permission on lock ALPHA was granted\n";
  sleep(1);
        cout << "Parent releasesweite permission on lock ALPHA\n";
  write_unlock(ALPHA);
  sleep(1);
        cout << "Parent releases write permission on lock BRAVO\n";
  write_unlock(BRAVO);

  // Child and parent join
        while (pid != wait(0));  // Busy wait
  delete_lock(ALPHA);
        delete_lock(BRAVO);
        // We assume that failed operations return a non-zero value
        if (write_lock(ALPHA) != 0) {
    cout << "Tried to access a deleted lock\n";
  }
  kill_server();
} // main