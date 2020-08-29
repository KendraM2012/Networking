#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "agentStruct.h"
#include "helper.h"

//defines some constant values
#define MAXAGENTS 10
#define MAXBUF 1024
#define MAXPORT 6

int main(int argc, char *argv[])
{
	int sd;
	int cSocket;
	int streamread;
	int nPort;
	char rBuffer[MAXBUF];
	char *agentList[MAXBUF];
	char cBuffer[MAXBUF] = {0};
	char sPort[MAXPORT];

	//Opens log file
	FILE *fp;
	fp = fopen("log.txt", "w+");


	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	//checks argc into server
	if (argc < 2)
	{
		printf("Usage: sever server_port \r\n");
		return 0;
	}
	//checks the port to see if it is <= MAXPORT
	memset(sPort, 0, MAXPORT);
  sprintf(sPort,"%s",argv[1]);
  nPort = atoi(sPort);

	//creats the agents list and makes sure it is <= MAXAGENTS
	struct AgentInfo cAgents[MAXAGENTS];
	for (int i = 0; i < MAXAGENTS; i++){
		memset(cAgents[i].ipaddress, 0, sizeof(cAgents[i].ipaddress));
	}
	//creates the socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		fprintf (stderr, "ERROR: socket() failed\n");
    exit (-1);
	}
	fprintf(stdout,"Socket Created\n");
	//attaches the socket to the port
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		fprintf (stderr, "ERROR: failed to attach socket to port %s\n", nPort);
    exit (-1);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(nPort);
 //binds the socket to the port
	if (bind(sd, (struct sockaddr *)&address, sizeof(address))<0){
		fprintf (stderr, "ERROR: failed to bind \n");
    exit (-1);
	}
	//listens for a connection
	if (listen(sd, 3) < 0){
		fprintf (stderr, "Listen \n");
		exit (-1);
	}
	printf("Listening for a connection\n");
	//Keeps the server open and listening for connections
	while (1)
	{
		memset(cBuffer, 0, sizeof(cBuffer));
		fflush(stdout);

		//accepts a connection
		if ((cSocket = accept(sd, (struct sockaddr *)&address,
						(socklen_t*)&addrlen))<0){
			fprintf (stderr, "accept \n");
			exit (-1);
		}
		//reads the client socket, buffer
		streamread = read( cSocket , cBuffer, MAXBUF);

		//gets agent ip
		struct sockaddr_in* pv4addr = (struct sockaddr_in*)&address;
		struct in_addr ipAddr = pv4addr->sin_addr;

		//turns agent ip into string
		char agentIP[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ipAddr, agentIP, INET_ADDRSTRLEN);

		//gets the agents actions and returns a int vaule
		int cAction = cActions(cBuffer);

		// uses the int values in a switch statement to call the correct function action
		switch (cAction) {
			case 0:
			{
				//if 0 is write back to cAction there is not a vaild action
				char *error = "Error: action is not vaild";

				printf("\n%s", error);
				write(cSocket , error , strlen(error));
				close(cSocket);
				break;
			}
			case 1:
			{
				//tells the agent that the server has received a #JOIN request and then
				// checks if the agent is a member of the agent list or not if not the
				//sever then adds the agent. if the agent is the member the server tells the
				//agent that it is already a member
				char *ok = "$OK\0";
				char *alreadyMem = "$ALREADY MEMBER\0";
				int check;

				fTime(fp);
				printf("\n Received a #JOIN action from agent %s", agentIP);
				fprintf(fp, "Received a #JOIN action from agent %s",agentIP);
				check = checkAgent(&cAgents, agentIP);
				if(check != 0){
					printf("\n Agent %s already member", agentIP);
					fTime(fp);
					fprintf(fp, "Responded to agent %s already a member", agentIP);
					write(cSocket, alreadyMem, strlen(alreadyMem));
					close(cSocket);
				}
				else{
					addAgent(&cAgents, agentIP);
					fTime(fp);
					printf("\n Responded to agent %s with %s",agentIP,ok);
					fprintf(fp, "Responded to agent %s with %s",agentIP,ok);
					write(cSocket , ok , strlen(ok));
					close(cSocket);
				}
				fflush(fp);
				break;
			}
			case 2:
			{
				//tells the agent that the server has received a #LEAVE request and then
				// checks if the agent is a member of the agent list.if the agent is a member
				// the sever removes the agent from the agent list. if the agent is not a member
				// the server tells the agent that it is not a member
				char *ok = "$OK\0";
				char *notMem = "$NOT MEMBER\0";
				int check;

				fTime(fp);
				printf("\n Received a #LEAVE action from agent %s", agentIP);
				fprintf(fp, "Received a #LEAVE action from agent %s", agentIP);
				check = checkAgent(&cAgents, agentIP);
				if (check != 0){
					removeAgent(&cAgents, agentIP);
					fTime(fp);
					printf("\n Agent %s has left", agentIP);
					fprintf(fp, "Responded to agent %s with %s", agentIP,ok);
					write(cSocket, ok, strlen(ok));
					close(cSocket);
				}
				else{
					fTime(fp);
					printf("\n Agent %s %s", agentIP, notMem);
					fprintf(fp, "Responded to agent %s with %s", agentIP, notMem);
					write(cSocket , notMem , strlen(notMem));
					close(cSocket);
				}
				fflush(fp);
				break;
			}
			case 3:
			{
				//tells the agent that the server has received a #LIST request and then
				// checks if the agent is a member of the agent list.if the agent is a member
				// the sever writes the list to the agent. if the agent is not a member
				// the server does not write the list and tells the agent that it is not a member
				char *ok = "$OK\0";
				char *notMem = "$NOT MEMBER\0";
				int check;
				printf("\n Received a #LIST action from agent %s", agentIP);
				fTime(fp);
				fprintf(fp, "Received a #LIST action from agent %s", agentIP);
				check = checkAgent(&cAgents, agentIP);
				if (check != 0){
					printf(" \n Sent List to %s", agentIP);
					fTime(fp);
					fprintf(fp, "Responded to agent %s with %s", agentIP, ok);
					listOfAgents(&cAgents, agentList);
					write(cSocket, agentList, strlen(agentList));
					close(cSocket);
				}
				else{
					fTime(fp);
					printf("\n Agent %s %s", agentIP, notMem);
					fprintf(fp, "Responded to agent %s with %s", agentIP, notMem);
					write(cSocket , notMem , strlen(notMem));
					close(cSocket);
				}
				fflush(fp);
				break;
			}
			case 4:
			{
				//tells the agent that the server has received a #LOG request and then
				// checks if the agent is a member of the agent list.if the agent is a member
				// the sever write the log file to the agent. if the agent is not a member
				// the server will not write the file and will tell the server that it is not a member
				char *ok = "$OK\0";
				char *notMem = "$NOT MEMBER\0";
				int check;

				fTime(fp);
				printf("\n Received a #LOG action from agent %s", agentIP);
				fprintf(fp, "Received a #LOG action from agent %s", agentIP);
				check = checkAgent(&cAgents, agentIP);
				if (check != 0){
					printf(" \n Sent Log file to %s", agentIP);
			 		FILE *fpr;
			 		fpr = fopen("log.txt", "r");
			 		while (fgets(rBuffer, MAXBUF, fpr) != NULL)
			 			write(cSocket, rBuffer, strlen(rBuffer));
			 		close(fpr);
			 		fTime(fp);
			 		fprintf(fp, "Responded to agent %s with %s", agentIP, ok);
			 		close(cSocket);
				}
				else{
					printf("\n Agent %s %s", agentIP, notMem);
					fTime(fp);
					fprintf(fp, "Responded to agent %s with %s", agentIP, notMem);
					write(cSocket , notMem , strlen(notMem));
					close(cSocket);
				}
				fflush(fp);
				break;
			}
		}
	}
	//closes the log file
	close(fp);
	return 0;
}
