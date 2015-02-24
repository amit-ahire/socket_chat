#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<cstdlib>
#include<string.h>
#include<fstream>

using namespace std;

class SocketClient
{
	int desc;
	public:

	SocketClient()
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

	int Connect(const char * addr, unsigned int port)
	{
		sockaddr_in addr_struct;
		addr_struct.sin_family = AF_INET;
		addr_struct.sin_addr.s_addr = inet_addr(addr);
		addr_struct.sin_port = htons(port);	

		if(connect(desc, (sockaddr * ) &addr_struct, sizeof(sockaddr_in)) < 0)
		{
			cout<<"Failed to connect to the server."<<endl;
			exit(0);
		}

		else
			cout<<"Connected to the server."<<endl;

		return 0;
	}
	
	int StartChat()
	{
		string text1="init";
		int rc=0;
		//cout<<"You are connected with server:"<<endl;	
		cout<<endl<<"Type EXIT for ending chat && wait for the message:"<<endl;
		rc = ReceiveText(100);		
		do
		{		
			cout<<"CLIENT : ";
			getline(cin,text1);
			if(text1 == "EXIT" || text1 == "exit") 
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
	
	int Send(string text)
	{ 
		if(send(desc, text.c_str(), text.size(), 0) < 0)
		{
			cout<<"Couldn't send the data."<<endl;
			exit(-1);
		}
		return 0;
		
	}
	
	int ReceiveText(int text_len){
		char * text = new char[1024];
		string text1="EXIT";		
		int rc=0;
		if((rc = recv(desc, text, text_len,0))>0)
		{	
			if(strcmp(text1.c_str(),text)==0) return 0;
			cout<<"SERVER : "<<text<<endl;
						
		}
		
		return 1;	
	}
	int Close()
	{

		if(shutdown(desc,2)!=0)
			cout<<"Couldn't stop socket"<<endl;
		else
		{
			cout<<"Socket stopped succefully"<<endl;
		}
		return 0;
	}    
};


int main()
{
	SocketClient test;
	test.Connect("0.0.0.0", 8080);
	test.StartChat();
	test.Close();
	exit(0);
}
