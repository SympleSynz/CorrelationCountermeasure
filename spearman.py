import os
import copy
from operator import itemgetter

def getFlowRate(dataFile):
	flowData = open(dataFile)
	interval = 0
	count = 0
	flow = 0
	flowRate = []
	for line in flowData:
		element = line.split()
		if count == 0:
			interval = float(element[1])
			flow = float(element[0])
			count = 1
		elif (float(element[1]) - interval < 1000):
			flow += float(element[0])
			count += 1
		elif (float(element[1]) - interval == 1000):
			flowRate.append(flow)
			count = 0
			interval = float(element[1])
		else:
			flowRate.append(flow)
			interval = float(element[1])
			flow = float(element[0])
			count = 1
	if count != 0:
		flowRate.append(flow)
	return flowRate

def Rank(data):
	sortData = copy.deepcopy(data)
	sortData.sort()
	rankData = {}
	for i in range(len(sortData)):
		rankData[sortData[i]] = i+1
	ranking = []
	for i in range(len(data)):
		ranking.append(rankData[data[i]])
	return ranking

def Spearman(srcFlowData,dstFlowData):
	srcLen = len(srcFlowData)
	dstLen = len(dstFlowData)
	if srcLen < dstLen:
		destFlowData = dstFlowData[:srcLen]
	elif dstLen < srcLen:
		srcFlowData = srcFlowData[:dstLen]
#Independent Variable: srcFlowData
	print("srcFlowData", srcFlowData)
#Dependent Variable: destFlowData
	print("dstFlowData", dstFlowData)
#Rank Dependent Variable
	srcRank = Rank(srcFlowData)
	print("srcRank",srcRank)
#Rank Independent Variable
	dstRank = Rank(dstFlowData)
	print("destRank",dstRank)
#Difference in Rank
	difference = []
	for i in range(len(srcRank)):
		difference.append(srcRank[i] - dstRank[i])
	print("Rank diff",difference)
#Square Difference
	for i in range(len(difference)):
		difference[i] = difference[i]**2
	print("Square diff",difference)
#Calculate correlation
	sumSquareDiff = sum(difference)
	nSize = len(srcFlowData)
	print("Sum of square difference",sumSquareDiff)
	correlation = 1 - ((6*sum(difference)/(nSize*(nSize**2-1))))
	return correlation

def eval(correlation):
	if abs(correlation) >= 0.00 and abs(correlation) <= 0.19:
		return "very weak"
	elif abs(correlation) >= 0.2 and abs(correlation) <= 0.39:
		return "weak"
	elif abs(correlation) >= 0.4 and abs(correlation) <= 0.59:
		return "moderate"
	elif abs(correlation) >= 0.6 and abs(correlation) <= 0.79:
		return "strong"
	else:
		return "very strong"

def main():
	results = open("resultsCorrelation.txt","a")
	correlateData = []
	for srcFilename in os.listdir("client/"):
		srcFlowData = getFlowRate(("client/"+srcFilename))
		spearmanData = []
		for dstFilename in os.listdir("server/"):
			dstFlowData = getFlowRate(("server/"+dstFilename))
			spearmanData.append((Spearman(srcFlowData, dstFlowData),dstFilename))
		highestCorrelation = max(spearmanData,key=itemgetter(0))
		evaluation = eval(highestCorrelation[0])
		correlateData.append((srcFilename, highestCorrelation[1], highestCorrelation[0], evaluation))
	for element in correlateData:
		resultStr = "%s %s %f %s\n" %(element[0],element[1],element[2],element[3])
		print(resultStr)
		results.write(resultStr)
		
main()