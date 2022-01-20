#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	float* buffer;
	size_t firstBufferPos;
	size_t lastBufferPos;
	size_t BUFFER_SIZE;
} circular_buffer;

int Count(circular_buffer* cbuf);
int EmptyCount(circular_buffer* cbuf);
int GetLastDataPos(circular_buffer* cbuf);
void EnqueueOne(circular_buffer* cbuf, float data);
void Enqueue(circular_buffer* cbuf, float* data, size_t addSize);
void EnqueueFromCertainIndex(circular_buffer* cbuf, float* data, int index, int length);
float* DequeueCertainSize(circular_buffer* cbuf, int size);
//uint8_t* Dequeue(circular_buffer* cbuf);
float* DequeueUntilACertainIndex(circular_buffer* cbuf, int index, int* dequedBufferSize);
float* Dequeue(circular_buffer* cbuf, int* dequedBufferSize);
float* GetData(circular_buffer* cbuf);
float* GetDataCertainSize(circular_buffer* cbuf, int size);
float* GetDataCetainSizeAndFromIndex(circular_buffer* cbuf, int startIndex, int size);
void InitBufferSize(circular_buffer* cbuf, float* buffer, int size);

float* GetCertainDataFromBegining(circular_buffer* cbuf, int size);
float* DequeueCertainSizeFromLastData(circular_buffer* cbuf, int size);

float GetAverage(circular_buffer* cbuf);
