#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

int keyIndex(Dictionary* d, int key);
int dictionaryManager(Dictionary* d);
void swap(int* x, int* y);
void bubbleSort(int arr[], int size);

struct Dictionary {
	// Pointers to an array of keys and to an array of values
	int *keys, *values;
	// Size of both arrays above
	int size;
	// Top index of both arrays above
	int topIndex;
};

/**************************************************************************************************
* Function Name: initDictionary
* Input: none
* Output: Dictionary* (pointer to Dictionary)
* Function Operation: The function allocates memory for an empty Dictionary and returns
*                     a pointer to it.
***************************************************************************************************/
Dictionary* initDictionary() {
	// Create pointer to Dictionary
	Dictionary* ptr;
	// Allocating memory to the pointer
	ptr = (Dictionary*)malloc(sizeof(Dictionary));
	if (ptr == NULL) {
		printf("Heap memory error! file: %s function: %s", __FILE__, __func__);
		return NULL;
	}
	// Allocating memory to an array of keys
	ptr->keys = (int*)malloc(sizeof(int));
	if (ptr->keys == NULL)
		printf("Heap memory error! file: %s function: %s", __FILE__, __func__);
	// Allocating memory to an array of values
	ptr->values = (int*)malloc(sizeof(int));
	if (ptr->values == NULL)
		printf("Heap memory error! file: %s function: %s", __FILE__, __func__);
	// Define the size of both keys and values arrays
	ptr->size = 1;
	// Define the top index of both keys and values arrays
	ptr->topIndex = -1;
	return ptr;
}

/************************************************************************
* Function Name: destroyDictionary
* Input: Dictionary* d
* Output: none
* Function Operation: The function release Dictionary's allocated memory.
*************************************************************************/
void destroyDictionary(Dictionary* d) {
	// Release values array
	free(d->values);
	// Release keys array
	free(d->keys);
	// Release dictionary
	free(d);
}

/************************************************************************
* Function Name: sizeOfDictionary
* Input: Dictionary* d
* Output: int size
* Function Operation: The function returns array's length.
                      (keys and values arrays are both in the same size)
*************************************************************************/
int sizeOfDictionary(Dictionary* d) {
	return d->topIndex + 1;
}

/******************************************************************************************
* Function Name: keyIndex
* Input: Dictionary* d, int key
* Output: int index
* Function Operation: The function returns key's index. If key is not exist in keys array,
*					  the function will return -1.
*******************************************************************************************/
int keyIndex(Dictionary* d, int key) {
	for (int i = 0; i <= d->topIndex; i++) {
		if (key == d->keys[i])
			return i;
	}
	return -1;
}

/****************************************************************************************
* Function Name: dictionaryManager
* Input: Dictionary* d
* Output: int successed (1) / failed (0)
* Function Operation: The function reallocate the capacity of dictionary's arrays-
*                     keys and values. If the reallocating failed the function returns 0
*                     If the reallocating successed the function returns 1.
*****************************************************************************************/
int dictionaryManager(Dictionary* d) {
	int capacity = d->size, len = sizeOfDictionary(d), newSize, *ptr, result = 1;
	// if both arrays are empty
	if (0 == len)
		return 1;
	// if both arrays are full
	else if (len == capacity) {
		// Define new size for the arrays
		newSize = 2 * capacity * sizeof(int);
		// Allocating keys array
		ptr = (int*)realloc(d->keys, newSize);
		// If the allocate was successful
		if (ptr != NULL) {
			// Change the size 
			d->size *= 2;
			d->keys = ptr;
		}
		// If the allocate was faild
		else {
			printf("Heap memory error! file: %s function: %s", __FILE__, __func__);
			free(ptr);
			result = 0;
		}
		// Allocating values array
		ptr = (int*)realloc(d->values, newSize);
		// If the allocate was successful
		if (ptr != NULL) 
			d->values = ptr;
		// If the allocate was faild
		else {
			printf("Heap memory error! file: %s function: %s", __FILE__, __func__);
			free(ptr);
			result = 0;
		}
	}
	// if both arrays have a lot of free memory
	else if (len == 0.5 * capacity - 1) {
		// Define new size for the arrays
		newSize = 0.5 * capacity * sizeof(int);
		// Allocating keys array
		ptr = (int*)realloc(d->keys, newSize);
		// If the allocate was successful
		if (ptr != NULL) {
			// Change the size 
			d->size = (int)(0.5 * d->size);
			d->keys = ptr;
		}
		// If the allocate was faild
		else {
			printf("Heap memory error! file: %s function: %s", __FILE__, __func__);
			free(ptr);
			result = 0;
		}
		// Allocating values array
		ptr = (int*)realloc(d->values, newSize);
		// If the allocate was successful
		if (ptr != NULL)
			d->values = ptr;
		// If the allocate was faild
		else {
			printf("Heap memory error! file: %s function: %s", __FILE__, __func__);
			free(ptr);
			result = 0;
		}
	}
	return result;
}

/********************************************************************************************
* Function Name: putInDictionary
* Input: Dictionary* d, int key, int value
* Output: Result
* Function Operation: The function gets a key and a value and puts them In the arrays white 
*                     the same index. if the key already exists in the dictionary, the value
*                     that fit to the key will be replaced with the new value
*********************************************************************************************/
Result putInDictionary(Dictionary* d, int key, int value) {
	int result;
	// If the key does not exist
	if (-1 == keyIndex(d, key)) {
		d->topIndex++;
		// Add the new key and value to the dictionary
		d->keys[d->topIndex] = key;
		d->values[d->topIndex] = value;
		// Memory check
		result = dictionaryManager(d);
		if (1 == result)
			return SUCCESS;
		else if (0 == result)
			return MEM_ERROR;
	}
	// If the key exists
	else if (0 <= keyIndex(d, key)) {
		// Replace key's value
		d->values[keyIndex(d, key)] = value;
		return SUCCESS;
	}
	return FAILURE;
}

/**********************************************************************************
* Function Name: getFromDictionary
* Input: Dictionary* d, int key
* Output: int value
* Function Operation: The function returns the value that fit to the key.
*                     if key does not exist, return 0.
***********************************************************************************/
int getFromDictionary(Dictionary* d, int key) {
	// If key does not exist
	if (-1 == keyIndex(d, key))
		return 0;
	// If the key exists
	return d->values[keyIndex(d, key)];
}

/**********************************************************************************
* Function Name: removeFromDictionary
* Input: Dictionary* d, int key
* Output: Result
* Function Operation: The function gets a key and remove him and his value from the
*                     dictionary. 
***********************************************************************************/
Result removeFromDictionary(Dictionary* d, int key) {
	int index = keyIndex(d, key), result = 1;
	// If key does not exist
	if (-1 == index)
		return FAILURE;
	// If key exists
	for (int i = index; i < d->topIndex; i++) {
		d->keys[i] = d->keys[i + 1];
		d->values[i] = d->values[i + 1];
	}
	d->topIndex--;
	result = dictionaryManager(d);
	if (0 == result)
		return MEM_ERROR;
	return SUCCESS;
}

/**********************************************************************************
* Function Name: swap
* Input: int* x, int* y
* Output: none
* Function Operation: The function gets 2 vars and swap their values.
***********************************************************************************/
void swap(int* x, int* y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

/**********************************************************************************
* Function Name: bubbleSort
* Input: int arr[], int size
* Output: none
* Function Operation: The function gets an array and sort it by using bubble sort
***********************************************************************************/
void bubbleSort(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - i; j++) {
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
		}
	}
}

/**********************************************************************************
* Function Name: printDictionary
* Input: Dictionary* d
* Output: none
* Function Operation: The function gets a dictionary and print it sorted 
***********************************************************************************/
void printDictionary(Dictionary* d) {
	int index, result = 1;
	// Create copy of d
	Dictionary* dCopy;
	dCopy = initDictionary();
	// Copy d to dCopy
	for (int i = 0; i <= d->topIndex; i++) {
		result = putInDictionary(dCopy, d->keys[i], d->values[i]);
		if (SUCCESS != result)
			continue;
	}
	bubbleSort(dCopy->keys, dCopy->topIndex);
	printf("{");
	for (int i = 0; i <= d->topIndex; i++) {
		index = keyIndex(d, dCopy->keys[i]);
		printf("[%d:%d]", dCopy->keys[i], d->values[index]);
	}
	printf("}");
	destroyDictionary(dCopy);
}

/******************************************************************************************
* Function Name: createDictionaryFromArrays
* Input: int keys[], int values[], int size
* Output: Dictionary* (pointer to Dictionary)
* Function Operation: The function creates a dictionary out of arrays of keys and values.
*******************************************************************************************/
Dictionary* createDictionaryFromArrays(int keys[], int values[], int size) {
	Dictionary* d;
	d = initDictionary();
	for (int i = 0; i < size; i++) {
		putInDictionary(d, keys[i], values[i]);
		dictionaryManager(d);
	}
	return d;
}