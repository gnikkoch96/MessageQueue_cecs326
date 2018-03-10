/* 

This is a simple illustration of the use of:
	ftok, msgget, msgsnd, msgrcv

Program A will use a message queue created by Program B. 
Then, they will pass messages back and forth.

Program A sends the first message and reads the reply. Program A
also sends a "fake" message to the msgQ that will never be read
by anyone.

Both child processes use message type mtype = 113 and 114.

*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;

#define S_IRUSR 256   // Allow the owner of the message queue to read from it.
#define S_IWUSR 128   // Allow the owner of the message queue to write to it.
#define S_IRGRP 32    // Allow the group of the message queue to read from it.
#define S_IWGRP 16    // Allow the group of the message queue to write to it.
#define S_IROTH 4     // Allow others to read from the message queue.
#define S_IWOTH 2     // Allow others to write to the message queue.

#define MSGSZ 256 // msg text length
#define running 1

int main() {

    int qid = msgget(1234, 0);

    // declare my message buffer
    struct buf {
            long mtype; // required
            char greeting[50]; // mesg content
    };
    
    buf msg;
    int size = sizeof(msg)-sizeof(long);

    // prepare my message to send
    strcpy(msg.greeting, "Hello there");	
    cout << getpid() << ": sends greeting" << endl;
    msg.mtype = 117; 	// set message type mtype = 117
    msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending

    msgrcv(qid, (struct msgbuf *)&msg, size, 314, 0); // reading
    cout << getpid() << ": gets reply" << endl;
    cout << "reply: " << msg.greeting << endl;
    cout << getpid() << ": now exits" << endl;


    exit(0);
}

