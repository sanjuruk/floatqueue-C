#include <FloatQueue.h>

int GetFirstDataPos(circular_buffer* cbuf)
{
	return ((cbuf->lastBufferPos + 1) == cbuf->BUFFER_SIZE) ? 0 : cbuf->lastBufferPos + 1;
}

int GetLastDataPos(circular_buffer* cbuf)
{
	return (cbuf->firstBufferPos == 0) ? cbuf->BUFFER_SIZE - 1 : cbuf->firstBufferPos - 1;
}

int Count(circular_buffer* cbuf)
{
	int temp = cbuf->firstBufferPos - cbuf->lastBufferPos - 1;

	if (temp < 0)
	{
		return temp + cbuf->BUFFER_SIZE;
	}
	else
	{
		return temp;
	}
}

int CountUntilACertainIndex(circular_buffer* cbuf, int index)
{
	int temp = index - cbuf->lastBufferPos;
	if (temp < 0)
	{
		return temp + cbuf->BUFFER_SIZE;
	}
	else
	{
		return temp;
	}
}

int EmptyCount(circular_buffer* cbuf)
{
	return cbuf->BUFFER_SIZE - 1 - Count(cbuf);
}

void EnqueueOne(circular_buffer* cbuf, float data)
{
	/*Buffer Full*/
	if (1 > EmptyCount(cbuf)){
		return;
	}
	
	cbuf->buffer[cbuf->firstBufferPos] = data;
	cbuf->firstBufferPos++;
	cbuf->firstBufferPos %= cbuf->BUFFER_SIZE;
}

void Enqueue(circular_buffer* cbuf, float* data, size_t addSize)
{

	if (addSize > EmptyCount(cbuf)){
		return; /*Buffer Full*/
	}

	int lastEmptySize = cbuf->BUFFER_SIZE - cbuf->firstBufferPos;

	if (lastEmptySize > addSize)
	{
		memcpy(cbuf->buffer + cbuf->firstBufferPos, data, addSize * sizeof(float));
		cbuf->firstBufferPos += addSize;
	}
	else
	{
		memcpy(cbuf->buffer + cbuf->firstBufferPos, data, lastEmptySize * sizeof(float));
		memcpy(cbuf->buffer, data + lastEmptySize, (addSize - lastEmptySize) * sizeof(float));
		cbuf->firstBufferPos = addSize - lastEmptySize;
	}
}

void EnqueueFromCertainIndex(circular_buffer* cbuf, float* data, int index, int length)
{
	float* temp = malloc(length * sizeof(float));
	memcpy(temp, data + index, length * sizeof(float));
	Enqueue(cbuf, temp, length);
	free(temp);
}

float* DequeueCertainSize(circular_buffer* cbuf, int size)
{
	float* data = GetDataCertainSize(cbuf, size);

	if(data != NULL){
		cbuf->lastBufferPos += size;
		if (cbuf->lastBufferPos >= cbuf->BUFFER_SIZE)
			cbuf->lastBufferPos -= cbuf->BUFFER_SIZE;
	}

	return data;
}

float* DequeueUntilACertainIndex(circular_buffer* cbuf, int index, int* dequedBufferSize)
{
	int dequeuingBufferSize = CountUntilACertainIndex(cbuf, index);
	*dequedBufferSize = dequeuingBufferSize;
	return DequeueCertainSize(cbuf, dequeuingBufferSize);
}

float* Dequeue(circular_buffer* cbuf, int* dequedBufferSize)
{
	int dequeuingBufferSize = Count(cbuf);
	*dequedBufferSize = dequeuingBufferSize;
	return DequeueCertainSize(cbuf, dequeuingBufferSize);
}

float* GetData(circular_buffer* cbuf)
{
	return GetDataCertainSize(cbuf, Count(cbuf));
}

float* GetDataCertainSize(circular_buffer* cbuf, int size)
{
	return GetDataCetainSizeAndFromIndex(cbuf, 0, size);
}

float* GetDataCetainSizeAndFromIndex(circular_buffer* cbuf, int startIndex, int size)
{
	float* data;
	//float data[size * sizeof(float)];// = malloc(size * sizeof(float));
	if (size <= 0)
		return NULL;

	int availableCount = Count(cbuf);
	if (size + startIndex > availableCount){
		return NULL; /*Exception: buffer is empty*/
	}

	int firstDataPos =  GetFirstDataPos(cbuf);
	//int lastDataPos = GetLastDataPos(cbuf);
	int lastDataLength = cbuf->BUFFER_SIZE - firstDataPos;

	data = (float*)malloc(size * sizeof(float));

	if (data == NULL) {
		debugPrintln("Float Queue NULL malloc buffer");
		free(data);
		return NULL;
	}

	if (size + startIndex <= lastDataLength)
	{
		memcpy(data, cbuf->buffer + (firstDataPos + startIndex), size * sizeof(float));
	}
	else
	{
		if (startIndex < lastDataLength)
		{
			memcpy(data, cbuf->buffer + (firstDataPos + startIndex), (lastDataLength - startIndex) * sizeof(float));
			memcpy(data + (lastDataLength - startIndex), cbuf->buffer, (size - lastDataLength + startIndex) * sizeof(float));
		}
		else
		{
			memcpy(data, cbuf->buffer + (startIndex - lastDataLength), size * sizeof(float));
		}
	}

	return data;
}

float* GetCertainDataFromBegining(circular_buffer* cbuf, int size)
{
	float* data;

	if (size <= 0)
		return NULL;

	if(size >  cbuf->BUFFER_SIZE)
		return NULL;

	data = (float*)malloc(size * sizeof(float));
	if (data == NULL) {
		debugPrintln("Float Queue NULL malloc buffer");
		free(data);
		return NULL;
	}

	memcpy(data, cbuf->buffer, size * sizeof(float));

	return data;
}

float* DequeueCertainSizeFromLastData(circular_buffer* cbuf, int size)
{
	float* data = GetDataCetainSizeAndFromIndex(cbuf, cbuf->lastBufferPos ,size);

	if(data != NULL){
		cbuf->lastBufferPos += size;
		if (cbuf->lastBufferPos >= cbuf->BUFFER_SIZE)
			cbuf->lastBufferPos -= cbuf->BUFFER_SIZE;
	}

	return data;
}

float GetAverage(circular_buffer* cbuf){
	int size = 0;
	float* data = Dequeue(cbuf, &size);

	float total=0.0;
	float validDataSize = 0.0;

	if(size == 0 || data == NULL){
		return NAN;
	}

	for(int i = 0; i < size; i++){
		if(!isnan(data[i])){
			total = total + data[i];
			validDataSize++;
		}
		else{
			debugPrintln("Get Average Null");
		}
	}

	if(total == 0.0 || validDataSize == 0.0){
		free(data);
		return NAN;
	}

	float avg = (total / validDataSize);

	free(data);
	return avg;
}

void InitBufferSize(circular_buffer* cbuf, float* buffer, int size)
{
	cbuf->BUFFER_SIZE = size;

	cbuf->buffer = buffer;

	cbuf->firstBufferPos = 0;

	cbuf->lastBufferPos = size - 1;
}

int IncreasePosition(circular_buffer* cbuf, int i) //ref int i
{
	i++;
	if (i == cbuf->BUFFER_SIZE)
		i = 0;

	return i;
}
