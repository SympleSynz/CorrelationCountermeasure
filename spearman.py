import os
import copy

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

def Spearman(srcFlowTxt,destFlowTxt):
	srcFlow = open(srcFlowTxt)
	destFlow = open(destFlowTxt)
	srcFlowData = []
	destFlowData = []
#Dependent Variable
	for data in srcFlow:
		data = data.strip('\n')
		srcFlowData.append(int(data))
	print("srcData",srcFlowData)
#Independent Variable
	for data in destFlow:
		data = data.strip('\n')
		destFlowData.append(int(data))
	print("destData",destFlowData)
#Rank Dependent Variable
	#srcRank = rankdata(srcFlowData)
	srcRank = Rank(srcFlowData)
	print("srcRank",srcRank)
#Rank Independent Variable
	#destRank = rankdata(destFlowData)
	destRank = Rank(destFlowData)
	print("destRank",destRank)
#Difference in Rank
	difference = []
	for i in range(len(srcRank)):
		difference.append(srcRank[i] - destRank[i])
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

def main():
	files = []
	for filename in os.listdir("."):
		if filename.endswith(".txt"):
			files.append(str(filename))
	print("Correlation Ranking", Spearman(files[1],files[0]))

main()
