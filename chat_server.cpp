#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<cstdlib>
#include<string>
#include<fstream>
#include<sstream>
#include<string.h>


using namespace std;

class Socket
{
	int desc;
	int newconn;

	public:

	Socket()
	{
		const int on=1;
		if((desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		{
			cout<<"Coudln't create the socket."<<endl;
		}
		else
		{
			cout<<"Socket created."<<endl;
			setsockopt(desc, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));
		}
	}

	int Bind(const char * addr, unsigned int port)
	{
		sockaddr_in addr_struct;
		addr_struct.sin_family = AF_INET;
		addr_struct.sin_addr.s_addr = inet_addr(addr);
		addr_struct.sin_port = htons(port);	

		bind(desc, (sockaddr * ) &addr_struct, sizeof(sockaddr_in));

		return 0;
	}

	int Listen(int queue)
	{
		listen(desc, queue);
		cout<<"Listening for connections..."<<endl;		
		return 0;
	}	

	int Accept()
	{
		if((newconn = accept(desc, 0, 0)) < 0)
			cout<<"Failed to accept a connection!"<<endl;
		return 0;
	}


	int Send(string text)
	{ 
		if(send(newconn, text.c_str(), text.size(), 0) < 0)
		{
			cout<<"Couldn't send the data."<<endl;
			exit(-1);
		}
		return 0;
	}
	int ReceiveText(int text_len){
		char * text = new char[1024];
		int rc=0;
		string text1="EXIT";
		if((rc = recv(newconn, text, text_len,0))>0)
		{
						
			if(strcmp(text1.c_str(),text)==0) return 0;
			cout<<"CLIENT : "<<text;
						
		}
		return 1;	
	}

	int StartChat()
	{
		int rc=0;
		string text1;
		//cout<<"You are connected with client:";
		cout<<endl<<"Type EXIT for ending chat:"<<endl;						
		do
		{
			cout<<endl<<"SERVER : ";
			getline(cin,text1);
			if(text1 == "EXIT"||text1 == "exit") 
			{
				Send(text1);
				rc=0;
			}
			else
			{
				Send(text1);
				rc = ReceiveText(100);
			}						
		}while(rc!=0);
	}

	int Close()
	{
		if(shutdown(desc, 2) != 0)
			cout<<"Socket could not be closed!"<<endl;
		else
		{
			shutdown(desc, 2);
			cout<<"Socket was closed successfully."<<endl;
		}
		return 0;
	}

};

int main()
{
	Socket test;

	test.Bind("0.0.0.0", 8080);
	test.Listen(5);
	test.Accept();
	test.StartChat();
	test.Close();
	exit(0);
}
