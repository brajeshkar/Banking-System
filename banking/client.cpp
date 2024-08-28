#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;

int main(int argc,char* argv[])
{
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    //cout<<argv[1]<<" "<<argv[2]<<endl;
    // Set up server address struct
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]); // IP address of the server
    serverAddr.sin_port = htons(stoi(argv[2]));

    int cid=connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    //cout<<cid<<endl;
    // Connect to the server
    if (cid == -1)
    {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }
    else cout<<"Welcome to Banking India.."<<endl;
    //else cout<<connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr))<<endl;
    
    cout << "\nEnter username: ";
    string username;
    cin >> username;

    cout << "\nEnter password: ";
    string password;
    cin >> password;
    int userSize = username.length();
    string toSend = to_string(userSize);

    struct user
    {
        char u[50];
        char p[50];
    };

    user user1;

    strncpy(user1.u, username.c_str(), sizeof(user1.u));
    strncpy(user1.p, password.c_str(), sizeof(user1.p));

    send(clientSocket, &user1, sizeof(user1), 0);

    //char buffer[1024];
    int usermode;
    
    int n;
    
    n=recv(clientSocket, &usermode, sizeof(usermode), 0);
    
    if(n<0)
      perror("Error on reading!");
    //printf("%d", usermode);
    
    if(usermode == 1){
	cout << "MENU\n";
	cout << "1. Check Balance\n";
	cout << "2. Mini statement\n";
	cout << "------------------------\n";
	cout << "enter your choice: ";
	int choice;
	cin>>choice;
	send(clientSocket,&choice,sizeof(choice),0);
	char buffer[1024];
	do{
	    n = recv(clientSocket,buffer,sizeof(buffer)-1,0);
	    if(n<0)
		  perror("Error on reading!");
	    if(n)
		  cout << buffer;
	}
	while(n);
    }
    else if(usermode == 2){
        cout << "MENU\n";
        cout << "1. Credit to a user\n";
        cout << "2. Debit from a user\n";
        cout << "------------------------\n";
        cout << "Enter your choice: ";
        struct transaction{
    	    char username[50];
    	    int choice;
        };
        int choice;
        cin>>choice;
        if(!(choice==1||choice==2)){
        	cout << "Invalid choice. Exiting\n";
        	exit(1);
        }
        cout << "\nEnter username: ";
        string username;
        cin >> username;
        transaction tns;
        tns.choice=choice;
        strncpy(tns.username,username.c_str(),sizeof(tns.username));
        send(clientSocket,&tns,sizeof(tns),0);
        bool permit;
        recv(clientSocket,&permit,sizeof(permit),0);
        if(permit){
            int amount;
            cout << "\nEnter amount to ";
            cout<<(choice==1 ? "credit" : "debit");
            cout<<": ";
            cin >> amount;
            send(clientSocket,&amount,sizeof(amount),0);
            char buffer[1024];
	    do{
	        n = recv(clientSocket,buffer,sizeof(buffer)-1,0);
	        if(n<0)
		      perror("Error on reading!");
	        if(n)
		      cout << buffer;
	    }
	    while(n);    
        }
        else{
        	cout << "Access denied. No such customer found.\n";
        	exit(1);
        }
    }
    else if(usermode==3){
        cout << "MENU\n";
        cout << "1. Check Balance of a user\n";
        cout << "2. Check Statment of a user\n";
        cout << "------------------------\n";
        cout << "Enter your choice: ";
	struct inquiry{
	    char username[50];
	    int choice;
	};        
        int choice;
        cin>>choice;
        if(!(choice==1||choice==2)){
        	cout << "Invalid choice. Exiting\n";
        	exit(1);
        }
        cout << "\nEnter username: ";
        string username;
        cin >> username;
        inquiry inq;
        inq.choice=choice;
        strncpy(inq.username,username.c_str(),sizeof(inq.username));
        send(clientSocket,&inq,sizeof(inq),0);
        char buffer[1024];
	do{
	    n = recv(clientSocket,buffer,sizeof(buffer)-1,0);
	    if(n<0)
		  perror("Error on reading!");
	    if(n)
		  cout << buffer;
	}
	while(n);
    }
    else{
    	cout<<"nikal\n";
    }
    close(clientSocket);

    return 0;
}
