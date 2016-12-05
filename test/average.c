
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
	int divValue = 10; // 1/100 of a second.
	
	long* averageArray = calloc(numSeconds * divValue, sizeof(long));
	
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
	
	char outputFile[128];
	
	FILE* file = fopen( fileName , "r");
	
	sprintf(outputFile, "%s.average", fileName);
	FILE* output = fopen(outputFile, "w+");
	
	// Read first line to get initial time value.
	if ((read = getline(&line, &len, file)) != -1)
	{
		sscanf(line , "%ld\t%d\n", &startTime, &packetSize);
		
		averageArray[0] = packetSize;
		totalSize += packetSize;
	}
	
	while ((read = getline(&line, &len, file)) != -1) 
	{
		sscanf(line , "%ld\t%d\n", &timeValue, &packetSize);
		
		averageArray[ (timeValue - startTime) / (1000000 / divValue)] += packetSize;
		totalSize += packetSize;
	}
	
	for(int i = 0; i < numSeconds * divValue; i++)
	{
		fprintf(output, "%d\t%ld\n", i, averageArray[i]);
	}
	
	printf("Total Size: %ld\n", totalSize);
	
	free(line);
	free(averageArray);
	fclose(file);
	fclose(output);
	
	return 0;
}



