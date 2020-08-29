#ifndef HELPER_H_
#define HELPER_H_
void addAgent(struct AgentInfo *cAgents, char *agentIP);
void removeAgent(struct AgentInfo *cAgents, char *agentIP);
void listOfAgents(struct AgentInfo *cAgents, char *agentList);
int checkAgent(struct AgentInfo *cAgents, char *agentIP);
void fTime(FILE *fpw);
int cActions(char* cBuffer);
#endif // HELPER_H_
