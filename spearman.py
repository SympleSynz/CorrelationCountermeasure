from __future__ import division
import os
import copy
import csv
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
	if abs(correlation) > 0.00 and abs(correlation) <= 0.19:
		return "very-weak"
	elif abs(correlation) > 0.19 and abs(correlation) <= 0.39:
		return "weak"
	elif abs(correlation) > 0.39 and abs(correlation) <= 0.59:
		return "moderate"
	elif abs(correlation) > 0.59 and abs(correlation) <= 0.79:
		return "strong"
	else:
		return "very-strong"

def main():
	folder = "20161206215925"
	directory = "results/average/"+folder+"/"
	with open("resultsCorrelation_"+folder+".csv","wb") as results:
	#results = open("resultsCorrelationBaseline.csv","wb")
		writer = csv.writer(results)
		correlateData = []
		# srcFilename = "results/average/20161205063812/client/112.124.140.210_19.19.19.2.out.average"
		# dstFilename = "results/average/20161205063812/server/193.1.193.64_19.19.19.25.out.average"
		# srcFlowData = getFlowRate(srcFilename)
		# dstFlowData = getFlowRate(dstFilename)
		# correlation = Spearman(srcFlowData,dstFlowData)
		# print(correlation)
		#count = 0
		totalCount = 0
		correct = 0
		for srcFilename in os.listdir(directory+"/client/"):
			srcFlowData = getFlowRate((directory+"/client/"+srcFilename))
			#src = "%s "%(srcFilename)
			#results.write(src)
			spearmanData = []
			if srcFilename == "all.csv":
				pass
			else:
				totalCount += 1
				writer.writerow((srcFilename," "))
				for dstFilename in os.listdir(directory+"/server/"):
					if dstFilename == "all.csv":
						pass
					else:
						dstFlowData = getFlowRate((directory+"/server/"+dstFilename))
						spearmanData.append((Spearman(srcFlowData, dstFlowData),dstFilename))
				for dst in spearmanData:
					#dstResults = "%s %f %s " %(dst[1],dst[0],eval(dst[0]))
					coef = "%f"%(dst[0])
					evaluate = eval(dst[0])
				 	writer.writerow((dst[1],coef,evaluate))
				 	#results.write(dstResults)
				highestCorrelation = max(spearmanData,key=itemgetter(0))
				for i in spearmanData:
					if i[0] == highestCorrelation[0]:
						if i[1] == srcFilename:
							highestCorrelation = (highestCorrelation[0],i[1])

				evaluation = eval(highestCorrelation[0])
				correlateData.append((srcFilename, highestCorrelation[1], highestCorrelation[0], evaluation))
				if(highestCorrelation[1] == srcFilename):
					correct += 1
				else:
					print("%s != %s"%(highestCorrelation[1],srcFilename))
		for element in correlateData:
			#resultStr = "%s %s %f %s " %(element[0],element[1],element[2],element[3])
			#print(resultStr)
			correlate = element[2]
			writer.writerow((element[0],element[1],correlate,element[3]))
			#results.write(resultStr)
		percent = "Accuracy rating: %f%%"%(correct/totalCount * 100)
		writer.writerow((percent," "))
			
main()