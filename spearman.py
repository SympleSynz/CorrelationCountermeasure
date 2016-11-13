import os
#Following code pulled from Stackoverflow to rank unsorted lists
#http://stackoverflow.com/questions/3071415/efficient-method-to-calculate-the-rank-vector-of-a-list-in-python
def rank_simple(vector):
    return sorted(range(len(vector)), key=vector.__getitem__)

def rankdata(a):
    n = len(a)
    ivec=rank_simple(a)
    svec=[a[rank] for rank in ivec]
    sumranks = 0
    dupcount = 0
    newarray = [0]*n
    for i in range(n):
        sumranks += i
        dupcount += 1
        if i==n-1 or svec[i] != svec[i+1]:
            averank = sumranks / float(dupcount) + 1
            for j in range(i-dupcount+1,i+1):
                newarray[ivec[j]] = averank
            sumranks = 0
            dupcount = 0
    return newarray

def Spearman(srcFlowTxt,destFlowTxt):
	srcFlow = open(srcFlowTxt)
	destFlow = open(destFlowTxt)
	srcFlowData = []
	destFlowData = []
#Dependent Variable
	for data in srcFlow:
		data = data.strip('\n')
		srcFlowData.append(data)
	print("srcData",srcFlowData)
#Independent Variable
	for data in destFlow:
		data = data.strip('\n')
		destFlowData.append(data)
	print("destData",destFlowData)
#Rank Dependent Variable
	srcRank = rankdata(srcFlowData)
	print("srcRank",srcRank)
#Rank Independent Variable
	destRank = rankdata(destFlowData)
	print("destRank",destRank)
#Difference in Rank
	difference = []
	for i in range(len(srcRank)):
		difference.append(abs(srcRank[i] - destRank[i]))
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