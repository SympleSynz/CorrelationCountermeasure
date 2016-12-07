from __future__ import division
import os
import copy
import csv
import numpy
from operator import itemgetter

def getFlowRate(dataFile):
	flowData = open(dataFile)
	flowrate = []
	for line in flowData:
		element = line.split()
		flowrate.append(float(element[0]))
	return flowrate

def Pearson(srcFlowData,dstFlowData):
	correlation = numpy.corrcoef(srcFlowData,dstFlowData)
	return correlation[0][1]

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
	folder = str(sys.argv[1])
	folder = "20161207153702"
	directory = "results/average/"+folder+"/"
	with open("pearsonResultsCorrelation_"+folder+".csv","wb") as results:
	#results = open("resultsCorrelationBaseline.csv","wb")
		writer = csv.writer(results)
		correlateData = []
		totalCount = 0
		correct = 0
		for srcFilename in os.listdir(directory+"/server/"):
			#src = "%s "%(srcFilename)
			#results.write(src)
			pearsonData = []
			if srcFilename == "all.csv" or srcFilename.startswith("10.0"):
				pass
			else:
				srcFlowData = getFlowRate((directory+"/server/"+srcFilename))
				totalCount += 1
				writer.writerow((srcFilename," "))
				for dstFilename in os.listdir(directory+"/client/"):
					if dstFilename == "all.csv" or dstFilename.startswith("10.0"):
						pass
					else:
						dstFlowData = getFlowRate((directory+"/client/"+dstFilename))
						pearsonData.append((Pearson(srcFlowData, dstFlowData),dstFilename))
				for dst in pearsonData:
					#dstResults = "%s %f %s " %(dst[1],dst[0],eval(dst[0]))
					coef = "%f"%(dst[0])
					evaluate = eval(dst[0])
				 	writer.writerow((dst[1],coef,evaluate))
				 	#results.write(dstResults)
				highestCorrelation = max(pearsonData,key=itemgetter(0))
				for i in pearsonData:
					if i[0] == highestCorrelation[0]:
						if i[1] == srcFilename:
							highestCorrelation = (highestCorrelation[0],i[1])

				evaluation = eval(highestCorrelation[0])
				correlateData.append((srcFilename, highestCorrelation[1], highestCorrelation[0], evaluation))
				if(highestCorrelation[1] == srcFilename):
					correct += 1
				#else:
				#	print("%s != %s"%(highestCorrelation[1],srcFilename))
		for element in correlateData:
			#resultStr = "%s %s %f %s " %(element[0],element[1],element[2],element[3])
			#print(resultStr)
			correlate = element[2]
			writer.writerow((element[0],element[1],correlate,element[3]))
			#results.write(resultStr)
		percent = "Accuracy rating: %f%%"%(correct/totalCount * 100)
		writer.writerow((percent," "))
			
main()