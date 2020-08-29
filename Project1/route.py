import sys

class Route:
    #Sets all the intital values
   def __init__(self, numOfData):
      self.numOfData = numOfData
      self.routingTable = []
      self.neighbors = []

   #adds the data to neighbor data, and also adds the data to the intial routing table
   def setNeighbor(self, data, cost):
      self.neighbors.append(data)
      self.routingTable.append([data.getNumOfData(), cost, data.getNumOfData()])

   #gets the number of the data
   def getNumOfData(self):
        return self.numOfData

   #pairs the data into paird data packets and then sends them to recived
   def send(self):
    pairiedData = []
    for i in self.routingTable:
       pairiedData.append([i[0], i[1]])
    pack = [self.numOfData, pairiedData]
    for i in self.neighbors:
         i.receive(pack)
   #recives the packets and updates the table for each of the neighbors
   def receive(self, pack):
       self.packets = pack
       self.updateTable()
   #updates the table based on the packets and paired data
   def updateTable(self):
      for i in self.routingTable:
         if i[0] != self.packets[0]:
            continue
         cost = i[1]

      for pairiedData in self.packets[1]:
         if pairiedData[0] == self.numOfData:
            continue

         converge = False
         for i in self.routingTable:
            if i[0] != pairiedData[0]:
               continue
            converge = True
            index = self.routingTable.index(i)

         if converge:
            #bellman-ford algorithm
            if (pairiedData[1] + cost) < self.routingTable[index][1]:
               self.routingTable[index][1] = pairiedData[1] + cost
               self.routingTable[index][2] = self.packets[0]
         else:
             self.routingTable.append([pairiedData[0], pairiedData[1] + cost, self.packets[0]])

   # prints the routing table to the console
   def printRoutingTable(self):
     print("Destination:" + "\t" + "Cost:" + "\t" "Next Hop:")
     for x in str(self.numOfData):
         print("Node #" + x)
     for i in self.routingTable:
          print(str(i[0]) + "\t"+"\t" + str(i[1]) + "\t" + str(i[2]))
