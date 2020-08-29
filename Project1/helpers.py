import sys

class SetUp:
    #Contains supporting functions that are mostly class based

    def __init__(self):
        pass

    @classmethod
    def get_input_filename(cls):
        #gets input file name from the command line and returns the name
        for i in range(len(sys.argv)):
            if (sys.argv[i] == '-i' and i < (len(sys.argv) - 1)):
                inputFileName = sys.argv[i + 1]

        return inputFileName
    @classmethod
    def get_rounds(cls):
        #gets the number of rounds from the command line and returns the rounds
        for i in range(len(sys.argv)):
            if (sys.argv[i] == '-r' and i < (len(sys.argv) - 1)):
                rounds = sys.argv[i + 1]

        return rounds

    @classmethod
    def import_data_file(cls):
        #Gets the file from the command line and then splits the data and returns it
        for i in range(len(sys.argv)):
            if (sys.argv[i] == '-i' and i < (len(sys.argv) - 1)):
                inputFileName = sys.argv[i + 1]

        try:
            rawData = []
            with open(inputFileName, 'r') as input_file:
                for i in input_file:
                    rawData.append(i.split())

        except IOError:
            print("Could not open input file, is path correct?")

        return rawData
    @classmethod
    def splitData(self,rawData):
        #Gets the length of all the source data
       data = []
       for i in rawData:
          if i[0] not in data:
             data.append(i[0])
          if i[1] not in data:
             data.append(i[1])
       return len(data)
