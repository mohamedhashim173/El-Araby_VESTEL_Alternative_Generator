/*
 * vestel_program.c
 *
 *  Created on: Jul 1, 2023
 *      Author: Mohamed Hashim Attiya Said
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "STD_TYPES.h"
#include "vestel_interface.h"

// macros
#define BUFFER_LENGTH 255
#define COMPONENT_LEN (codeDigitLength(component) + 1)

/****************************************** Source Data Array Funciotns ******************************************/

FILE *sourceData_getFilePath(char *source_file_path)
{
	printf("\n-----------------------------------------------------------------\n");
	printf(">> Starting VESTEL Substitute Table Generator by Mohamed Hashim:");
	printf("\n-----------------------------------------------------------------\n\n");
	printf("* Restrictions:\n");
	printf("  - sourceFile.txt and programFile.exe MUST be located in the same directory.\n");
	printf("  - sourceFile.txt is a text file that has 2 columns of numbers (only numbers), separated by a Tab character.\n");
	printf("  - Use ISNUMBER() excel function to make sure that all spare parts' codes are numbers.\n");

	// get file name
	printf("\n* Source file name (ex: source.txt): ");
	scanf("%s", source_file_path);

	// open file
	return fopen(source_file_path, "r");
}

u32 sourceData_getNumberOfRows(FILE *filePtr_source)
{
	u32 rows = 0;
	u32 component;
	char buffer[BUFFER_LENGTH];

	fseek(filePtr_source, 0, SEEK_SET);
	while (fgets(buffer, BUFFER_LENGTH, filePtr_source))
	{
		component = atoi(buffer);

		if (component == 0)
		{
			continue;
		}

		rows++;
	}

	return rows;
}

void sourceData_importData(FILE *filePtr_source, u32 **sourceDataArray)
{
	printTimeNow();
	printf("1. Loading database...\t\t\t");

	u32 index = 0;
	u32 component, alt;
	char buffer[BUFFER_LENGTH];

	fseek(filePtr_source, 0, SEEK_SET);
	while (fgets(buffer, BUFFER_LENGTH, filePtr_source))
	{
		component = atoi(buffer);
		alt = atoi(&buffer[COMPONENT_LEN]);

		if (component == 0)
		{
			continue;
		}

		// fill source data array
		sourceDataArray[index][0] = component;
		sourceDataArray[index][1] = alt;

		index++;
	}

	printf("Done!");
}

void sourceData_getMinMaxVal(u32 **sourceDataArray, u32 sourceData_rows, u32 *code_min_val, u32 *code_max_val)
{
	for (u32 i = 0; i < sourceData_rows; i++)
	{
		*code_max_val = fmax(*code_max_val, sourceDataArray[i][0]);
		*code_max_val = fmax(*code_max_val, sourceDataArray[i][1]);

		*code_min_val = fmin(*code_min_val, sourceDataArray[i][0]);
		*code_min_val = fmin(*code_min_val, sourceDataArray[i][1]);
	}
}

void sourceData_print(u32 **sourceDataArray, u32 sourceData_rows)
{
	for (u32 i = 0; i < sourceData_rows; i++)
	{
		printf("row %3lu:  %lu\t%lu\n", i + 1, sourceDataArray[i][0], sourceDataArray[i][1]);
	}

	printf("sourceData_print() completed!\n");
}

/****************************************** Unique Array Funciotns ******************************************/

void hashTable_fill(component_t *hashTable, u32 **sourceDataArray, u32 sourceData_rows)
{
	for (u32 i = 0; i < sourceData_rows; i++)
	{
		hashTable[sourceDataArray[i][0]].comp = sourceDataArray[i][0];
		hashTable[sourceDataArray[i][1]].comp = sourceDataArray[i][1];
	}
}

void hashTable_print(component_t *hashTable, u32 code_min_val, u32 hashTableSize)
{
	u32 c = 0;

	printf("\ncount\tid\tcomponent\n");
	for (u32 i = code_min_val; i < hashTableSize; i++)
	{
		if (hashTable[i].comp != 0)
		{
			printf("%2lu:\t(%lu)\t%lu\n", ++c, hashTable[i].id, hashTable[i].comp);
		}
	}

	printf("hashTable_print() completed!\n");
}

u32 uniqueArray_getSize(component_t *hashTable, u32 *code_min_val, u32 *code_max_val)
{
	u32 uniqueArraySize = 0;
	for (u32 i = *code_min_val; i <= *code_max_val; i++)
	{
		if (hashTable[i].comp != 0)
		{
			uniqueArraySize++;
		}
	}

	return uniqueArraySize;
}

void uniqueArray_fill(component_t *hashTable, u32 hashTableSize, component_t *uniqueArray)
{
	for (u32 i = 0; i < hashTableSize; i++)
	{
		static u32 index = 0;
		if (hashTable[i].comp != 0)
		{
			uniqueArray[index++].comp = hashTable[i].comp;
		}
	}
}

void uniqueArray_print(component_t *uniqueArray, u32 uniqueArraySize)
{
	printf("\ncount\tid\tcomponent\n");
	for (u32 i = 0; i < uniqueArraySize; i++)
	{
		printf("%2lu:\t(%lu)\t%lu\n", i + 1, uniqueArray[i].id, uniqueArray[i].comp);
	}
	printf("uniqueArray_print() completed!\n");
}

/*********************************** Alternatives Generation Funciotns ***********************************/

void alts_generateID(component_t *uniqueArray, u32 uniqueArraySize, u32 **sourceDataArray, u32 sourceData_rows)
{
	printTimeNow();
	printf("2. Generating Alternatives...\t\t    ");

	long key = 0;
	u32 uniqueVal, component, alt;
	int component_index, alt_index;
	int counter;

	// loop over unique values array
	for (u32 i = 0; i < uniqueArraySize; i++)
	{
		key++;
		uniqueVal = uniqueArray[i].comp;

		if (uniqueArray->id == 0)
		{
			uniqueArray->id = key;
		}

		// loop over source data array
		for (u32 j = 0; j < sourceData_rows; j++)
		{
			component = sourceDataArray[j][0];
			alt = sourceDataArray[j][1];

			component_index = binarySearch(uniqueArray, 0, uniqueArraySize - 1, component);
			alt_index = binarySearch(uniqueArray, 0, uniqueArraySize - 1, alt);

			// compare unique value with the read value from source file
			if (uniqueVal == component || uniqueVal == alt)
			{
				if (uniqueArray[component_index].id == 0 && uniqueArray[alt_index].id == 0)
				{
					uniqueArray[component_index].id = uniqueArray[alt_index].id = key;
				}
				else if (uniqueArray[component_index].id != 0 && uniqueArray[alt_index].id == 0)
				{
					uniqueArray[alt_index].id = uniqueArray[component_index].id;
				}
				else if (uniqueArray[component_index].id == 0 && uniqueArray[alt_index].id != 0)
				{
					uniqueArray[component_index].id = uniqueArray[alt_index].id;
				}
			}
		}

		// compare IDs
		u32 uniqueID, componentID, altID;
		do
		{
			counter = 0;
			for (u32 j = 0; j < sourceData_rows; j++)
			{
				component = sourceDataArray[j][0];
				alt = sourceDataArray[j][1];

				component_index = binarySearch(uniqueArray, 0, uniqueArraySize - 1, component);
				alt_index = binarySearch(uniqueArray, 0, uniqueArraySize - 1, alt);

				uniqueID = uniqueArray[i].id;
				componentID = uniqueArray[component_index].id;
				altID = uniqueArray[alt_index].id;

				if ((componentID != altID) && (uniqueID == componentID || uniqueID == altID))
				{
					counter++;
					uniqueArray[component_index].id = uniqueArray[alt_index].id = uniqueArray[i].id;
				}
			}

		} while (counter != 0);

		printProgressPercentage((f64)i, (f64)uniqueArraySize);
	}

	printf("\b\b\b\b\b Done!");
}

int alts_exportMatrix(component_t *uniqueArray, u32 uniqueArraySize)
{
	printTimeNow();
	printf("3. Exporting Alternatives...\t\t    ");

	// export file
	FILE *filePtr_destination = fopen("vestel_export.txt", "w+");
	if (filePtr_destination == NULL)
	{
		return -1;
	}

	for (u32 i = 0; i < uniqueArraySize; i++)
	{
		fprintf(filePtr_destination, "%ld\t", uniqueArray[i].comp);

		for (u32 j = 0; j < uniqueArraySize; j++)
		{
			if (uniqueArray[i].id == uniqueArray[j].id)
			{
				if (uniqueArray[j].comp == uniqueArray[i].comp)
				{
					continue;
				}

				fprintf(filePtr_destination, "%ld\t", uniqueArray[j].comp);
			}
		}

		fprintf(filePtr_destination, "\n");

		printProgressPercentage((f64)i, (f64)uniqueArraySize);
	}

	fclose(filePtr_destination);

	printf("\b\b\b\b\b Done!");

	return 0;
}

void alts_printMatrix(component_t *uniqueArray, u32 uniqueArraySize)
{
	printf("\n");

	for (u32 i = 0; i < uniqueArraySize; i++)
	{
		printf("%lu\t", uniqueArray[i].comp);

		for (u32 j = 0; j < uniqueArraySize; j++)
		{
			if (uniqueArray[i].id == uniqueArray[j].id)
			{
				if (uniqueArray[j].comp == uniqueArray[i].comp)
				{
					continue;
				}

				printf("%lu\t", uniqueArray[j].comp);
			}
		}

		printf("\n");
	}
}

/****************************************** Other Funciotns ******************************************/

int binarySearch(component_t *arr, int l, int r, u32 component)
{
	while (l <= r)
	{
		int m = l + (r - l) / 2;

		// Check if component is present at mid
		if (arr[m].comp == component)
			return m;

		// If component greater, ignore left half
		if (arr[m].comp < component)
			l = m + 1;

		// If component is smaller, ignore right half
		else
			r = m - 1;
	}

	// If we reach here, then element was not present
	return -1;
}

char *printTimeNow(void)
{
	time_t now = time(NULL);
	char *time = asctime(gmtime(&now));

	time = &time[11];			   // remove date
	time[strlen(time) - 6] = '\0'; // Remove year and \n

	printf("\n(GMT) %s\t", time);

	return time;
}

void printProgressPercentage(f64 val, f64 maxVal)
{
	// percentage of completion
	f64 percent = val * (f64)100 / maxVal;
	printf("\b\b\b\b\b\b%5.0f%%", percent);
}

int codeDigitLength(u32 code)
{
	int c = 0;
	do
	{
		code /= 10;
		++c;
	} while (code != 0);

	return c;
}

void aboutTheDev(void)
{
	printf("\n\n----------------------------------------------\n");
	printf("Developer: Mohamed Hashim\n");
	printf("E-mail   : Mohamed-Hashm@elarabygroup.com\n");
}

void pressEnterToExit(void)
{
	int x;
	do
	{
		scanf("%d", &x);
	} while (0);
}
