/*
 * main.c
 *
 *  Created on: Jul 1, 2023
 *      Author: Mohamed Hashim Attiya Said
 */

#include <stdio.h>
#include <stdlib.h>
#include "STD_TYPES.h"
#include "vestel_interface.h"

void main(void)
{
	/****************************** Get Data Source ******************************/

	FILE *filePtr_source;
	char source_file_path[500];
	filePtr_source = sourceData_getFilePath(source_file_path);
	if (filePtr_source == NULL)
	{
		printf("\n(x) Source file doesn't exist! Program terminated.\n\n");
		pressEnterToExit();
		return;
	}

	/****************************** Source Data Array ******************************/

	// create source data array
	u32 code_min_val = 0xFFFFFFFF;
	u32 code_max_val = 0;

	u32 sourceData_rows = sourceData_getNumberOfRows(filePtr_source);

	u32 **sourceDataArray = malloc(sourceData_rows * sizeof(u32 *));
	for (u32 i = 0; i < sourceData_rows; i++)
	{
		sourceDataArray[i] = malloc(2 * sizeof(u32));
	}

	// 1. fill source data array
	sourceData_importData(filePtr_source, sourceDataArray);

	// min and max values
	sourceData_getMinMaxVal(sourceDataArray, sourceData_rows, &code_min_val, &code_max_val);

	// close source file
	fclose(filePtr_source);

	// test
	// printf("\nmin = %ld, max = %ld, rows = %ld\n", code_min_val, code_max_val, sourceData_rows);
	// sourceData_print(sourceDataArray, sourceData_rows);
	// return;

	/****************************** Unique Array ******************************/

	// create and fill hashTable
	u32 hashTableSize = code_max_val + 1;
	component_t *hashTable = calloc(hashTableSize, sizeof(component_t));
	hashTable_fill(hashTable, sourceDataArray, sourceData_rows);

	// create and fill unique array
	u32 uniqueArraySize = uniqueArray_getSize(hashTable, &code_min_val, &code_max_val);
	component_t *uniqueArray = calloc(uniqueArraySize, sizeof(component_t));
	uniqueArray_fill(hashTable, hashTableSize, uniqueArray);

	// test
	// uniqueArray_print(uniqueArray, uniqueArraySize);
	// return;

	/****************************** Generate Alternatives ******************************/

	// 2. set id
	alts_generateID(uniqueArray, uniqueArraySize, sourceDataArray, sourceData_rows);

	// test
	// uniqueArray_print(uniqueArray, uniqueArraySize);
	// alts_printMatrix(uniqueArray, uniqueArraySize);
	// return;

	// 3. export matrix to txt file
	int res = alts_exportMatrix(uniqueArray, uniqueArraySize);
	if (res == -1)
	{
		printf("\n(x) Cannot create export file! Program terminated.\n\n");
		pressEnterToExit();
		return;
	}

	/******************************** End of Program ********************************/

	printTimeNow();
	printf("Program completed 100%%. File exported successfully!\n\n");
	aboutTheDev();
	pressEnterToExit();
}
