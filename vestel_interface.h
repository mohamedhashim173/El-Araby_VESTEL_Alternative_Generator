/*
 * vestel_interface.h
 *
 *  Created on: Jul 1, 2023
 *      Author: Mohamed Hashim Attiya Said
 */

#ifndef VESTEL_INTERFACE_H_
#define VESTEL_INTERFACE_H_

// source data related functions
FILE *sourceData_getFilePath(char *source_file_path);
u32 sourceData_getNumberOfRows(FILE *filePtr_source);
void sourceData_importData(FILE *filePtr_source, u32 **sourceDataArray);
void sourceData_getMinMaxVal(u32 **sourceDataArray, u32 sourceData_rows, u32 *code_min_val, u32 *code_max_val);
void sourceData_print(u32 **sourceDataArray, u32 sourceData_rows);

// hashing and unique array functions
void hashTable_fill(component_t *hashTable, u32 **sourceDataArray, u32 sourceData_rows);
void hashTable_print(component_t *hashTable, u32 code_min_val, u32 hashTableSize);
u32 uniqueArray_getSize(component_t *hashTable, u32 *code_min_val, u32 *code_max_val);
void uniqueArray_fill(component_t *hashTable, u32 hashTableSize, component_t *uniqueArray);
void uniqueArray_print(component_t *uniqueArray, u32 uniqueArraySize);

// generation of alternatives functions
void alts_generateID(component_t *uniqueArray, u32 uniqueArraySize, u32 **sourceDataArray, u32 sourceData_rows);
int alts_exportMatrix(component_t *uniqueArray, u32 uniqueArraySize);
void alts_printMatrix(component_t *uniqueArray, u32 uniqueArraySize);

// other functions
int binarySearch(component_t *arr, int l, int r, u32 component);
int codeDigitLength(u32 code);
void printProgressPercentage(f64 val, f64 maxVal);
char *printTimeNow(void);
void aboutTheDev(void);
void pressEnterToExit(void);

#endif /* VESTEL_INTERFACE_H_ */
