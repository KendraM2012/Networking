#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "agentStruct.h"
#include "helper.h"
#define MAXAGENTS 10 // Max agent list

//Adds agents to the list unless the agent list is full
void addAgent(struct AgentInfo *cAgents, char *agentIP){

	for (int i = 0; i < MAXAGENTS; i++){
		if (cAgents[i].ipaddress[0] == '\0'){
			strcpy(cAgents[i].ipaddress, agentIP);
			cAgents[i].startTime = time(0);
			return;
		}
	}
	printf("\nError: The list is full. Please remove a agent"); //if the list of agents is full
}
// Removes a agent from the list of agents
void removeAgent(struct AgentInfo *cAgents, char *agentIP){
	int check;
	for (int i = 0; i < MAXAGENTS; i++){
		check = strcmp(cAgents[i].ipaddress, agentIP);
		if (check == 0){
			memset(cAgents[i].ipaddress, 0, sizeof(cAgents[i].ipaddress));
		}
	}
}
// creates a list of agents and adds the time in seconds
void listOfAgents(struct AgentInfo *cAgents, char *agentList){

	int timeInSec;
	int *timeStr[128];
	memset(agentList, 0, sizeof(agentList));

	strcpy(agentList, "\n");
	for (int i = 0; i < MAXAGENTS; i++){
		if (cAgents[i].ipaddress[0] == '\0'){continue;}
		strcat(agentList, cAgents[i].ipaddress);
		memset(timeStr, 0, sizeof(timeStr));
		timeInSec = difftime(time(0), cAgents[i].startTime);
		sprintf(timeStr, "%d", timeInSec);
		strcat(agentList, ",  ");
		strcat(agentList, timeStr);
		strcat(agentList, "\n");
	}
}
//Checks if the agent is in the list. Returns a 1 if it is and a 0 if it is not in the list
int checkAgent(struct AgentInfo *cAgents, char *agentIP){

	int check;
	for (int i = 0; i < MAXAGENTS; i++){
		check = strcmp(cAgents[i].ipaddress, agentIP);
		if (check == 0){
			return 1;
		}
	}
	return 0;
}

//creates the time for the program
void fTime(FILE *fp){
	struct timeval tp;
	gettimeofday(&tp, 0);
	time_t curtime = tp.tv_sec;
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	fprintf(fp, "\n%02d:%02d:%02d:%03d  ", time.tm_hour, time.tm_min, time.tm_sec, tp.tv_usec/1000);
}
//gets the action and converts it to a string
int cActions(char* cBuffer){

	char Join[] = {'J','O','I','N','\0'};
	char Leave[] = {'L','E','A','V','E','\0'};
	char List[] = {'L','I','S','T','\0'};
	char Log[] = {'L','O','G','\0'};
	int check = 0;

	check = strcmp(Join, cBuffer);
	if (check == 0) return 1;
	check = strcmp(Leave, cBuffer);
	if (check == 0) return 2;
	check = strcmp(List, cBuffer);
	if (check == 0) return 3;
	check = strcmp(Log, cBuffer);
	if (check == 0) return 4;

	return 0;
}
