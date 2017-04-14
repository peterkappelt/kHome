/**
 * @file kBuffer.h
 * @author Peter Kappelt
 * @brief A universal ringbuffer library
 * @see https://github.com/peterkappelt/kBuffer
 * @copyright Peter Kappelt 2016; MIT License (see LICENSE.txt in the root of this repository)
 */

#ifndef KBUFFER_H
#define KBUFFER_H

#ifdef __cplusplus
extern "C"{
#endif

#include "../khTelegram.h"

#include <stdio.h>
#include <stdint.h>

/**
 * @brief The datatype of one buffer element.
 * As default, it is an 16 bit unsigned integer. Feel free to change it to your needs
 */
#ifndef bufferDatatype
	#define bufferDatatype uint16_t
#endif

/**
 * @brief Enables mean/ averaging functions. 
 * If you uncomment this define, the following functions will be compiled.<br>
 * Only enable it, if bufferDatatype is some sort of numeric datatype (integer, float, ...)<br>
 * bufferMean();<br>
 * bufferMeanRMS();<br>
 */
//#define BUFFER_ENABLE_MEAN

/**
 * @brief Struct for buffer handling.
 * If you need a ringbuffer in your software, you should instantiate a buffer_t, and run the neccessary functions with a pointer to your instance
 */
typedef struct{
    /// is 0 if the buffer is not initialized
    uint8_t isInitialized;
    /// The write pointer of the buffer. At a write procedure, data gets written and the pointer is incremented
    uint16_t writePointer;
    /// The read pointer of the buffer. At a read procedure, data gets read and the pointer is incremented
    uint16_t readPointer;
    /// The number of elements in the buffer
    uint16_t length;
    /// The number of bytes of one buffer element. The total memory consumption in Bytes is equal to length * elementLength
    uint8_t elementLength;  
    /// A variable which is increased by one when new data gets written and decremented by one when data is read
    uint16_t datacount;
    /// A pointer to the first element of the buffer. Length * elementLength bytes of memory are allocated after this pointer
    bufferDatatype* data;   
}buffer_t;

/**
 * @enum bufferStatus_t
 * @brief buffer function return codes
 */
typedef enum{
    /** it seems, as everything worked as expected */
    bufferOK = 0,
    /** happens while allocating memory,<br>there is not enough free memory (->malloc failed) */
    bufferMemoryAllocationFailed,
    /** happens at reading data,<br>buffer is empty and there is no more data to read */
    bufferEmpty,
    /** happens at writing data,<br>buffer is full, no more data can be written */
    bufferFull,
    /** The buffer is not initialized */
    bufferNotInitialized,
    /** an error occured, which isn't explained nearer. Have a look at the according function */
    bufferError
}bufferStatus_t;


//Function declearations

bufferStatus_t bufferInit(buffer_t* buffer, uint16_t bufferSize);
bufferStatus_t bufferInitStatic(buffer_t* buffer, uint16_t bufferSize, bufferDatatype* bufferArray);
bufferStatus_t bufferWriteToIndex(buffer_t* buffer, uint16_t index, bufferDatatype data);
bufferStatus_t bufferReadFromIndex(buffer_t* buffer, uint16_t index, bufferDatatype* data);
uint8_t bufferIsEmpty(buffer_t* buffer);
uint8_t bufferIsFull(buffer_t* buffer);
bufferStatus_t bufferWrite(buffer_t* buffer, bufferDatatype data);
bufferStatus_t bufferWriteOverwrite(buffer_t* buffer, bufferDatatype data);
bufferStatus_t bufferRead(buffer_t* buffer, bufferDatatype* data);
bufferStatus_t bufferPeek(buffer_t* buffer, bufferDatatype* data);
bufferStatus_t bufferFill(buffer_t* buffer, bufferDatatype data, uint8_t silent);
bufferStatus_t bufferAvailable(buffer_t* buffer, uint16_t* available);

#ifdef BUFFER_ENABLE_MEAN
bufferStatus_t bufferMean(buffer_t* buffer, bufferDatatype* meanOut);
bufferStatus_t bufferMeanRMS(buffer_t* buffer, bufferDatatype* meanOut);
#endif

#ifdef __cplusplus
}
#endif

#endif