gcc main.c helper.c -o server -std=c99 complies the server on the university linux server.
You can drop the -std=c99 if not on the university linux server.

To run the server just run ./server portNum
		
	EX: ./server 8080
gcc agent.c -o agent comples the agent

To run the agent just run ./agent serverIP/host portNum action

	EX: ./agent zeus.cs.txstate.edu 8080 JOIN

Note: # is not included in the server read so just drop that from the action 

EX: JOIN , LEAVE , LIST , LOG 
