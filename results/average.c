
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	char* fileName = argv[1];
	int numSeconds = atoi(argv[2]);
	
	long startTime;
	long timeValue;
	int packetSize;
	
	long totalSize = 0;
	
	int divValue = 1; // 1/100 of a second.
	
	int arraySize = numSeconds * divValue;
	int index; 
	
	long* averageArray = calloc(arraySize, sizeof(long));
	
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
	
	char outputFile[128];
	
	FILE* file = fopen( fileName , "r");
	
	sprintf(outputFile, "%s.average", fileName);
	FILE* output = fopen(outputFile, "w+");
	FILE* allFlows = fopen("all.csv", "a");
	
	// Read first line to get initial time value.
	if ((read = getline(&line, &len, file)) != -1)
	{
		sscanf(line , "%ld\t%d\n", &startTime, &packetSize);
		
		averageArray[0] = packetSize;
		totalSize += packetSize;
	}

	while ((read = getline(&line, &len, file)) != -1) 
	{
		if (sscanf(line , "%ld\t%d\n", &timeValue, &packetSize) != 2)
		{
			break;
		}
		
		index = (timeValue - startTime) / (1000000 / divValue);
		
		if (index >= arraySize)
		{
			break;
		}
		
		averageArray[ index ] += packetSize;
		totalSize += packetSize;
	}
	
	index = 0;
	
	while (fileName[index + 1] != 'o')
	{
		//printf("%c", fileName[index]);
		fprintf(allFlows, "%c", fileName[index++]);
	}
	fprintf(allFlows, ",");
	
	for(int i = 0; i < numSeconds * divValue; i++)
	{
		fprintf(output, "%ld\n", averageArray[i]);
		fprintf(allFlows, "%ld,", averageArray[i]);
	}
	
	fprintf(allFlows, "\n");
	//printf("Total Size: %ld\n", totalSize);
	
	free(line);
	free(averageArray);
	fclose(file);
	fclose(output);
	fclose(allFlows);
	
	return 0;
}



