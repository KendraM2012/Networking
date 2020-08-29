from helpers import SetUp
import sys
import route

class RunTable:
    def run(self):
        data = []
        #Read file and rounds
        rawData = SetUp.import_data_file()
        rounds = SetUp.get_rounds()
        print("Will run"+" " + rounds + " " + "rounds")
        #gets the data point count
        splitData = SetUp.splitData(rawData)
        preFillDataInRoute(rawData,splitData,data)
        printAllRounds(rounds,data)
#keeps track of the rounds, and sends the data to route to be processed
#call the print routeing table function for all the data
def printAllRounds(rounds,data):
        messageCount = 0
        for numRounds in range(0, int(rounds)):
            print("Round #" + str(numRounds))
            for i in data:
                i.send()
                i.printRoutingTable()
                messageCount = messageCount + 1
        print("number of messages sent" + "\t" + str(messageCount))
def preFillDataInRoute(rawData,splitData,data):
    #counts all the data in data and sends the number of data to route
    for i in range(0, splitData):
        data.append(route.Route(i))
    #sets the neighbors in the route class
    for i in rawData:
        data[int(i[0])].setNeighbor(data[int(i[1])], int(i[2]))
        data[int(i[1])].setNeighbor(data[int(i[0])], int(i[2]))
