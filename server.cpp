/*
Name : Mohammed Emtiaz Ahmed
PS id: 1455599
Email: a.emtiaz@yahoo.com, mahmed24@uh.edu
*/
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
#include <queue>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

#define PORT_NUMBER 3051              /* Port number */
#define BUFFER_SIZE 1024              /* Buffer size */

struct Lock
{
  int lockID;                         /* from 0 to 31, for 32 locks                 */
  int isBusy;                         /* 0 = not busy, 1 = busy , -1 = not set      */
  int lockCreator;                    /* pID who creates this lock                  */
  int activeLock;                     /* 0 = readlock, 1 = writelock , -1 = not set */
  vector<int> activeLock_creator;     /* pid ID that request read or write lock     */
  vector<int> readQueue;              /* read queue                                 */
  vector<int> writeQueue;             /* write queue                                */
};


Lock lock[33];                        /* total 32 locks */

int lockCount = 0;

socklen_t serverAddSize;
struct sockaddr_in serverAddress;

/* socket_creation() function establish the socket creation with binding */
int socket_creation(){

    int socketCreate, clientAccept;
    int resource;
     
    socketCreate = socket(AF_INET, SOCK_STREAM, 0);                /* creates socket */

    if(socketCreate < 0){
      cout<<"Error establishing socket"<<endl;
      return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT_NUMBER);  

    // binding socket with PORT_NUMBER
    if(bind(socketCreate, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){

      cout<<"Error in binding"<<endl;
      return -1;
    }
    
   
  return socketCreate;
}

/* Creates lock */
int create_lock(int resource, int pid){
    if(lockCount > 32){
       cout<<"No more resource create please"<<endl;
       return -1;
    }

    if(lock[resource].lockID == resource){
        cout<<"You can not create this resource again.."<<endl;
        return -1;
    }

    
    lock[resource].lockID = resource;
    lock[resource].isBusy = 0;
    lock[resource].lockCreator = pid;

 return 0;
}

/* create Read lock */
int saveRead_lock(int resource, int pid){
    if(lock[resource].lockID == -1){
      cout<<"This resource not available (create it first).."<<endl;
      return 0;
    }

    if(lock[resource].activeLock == 1 || lock[resource].writeQueue.empty() != true){
      cout<<"Resource busy or writelock in queue.."<<endl;
      lock[resource].readQueue.push_back(pid);
      return 1;
    }

    lock[resource].isBusy = 1;
    lock[resource].activeLock = 0;
    lock[resource].activeLock_creator.push_back(pid);
    cout<<"Read lock granted.."<<endl;

 return 2;
}

/* creates write lock */
int saveWrite_lock(int resource, int pid){
    if(lock[resource].lockID == -1){
      cout<<"This resource not available (create it first).."<<endl;
      return 0;
    }

    if(lock[resource].isBusy == 0){
       lock[resource].isBusy = 1;          /*make it busy*/
       lock[resource].activeLock = 1;   /* make write lock active */
       lock[resource].activeLock_creator.push_back(pid);
       cout<<"Write lock for resource:"<<resource<<" active now"<<endl;
       return 1;
    }
    
    lock[resource].writeQueue.push_back(pid);
    cout<<"write lock goes to the write queue of resource:"<<resource<<endl;
    
    

 return 2;
}

/* unlock the read lock */
int read_unlock(int resource, int pid){
   if(lock[resource].lockID == -1){
      cout<<"This resource is not available (create it first).."<<endl;
      return 0;
    }
    if(lock[resource].activeLock != 0){
      cout<<"This lock is not active .."<<endl;
      return 1;
    }
    if(lock[resource].activeLock == 0 && lock[resource].activeLock_creator.size() > 1){
      int position;
      vector<int>::iterator i = find(lock[resource].activeLock_creator.begin(), lock[resource].activeLock_creator.end(), pid);
      if(i != lock[resource].activeLock_creator.end()){
         position = distance(lock[resource].activeLock_creator.begin(), i);
         lock[resource].activeLock_creator.erase(lock[resource].activeLock_creator.begin() + position );
         cout<<"read Lock successfully unlocked.."<<endl;
         return 3;
      }
      else
         cout<<"This client did not create any read lock before.."<<endl;
      return 2;
    }
    
    if(lock[resource].writeQueue.empty() == true){
      lock[resource].activeLock = -1;
      cout<<"Read Lock unlocked successfully.."<<endl;
      return 3;
    }

    lock[resource].activeLock_creator.push_back(lock[resource].writeQueue.front());
    lock[resource].writeQueue.erase(lock[resource].writeQueue.begin(), lock[resource].writeQueue.begin() + 1);
    lock[resource].activeLock = 1;
    lock[resource].isBusy = 1;
    cout<<"Read Lock unlocked successfully and next write lock active from queue.."<<endl;

  return 4;
}

/* unlock the write lock */
int write_unlock(int resource, int pid){
    if(lock[resource].lockID == -1){
      cout<<"This resource is not available (create it first).."<<endl;
      return 0;
    }

    if(lock[resource].activeLock != 1){
      if(lock[resource].writeQueue.empty() == true){
        cout<<"This lock is not active or not available in queue.."<<endl;
        return 1;
      }     
    }

    if(lock[resource].activeLock_creator[0] != pid){
      if(lock[resource].writeQueue.empty() != true){
        int position;
        vector<int>::iterator i = find(lock[resource].writeQueue.begin(), lock[resource].writeQueue.end(), pid);
        if(i != lock[resource].writeQueue.end()){
           position = distance(lock[resource].writeQueue.begin(), i);
           lock[resource].writeQueue.erase(lock[resource].writeQueue.begin() + position);
           cout<<"Write lock unlocked from queue.."<<endl;
         return 6;
        }
        cout<<" This client is not the owner of this lock.."<<endl;
        return 2;
      }
      
      cout<<"This client is not the owner of this lock.."<<endl;
      return 2;
    }

    if(lock[resource].writeQueue.empty() != true ){
         lock[resource].activeLock_creator.clear();
         lock[resource].activeLock_creator.push_back(lock[resource].writeQueue.front());
         lock[resource].writeQueue.erase(lock[resource].writeQueue.begin(), lock[resource].writeQueue.begin() + 1);
         cout<<"Write Lock unlocked successfully.."<<endl;
        return 3;
      }    

    if(lock[resource].readQueue.empty() != true){
      lock[resource].activeLock = 0;
      int temp =  lock[resource].readQueue.size();
      for(int i = 1; i<= temp; i++){
        lock[resource].activeLock_creator.push_back(lock[resource].readQueue.front());
        lock[resource].readQueue.erase(lock[resource].readQueue.begin(), lock[resource].readQueue.begin() + 1);
      }
      cout<<"Write Lock successfully unlocked.."<<endl;

      return 4;
    }

    lock[resource].isBusy = -1;
    lock[resource].activeLock = -1;
    lock[resource].activeLock_creator.clear();
    cout<<"Write Lock successfully unlocked.."<<endl;

  return 5;
}

/* delete lock or resource */
int delete_lock(int resource, int pid){
    if( lock[resource].lockID == resource && lock[resource].lockCreator == pid){
        lock[resource].lockID = -1;
        lock[resource].isBusy = -1;
        lock[resource].lockCreator = -1;
        lock[resource].activeLock = -1;
        lock[resource].activeLock_creator.clear();
        lock[resource].readQueue.clear();
        lock[resource].writeQueue.clear();
        cout<<"Lock deleted successfully"<<endl;
        lockCount--;
        return 1;
    }
    cout<<"You have no access to delete this lock"<<endl;

  return 0;
}

/* initialize lock struct */
void lock_initialize(){
    for(int i = 0;i <= 32; i++){
       lock[i].lockID = -1;
       lock[i].isBusy = -1;
       lock[i].lockCreator = -1;
       lock[i].activeLock = -1;
       
    }

}

int main () {

    int socketCreate, clientAccept;
    int resource;
    int resourceID, functionID, pidID;
    char accBuffer[BUFFER_SIZE], sendBuffer[BUFFER_SIZE];
    lock_initialize();

    socketCreate = socket_creation();

    if(socketCreate == -1){
      cout<<"Error in establishing socket and server closing"<<endl;
      return 0;
    }

    cout<<"Looking for clients...."<<endl;

    // listen from client 
    listen(socketCreate, 100); /*maximum queue size is 100*/
    serverAddSize = sizeof(serverAddress);

    while(true){

    
        clientAccept = accept(socketCreate, (struct sockaddr*) &serverAddress, &serverAddSize);
             
        if(clientAccept < 0){
             cout<<"Error in accepting..."<<endl;
           return 0;         
        }
         
       cout<<"Client connected.."<<endl;

        recv(clientAccept, accBuffer, BUFFER_SIZE, 0);
        if(clientAccept < 0){
            cout<<"Error on receiving from client.. "<<endl;
            return 0;
        }  
    /* processes recieved message from client to retrive resource , function id, pidID parts  */
        char *tempArray = strtok(accBuffer, ",");
        int count = 1;

        while(tempArray){
          
          if(count == 1)
            resourceID = atoi(tempArray);
          else if(count == 2)
            functionID = atoi(tempArray);
          else if(count == 3)
            pidID = atoi(tempArray);
           
          tempArray = strtok(NULL,",");
          count++;
        } 

      cout<<"Request from : [ "<<"resourceID:"<<resourceID<<" , functionID:"<<functionID<<" , pidID:"<<pidID<<" ]"<<endl;
      
      if(functionID == 1){
          int status = create_lock(resourceID, pidID);
          if(status == 0){
             cout<<"Lock Successfully created.."<<endl;
             lockCount++;
          }          
      }

      else if(functionID == 2){
          int status = saveRead_lock(resourceID, pidID);
          if(status == 0){
            strcpy(sendBuffer, "Resource not available (create it first)..");
            send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }
          else if(status == 1){
             strcpy(sendBuffer, "Resource busy or writelock in queue..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);             
          }
          else if(status == 2){
             strcpy(sendBuffer, "read lock granted");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }  

      } 

      else if(functionID == 3){
          int status = saveWrite_lock(resourceID, pidID);
          if(status == 0){
            strcpy(sendBuffer, "Resource not available (create it first)..");
            send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }
          else if(status == 1){
             strcpy(sendBuffer, "write lock successfully created");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);             
          }
          else if(status == 2){
             strcpy(sendBuffer, "Resource busy, write lock go to the queue");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }          
      }
      
      else if(functionID == 4){
          int status = read_unlock(resourceID, pidID);
          if(status == 0){
             strcpy(sendBuffer, "This resource not available (create it first)..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }
          else if(status == 1){
             strcpy(sendBuffer, "This lock is not active ..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }
          else if(status == 2){
             strcpy(sendBuffer, "this client is not the owner of this lock , so can not unlock it..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }
          else{
             strcpy(sendBuffer, "Lock unlocked successfully..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }
                
      }

      else if(functionID == 5){
          int status = write_unlock(resourceID, pidID);
          if(status == 0){
            strcpy(sendBuffer, "This resource not available (create it first)..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }
          else if(status == 1){
             strcpy(sendBuffer, "This lock is not active or not available in queue..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }
          else if(status == 2){
             strcpy(sendBuffer, "this client is not the owner of this lock , so can not unlock it..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }
          else{
             strcpy(sendBuffer, "Lock unlocked successfully..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }         
                
      }

      else if(functionID == 6){
          int status = delete_lock(resourceID, pidID);
          if(status == 0){
             strcpy(sendBuffer, "You have no right to delete this lock..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }
          else{
             strcpy(sendBuffer, "Lock deleted successfully..");
             send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
          }

      }
      else{
        cout<<"Server killed.."<<endl;
        strcpy(sendBuffer, "Server killed successfully..");
        send(clientAccept, sendBuffer, BUFFER_SIZE, 0);
        close(socketCreate);
        return 0;
      }


    }

  close(socketCreate);

return 0;	
}