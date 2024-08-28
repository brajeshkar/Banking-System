#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

// for getting user's type //used by police/admin
string auth(string username)
{
    ifstream file("login_file.txt");

    if (!file.is_open())
    {
        cerr << "Unable to open the file." << std::endl;
        return "err"; // Return an error code
    }

    std::string line;
    string u, p, t;
    while (file)
    {
        file >> u >> p >> t;
        if (u == username)
        {
            file.close();
            return t;
        }
    }

    file.close();
    return "err";
}
int balanceCheck(string username)
{
    string filename = "database/" + username + ".txt";
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Unable to open the file." << std::endl;
        return 0; // Return an error code
    }

    string val;
    while (file)
    {
        file >> val;
    }
    /*string str = "Account Balance is :"+val;
    char balance[1000];
    strncpy(balance, str.c_str(), sizeof(balance));
    send(*clientSocket,balance,strlen(balance),0);
    */
    //cout << "Account Balance is :" << val;
    
    return stoi(val);
}
/*void miniStatment(string username)
{
    string filename = "database/" + username + ".txt";
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Unable to open the file." << std::endl;
        return; // Return an error code
    }

    string line;
    cout << "Your Mini Statement is :" << endl;
    while (file)
    {
        getline(file, line);
        cout << line << endl;
    }
    //char
}*/

int balanceCheck_(string username,int* clientSocket)
{
    string filename = "database/" + username + ".txt";
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Unable to open the file." << std::endl;
        return 0; // Return an error code
    }

    string val;
    while (file)
    {
        file >> val;
    }
    //cout<<"called "<<username<<endl;
    string str = "Account Balance is : "+val+"\n";
    char balance[1000]; const char *bal=str.c_str();
    strncpy(balance, bal, strlen(bal));
    //cout<<bal<<" "<<strlen(bal)<<";
    balance[strlen(bal)]='\0';
    send(*clientSocket,balance,strlen(balance)+1,0);
    //cout << "Account Balance is : " << val;
    
    return stoi(val);
}

void miniStatment_(string username,int* clientSocket)
{
    string filename = "database/" + username + ".txt";
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Unable to open the file." << std::endl;
        return; // Return an error code
    }

    string line;
    //cout << "Your Mini Statement is :" << endl;
    char stmt[1000]="Your Mini Statement reads :\n";
    send(*clientSocket,stmt,strlen(stmt)+1,0);
    while (file)
    {
        getline(file, line);
        cout << "" ;
        //if(line.empty()) break;
        const char* stmtline=(line.append("\n")).c_str();
        //cout<< stmtline;
        strncpy(stmt,stmtline,strlen(stmtline));
        stmt[strlen(stmtline)]='\0';
        send(*clientSocket,stmt,strlen(stmt)+1,0);
    }
    //char
}
void admin(int* clientSocket)
{
    int usermode=2;
    send(*clientSocket,&usermode,sizeof(usermode),0);
    /*cout << "MENU\n";
    cout << "1. Credit to a user\n";
    cout << "2. Debit from a user\n";
    cout << "------------------------\n";
    cout << "Enter your choice: ";*/
    struct transaction{
    	char username[50];
    	int choice;
    }tns;
    recv(*clientSocket,&tns,sizeof(tns),0);
    int choice=tns.choice;
    string username=tns.username;
    //cin >> choice;
    /*if (choice == 1)
    {
        cout << "\nEnter username: ";
        string username=tns.username;
        //cin >> username;
        */
        if (auth(username) == "C")
        {
            bool permit=1;
            send(*clientSocket,&permit,sizeof(permit),0);
            int currentAmount = balanceCheck(username);
            if(choice == 1)
            {
		    int toCredit;
		    //cout << "\nEnter amount to credit: ";
		    //cin >> toCredit;
		    recv(*clientSocket,&toCredit,sizeof(toCredit),0);
		    currentAmount += toCredit;
		    //cout << currentAmount << endl;

		    fstream fout;
		    string filename = "database/" + username + ".txt";
		    fout.open(filename, ios::app);
		    if (!fout.is_open())
		    {
		        cerr << "Unable to open the file." << std::endl;
		        return; // Return an error code
		    }
		    time_t t;
		    struct tm *tmp;
		    char MY_TIME[50];
		    time(&t);

		    tmp = localtime(&t);

		    // using strftime to display time
		    strftime(MY_TIME, sizeof(MY_TIME), "%x - %I:%M%p", tmp);
		    fout<<MY_TIME;

		    string stmt = " CREDIT " + to_string(currentAmount);
		    fout << stmt << endl;
		    fout.close();
		    const char* credited="Amount successfully credited to account!\nBank balance updated.\n";
		    send(*clientSocket,credited,strlen(credited)+1,0);
		}
		else
		{
		    int toDebit;
		    //cout << "\nEnter amount to debit: ";
		    //cin >> toDebit;
		    recv(*clientSocket,&toDebit,sizeof(toDebit),0);
		    
		    currentAmount -= toDebit;
		    
		    if(currentAmount<0){
		        const char* underflow="\nUnderflow error in balance.\n";
		        send(*clientSocket,underflow,strlen(underflow)+1,0);
		        return;
		    }

		    fstream fout;
		    string filename = "database/" + username + ".txt";
		    fout.open(filename, ios::app);
		    if (!fout.is_open())
		    {
		        cerr << "Unable to open the file." << std::endl;
		        return; // Return an error code
		    }
		    time_t t;
		    struct tm *tmp;
		    char MY_TIME[50];
		    time(&t);

		    tmp = localtime(&t);

		    // using strftime to display time
		    strftime(MY_TIME, sizeof(MY_TIME), "%x - %I:%M%p", tmp);
		    fout<<MY_TIME;

		    string stmt = " DEBIT " + to_string(currentAmount);
		    fout << stmt << endl;
		    fout.close();
		    const char* debited="Amount successfully debited from account!\nBank balance updated.\n";
		    send(*clientSocket,debited,strlen(debited)+1,0);
		}
        }
        else
        {
            bool permit=0;
            send(*clientSocket,&permit,sizeof(permit),0);
            //cout << "Access denied. No such customer found.\n";
        }
    //}
    /*else if (choice == 2) // to debit from user by admin
    {
        cout << "\nEnter username: ";
        string username;
        cin >> username;
        if (auth(username) == "C")
        {
            int currentAmount = balanceCheck(username);
            int toDebit;
            cout << "\nEnter amount to debit: ";
            cin >> toDebit;
            currentAmount -= toDebit;
            if(currentAmount<0){
                cout<<"\nUnderflow error in balance.\n";
                return;
            }

            fstream fout;
            string filename = "database/" + username + ".txt";
            fout.open(filename, ios::app);
            if (!fout.is_open())
            {
                cerr << "Unable to open the file." << std::endl;
                return; // Return an error code
            }
            time_t t;
            struct tm *tmp;
            char MY_TIME[50];
            time(&t);

            tmp = localtime(&t);

            // using strftime to display time
            strftime(MY_TIME, sizeof(MY_TIME), "%x - %I:%M%p", tmp);
            fout<<MY_TIME;

            string stmt = " DEBIT " + to_string(currentAmount);
            fout << stmt << endl;
            fout.close();
        }
        else
        {
            cout << "Access denied. No such customer found.\n";
        }
    }
    else
    {
        cout << "Invalid choice. Exiting\n";
    }*/
}
void police(string username,int* clientSocket)
{
    int usermode=3;
    send(*clientSocket,&usermode,sizeof(usermode),0);
    /*cout << "MENU\n";
    cout << "1. Check Balance of a user\n";
    cout << "2. Check Statment of a user\n";
    cout << "------------------------\n";
    cout << "Enter your choice: ";*/
    struct inquiry{
	char username[50];
	int choice;
    }inq;
    int n=recv(*clientSocket,&inq,sizeof(inq),0);
    //cout<<"hello\n";
    //cout<<inq.choice<<" "<<inq.username<<endl;
    int choice=inq.choice;
    //cin >> choice;
    if (choice == 1)
    {
        //cout << "\nEnter username: ";
        string username=inq.username;
        //cin >> username;
        if (auth(username) == "C")
        {
            balanceCheck_(username,clientSocket);
        }
        else
        {
            cout << "Access denied. No such customer found.\n";
        }
    }
    else if (choice == 2)
    {
        //cout << "\nEnter username: ";
        string username=inq.username;
        //cin >> username;
        if (auth(username) == "C")
        {
            miniStatment_(username,clientSocket);
        }
        else
        {
            cout << "Access denied. No such customer found.\n";
        }
    }
    /*else
    {
        cout << "Invalid choice. Exiting\n";
    }*/
}

void customer(string username,int* clientSocket)
{
    int usermode=1;
    //int mode = htonl(usermode);
    send(*clientSocket,&usermode,sizeof(usermode),0);
    /*cout << "MENU\n";
    cout << "1. Check Balance\n";
    cout << "2. Mini statement\n";
    cout << "------------------------\n";
    cout << "enter your choice: ";*/
    int choice;
    recv(*clientSocket, &choice, sizeof(choice), 0);
    //cin >> choice;
    if (choice == 1)
        balanceCheck_(username,clientSocket);
    else if (choice == 2)
        miniStatment_(username,clientSocket);
    else
    {
        cout << "Invalid choice. Exiting\n";
    }
}

// For pure authentication of user
string auth(string username, string password)
{
    ifstream file("login_file.txt");

    // Check if the file is open
    if (!file.is_open())
    {
        cerr << "Unable to open the file." << std::endl;
        return "err"; // Return an error code
    }

    // Read and print the file content line by line
    std::string line;
    string u, p, t;
    while (file)
    {
        file >> u >> p >> t;
        if (u == username && p == password)
        {
            file.close();
            return t;
        }
    }

    file.close();
    return "err";
}

/*struct client{
	int mode,op;
} newclient;*/

int main(int argc,char* argv[])
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    // char username[50], password[50];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("192.168.0.119"); //"127.2.3.4");
    serverAddr.sin_port = htons(stoi(argv[1]));

    int reuse = 1;
    #ifdef SO_REUSEADDR
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    #endif
    #ifdef SO_REUSEPORT
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
        perror("setsockopt(SO_REUSEPORT) failed");
    #endif
    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1)
    {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n",stoi(argv[1]));

    // Accept a connection
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
    if (clientSocket == -1)
    {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }
    //char buffer[]="Hello";
    //send(clientSocket,buffer,strlen(buffer),0);
    
    struct user
    {
        char u[50];
        char p[50];
    };

    user st1;
    recv(clientSocket, &st1, sizeof(st1), 0);
    string userType = auth(st1.u, st1.p);
    if (userType != "err")
    {
        // cout<<"Welcome "<<st1.u<<endl;
        if (userType == "C")
        {
            customer(st1.u,&clientSocket);
        }
        else if (userType == "A")
        {
            admin(&clientSocket);
        }
        else
        {
            police(st1.u,&clientSocket);
        }
    }
    else{
        cout << "nikal";
        int usermode=0;
        send(clientSocket,&usermode,sizeof(clientSocket),0);
    }
    // cout<<st1.u<<endl<<st1.p<<endl;

    // Close the sockets
    close(serverSocket);
    close(clientSocket);

    return 0;
}
