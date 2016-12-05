from __future__ import division
import os
import copy
from operator import itemgetter

def getFlowRate(dataFile):
	flowData = open(dataFile)
	flowrate = []
	for line in flowData:
		element = line.split()
		flowrate.append(element[0])
	return flowrate

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
	#print("srcFlowData", srcFlowData)
#Dependent Variable: destFlowData
	#print("dstFlowData", dstFlowData)
#Rank Dependent Variable
	srcRank = Rank(srcFlowData)
	#print("srcRank",srcRank)
#Rank Independent Variable
	dstRank = Rank(dstFlowData)
	#print("destRank",dstRank)
#Difference in Rank
	difference = []
	for i in range(len(srcRank)):
		difference.append(srcRank[i] - dstRank[i])
	#print("Rank diff",difference)
#Square Difference
	for i in range(len(difference)):
		difference[i] = difference[i]**2
	#print("Square diff",difference)
#Calculate correlation
	sumSquareDiff = sum(difference)
	nSize = len(dstFlowData)
	# print("Sum of square difference",sumSquareDiff)
	# print("size",nSize)
	# print("6*diff",(6*sumSquareDiff))
	# print("size**2 - 1", (nSize**2 - 1))
	# dividend = nSize*(nSize**2 - 1)
	# print("size * above", dividend)
	# print("now divide", (6*sumSquareDiff)/dividend)
	correlation = 1 - ((6*sumSquareDiff/(nSize*(nSize**2-1))))
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
	results = open("resultsCorrelationBaseline.txt","a")
	correlateData = []
	# srcFilename = "results/average/20161205063812/client/112.124.140.210_19.19.19.2.out.average"
	# dstFilename = "results/average/20161205063812/server/193.1.193.64_19.19.19.25.out.average"
	# srcFlowData = getFlowRate(srcFilename)
	# dstFlowData = getFlowRate(dstFilename)
	# correlation = Spearman(srcFlowData,dstFlowData)
	# print(correlation)
	#count = 0
	for srcFilename in os.listdir("results/average/20161205063812/client/"):
		srcFlowData = getFlowRate(("results/average/20161205063812/client/"+srcFilename))
		spearmanData = []
		for dstFilename in os.listdir("results/average/20161205063812/server/"):
			dstFlowData = getFlowRate(("results/average/20161205063812/server/"+dstFilename))
			spearmanData.append((Spearman(srcFlowData, dstFlowData),dstFilename))
		# for dst in spearmanData:
		# 	dstResults = "\n%s %s %f\n" %(srcFilename,dst[1],dst[0])
		# 	results.write(dstResults)
		highestCorrelation = max(spearmanData,key=itemgetter(0))
		evaluation = eval(highestCorrelation[0])
		correlateData.append((srcFilename, highestCorrelation[1], highestCorrelation[0], evaluation))
	for element in correlateData:
		resultStr = "%s %s %f %s\n" %(element[0],element[1],element[2],element[3])
		#print(resultStr)
		results.write(resultStr)
		
main()