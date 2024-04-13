#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_MAP_SIZE 37
#define NAME_LENGTH 20

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Fill out this structure
struct HashType
{
    struct RecordType *record;
    struct HashType *next;
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_MAP_SIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;
    struct HashType *temp;
    for (i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].record != NULL)
        {
            temp = &pHashArray[i];
            printf("index %d", i);
            while (temp != NULL)
            {
                printf(" -> ");
                printf("%d %c %d", temp->record->id, temp->record->name, temp->record->order);
                temp = temp->next; 
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("C:\\Users\\aisid\\Downloads\\lab9\\input_lab_9.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    struct HashType *hashMap = malloc(sizeof(struct HashType) * HASH_MAP_SIZE);
    for (int i = 0; i < HASH_MAP_SIZE; i++)
    {
        hashMap[i].next = NULL;
        hashMap[i].record = NULL;
    }
    int index;
    for (int i = 0; i < recordSz; i++)
    {
        index = hash(pRecords[i].id);

        if (hashMap[index].record == NULL)
        {
            // Allocate memory for the new record
            hashMap[index].record = (struct RecordType *)malloc(sizeof(struct RecordType));
            // Copy data to the record
            hashMap[index].record->id = pRecords[i].id;
            hashMap[index].record->name = pRecords[i].name;
            hashMap[index].record->order = pRecords[i].order;
        }
        else
        {
            struct HashType *temp = &hashMap[index];
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = (struct HashType *)malloc(sizeof(struct HashType));
            temp->next->record = (struct RecordType *)malloc(sizeof(struct RecordType));
            temp->next->record->id = pRecords[i].id;
            temp->next->record->name = pRecords[i].name;
            temp->next->record->order = pRecords[i].order;
            temp->next->next = NULL;
        }
    }

    displayRecordsInHash(hashMap, HASH_MAP_SIZE);

    for (int i = 0; i < HASH_MAP_SIZE; i++)
    {
        struct HashType *current = &hashMap[i];
        struct HashType *next;
        while (current != NULL)
        {
            next = current->next;
            free(current->record);
            current = next;
        }
    }
	free(hashMap);
    free(pRecords);

    return 0;
}
