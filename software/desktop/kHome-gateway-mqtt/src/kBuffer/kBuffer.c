/**
 * @file kBuffer.c
 * @author Peter Kappelt
 * @brief A universal ringbuffer library
 * @see https://github.com/peterkappelt/kBuffer
 * @copyright Peter Kappelt 2016; MIT License (see LICENSE.txt in the root of this repository)
 */

/**
 * @mainpage
 * @section intro Introduction
 * kBuffer is a universal library for a ring- / circular buffer.
 * @section function Functions and Datatypes
 * buffer_t<br>
 * #bufferStatus_t<br>
 * <br>
 * bufferInit()<br>
 * bufferInitStatic()<br>
 * <br>
 * bufferWrite()<br>
 * bufferWriteOverwrite()<br>
 * bufferRead()<br>
 * bufferPeek()<br>
 * <br>
 * bufferFill()
 * <br>
 * bufferIsFull()<br>
 * bufferIsEmpty()<br>
 * <br>
 * bufferWriteToIndex()<br>
 * bufferReadFromIndex()<br>
 * <br>
 * bufferAvailable()<br>
 * <br>
 * bufferMean()<br>
 * bufferMeanRMS()<br>
 * @section usage Usage
 * Have a look @ref fundamental_usage for an explenation of the main kBuffer functions (with some examples)<br>
 * If you want to take the mean of your buffer, have a look at @ref mean
 * @section example Example code
 * An example code project is available under ../../test/x86. It isn't well documented, but you can compile it for your system.
 */

/**
 * @page fundamental_usage Fundamental Usage
 * @section datatype Buffer datatype definition
 * A ringbuffer consists of variables, which can be accessed in a continuous way.<br>
 * You have to define, which datatype you want to have the elements.<br>
 * By default, the elements are unsigned 16bit integers (uint16_t).<br>
 * You can change the datatype by defining it. This definition must be <b>before</b> the inclusion of th kBuffer.h header file
 * @code
 * #define bufferDatatype your_datatype
 * @endcode
 * Instead of uint16_t, you can insert (almost) any datatype you want.
 * @section init Initializing a ringbuffer
 * At first, you have to include the kBuffer library into your project.
 * This can be done by copying the files from src/kBuffer to your project's directory.
 * You can include the header as usual:
 * @code
 * #include "kBuffer.h"
 * @endcode
 * In your code, you have to define an instance of buffer_t. You have to init this instance with the function bufferInit(). If you want to have a ringbuffer with 8 elements:
 * @code
 * buffer_t ringbuffer;
 * bufferInit(&ringbuffer, 8);
 * @endcode
 * To check, if the initialization was successfull, you need to parse the return value of bufferInit():
 * @code
 * buffer_t ringbuffer;
 * if(bufferInit(&ringbuffer, 8) == bufferOK){
 *  do_something_it_worked_ok();
 * }else{
 *  do_something_there_was_an_error();
 * }
 * @endcode
 * If you want to avoid the memory overhead of the dynamic memory allocation of the malloc() function you could use the bufferInitStatic() function.
 * @code
 * buffer_t ringbuffer;
 * bufferDatatype ringbufferPayload[8];
 * bufferInitStatic(&ringbuffer, 8, &ringbufferPayload[0]);
 * @endcode
 * @section write Writing data to the buffer
 * To write data to the buffer, you can use the bufferWrite() function:
 * @code
 * #include "kBuffer.h"
 * 
 * int main(void){
 * 
 *  buffer_t ringbuffer;            // Declare an buffer instance
 *  bufferInit(&ringbuffer, 8);     // Init the buffer with 8 elements
 *  //Notice, that no errorhandling has been done. We just expect a success
 *  
 *  bufferWrite(&ringbuffer, 42);   // Write the integer "42" to the buffer.
 * 
 *  return 0;
 * }
 * @endcode
 * @section read Reading data from the buffer
 * To read data from the buffer, you can use the bufferRead() function:
 * @code
 * #include "kBuffer.h"
 * 
 * int main(void){
 * 
 *  buffer_t ringbuffer;                // Declare an buffer instance
 *  bufferInit(&ringbuffer, 8);         // Init the buffer with 8 elements
 *  //Notice, that no errorhandling has been done. We just expect a success
 *  
 *  bufferWrite(&ringbuffer, 42);       // Write the integer "42" to the buffer.
 * 
 *  uint16_t dataRead;                  // Declare an integer, where the read data should be stored
 *  bufferRead(&ringbuffer, &dataRead); // We expect, that dataRead is now 42 (because we have written 42 to the buffer before)
 * 
 *  return 0;
 * }
 * @endcode
 */

/**
 * @page mean Mean of buffer
 * @section enabling Enabling of mean functions
 * Notice: Only enable the mean functions, if the element datatype (i.e. buffer datatype) is some sort of numeric type (i.e. integer, float, ...)
 * To enable the buffer mean functions, you have to uncommented the following define in kBuffer.h:
 * @code
 * #define BUFFER_ENABLE_MEAN
 * @endcode
 * @section caution Caution!
 * There might be problems with this functions. The sum of the values (or the squared values) must be stored in a variable.<br>
 * This variable is currently a long, but under certain conditions it might overflow.<br>
 * You could replace it with an "unsigned long long" (or something smaller)
 * @section meanfunc Mean of the buffer
 * You can take the mean of the buffer with the function bufferMean():
 * @code
 * uint16_t mean;
 * 
 * bufferMean(&buffer, &mean);
 * @endcode
 * You can also get the RMS (Root Mean Square), by calling the function bufferMeanRMS() (Parameters are the same)
 */

#include "../khTelegram.h"
#include "kBuffer.h"

#include <stdlib.h>

#ifdef BUFFER_ENABLE_MEAN
    #include <math.h>
#endif

/**
 * @brief init a new buffer
 * This function inits a new buffer_t.<br>
 * @param   buffer  Pointer (&) to a buffer_t object.
 * @param   bufferSize  desired size of the buffer, the total buffer size (e.g. length-of-datatype * bufferSize) may not exceed 2^16 bytes
 * @return  an element of #bufferStatus_t
 * @retval  bufferMemoryAllocationFailed The memory allocation with malloc failed. Make sure, you have enough memory available
 * @retval  bufferOK    It seems, like everything went well
 */
bufferStatus_t bufferInit(buffer_t* buffer, uint16_t bufferSize){
    buffer->readPointer = 0;
    buffer->writePointer = 0;
    buffer->datacount = 0;
    
    buffer->elementLength = sizeof(bufferDatatype);
    buffer->data = (bufferDatatype*)malloc(buffer->elementLength * bufferSize);
    if(buffer->data != NULL){
        buffer->isInitialized = 1;
        buffer->length = bufferSize;
        //bufferFill(buffer, (bufferDatatype)0, 1);
        return bufferOK;
    }else{
        buffer->isInitialized = 0;
        return bufferMemoryAllocationFailed;
    }
}

/**
 * @brief init a new buffer
 * This function inits a new buffer_t, but doesn't allocate the memory dynamically<br>
 * You've to provide an array of the datatype and the required length when calling this function.<br>
 * This function might be useful, if you want to save the overhead of the malloc() function<br>
 * @param   buffer  Pointer (&) to a buffer_t object.
 * @param   bufferSize  desired size of the buffer, the total buffer size (e.g. length-of-datatype * bufferSize) may not exceed 2^16 bytes
 * @param   bufferArray	pointer to a array of the type bufferDatatype, which is bufferSize elements long
 * @return  an element of #bufferStatus_t
 * @retval  bufferOK    It seems, like everything went well
 */
bufferStatus_t bufferInitStatic(buffer_t* buffer, uint16_t bufferSize, bufferDatatype* bufferArray){
    buffer->readPointer = 0;
    buffer->writePointer = 0;
    buffer->datacount = 0;
    
    buffer->elementLength = sizeof(bufferDatatype);
    buffer->data = bufferArray;

    buffer->isInitialized = 1;
    buffer->length = bufferSize;
    //bufferFill(buffer, 0, 1);
    return bufferOK;
}

/**
 * @brief write data to a specific index of the buffer. 
 * WARNING: Take care when using this function, it is against the main concept of a ringbuffer
 * @param   buffer  Pointer to a buffer_t instance
 * @param   index   The index, where data should be written.<br>It can be in range 0 to length - 1
 * @param   data    The actual data which should be written
 * @return  an element of #bufferStatus_t
 * @retval  bufferOK    It went successfull
 * @retval  bufferNotInitialized    The buffer is not initialized. You have to call bufferInit before (or the init failed before)
 * @retval  bufferError The desired data index is out of range
 */
bufferStatus_t bufferWriteToIndex(buffer_t* buffer, uint16_t index, bufferDatatype data){
    if(buffer->isInitialized){
        if(index <= (buffer->length - 1)){
            *(buffer->data + (buffer->elementLength * index)) = data;
            return bufferOK;
        }else{
            return bufferError;
        }
    }else{
        return bufferNotInitialized;
    }
}

/**
 * @brief read data from a specifig index of the buffer
 * WARNING: Take care when using this function, it is against the main concept of a ringbuffer
 * @param   buffer  Pointer to a buffer_t instance
 * @param   index   The index, where data should be written.<br>It can be in range 0 to length - 1
 * @param   data    Pointer to a variable where the read data should be written to.
 * @return  an element of #bufferStatus_t
 * @retval  bufferOK    It went successfull
 * @retval  bufferNotInitialized    The buffer is not initialized. You have to call bufferInit before (or the init failed before)
 * @retval  bufferError The desired data index is out of range
 */
bufferStatus_t bufferReadFromIndex(buffer_t* buffer, uint16_t index, bufferDatatype* data){
    if(buffer->isInitialized){
        if(index <= (buffer->length - 1)){
            *data = *(buffer->data + (buffer->elementLength * index));
            return bufferOK;
        }else{
            return bufferError;
        }
    }else{
        return bufferNotInitialized;
    }
}

/**
 * @brief Checks, wheter the buffer is empty
 * @param buffer Pointer to a buffer_t instance
 * @retval  1   buffer is empty
 * @retval  0   buffer is not empty
 */
uint8_t bufferIsEmpty(buffer_t* buffer){
    if((buffer->writePointer == buffer->readPointer) && (buffer->datacount == 0)){
        return 1;
    }else{
        return 0;
    }
}

/**
 * @brief Checks, wheter the buffer is full
 * @param buffer Pointer to a buffer_t instance
 * @retval  1   buffer is full
 * @retval  0   buffer is not full
 */
uint8_t bufferIsFull(buffer_t* buffer){
    if((buffer->writePointer == buffer->readPointer) && (buffer->datacount == buffer->length)){
        return 1;
    }else{
        return 0;
    }
}

/**
 * @brief add data to the end of the ringbuffer
 * @param   buffer  pointer to a buffer_t instance
 * @param   data    data which should be written
 * @return  a element of #bufferStatus_t
 * @retval  bufferOK    it worked as expected
 * @retval  bufferNotInitialized    the bufferInit() method hasn't been called or failed before
 * @retval  bufferFull  the buffer is full an no more data can be written
 */
bufferStatus_t bufferWrite(buffer_t* buffer, bufferDatatype data){
    if(!bufferIsFull(buffer)){
        bufferStatus_t temp = bufferWriteToIndex(buffer, buffer->writePointer, data);
        if(temp == bufferOK){
            buffer->datacount++;
            buffer->writePointer = (buffer->writePointer + 1) % buffer->length;
            return temp;
        }else{
            return temp;
        }
    }else{
        return bufferFull;
    }
}

/**
 * @brief Add data to the end of the ringbuffer. If the buffer is full, overwrite the first data
 * @param   buffer  pointer to a buffer_t instance
 * @param   data    data which should be written
 * @return  a element of #bufferStatus_t
 * @retval  bufferOK    it worked as expected
 * @retval  bufferNotInitialized    the bufferInit() method hasn't been called or failed before
 */
bufferStatus_t bufferWriteOverwrite(buffer_t* buffer, bufferDatatype data){
    //If the buffer is full, dummy read one byte
    if(bufferIsFull(buffer)){
        bufferDatatype temp;
        (void)bufferRead(buffer, &temp);
    }
    
    bufferStatus_t temp = bufferWriteToIndex(buffer, buffer->writePointer, data);
    if(temp == bufferOK){
        buffer->datacount++;
        buffer->writePointer = (buffer->writePointer + 1) % buffer->length;
        return temp;
    }else{
        return temp;
    }
}

/**
 * @brief read data from the beginning of the buffer
 * @param   buffer  pointer to a buffer_t instance
 * @param   data    pointer to a variable where data should be stored
 * @return  a element of #bufferStatus_t
 * @retval  bufferOK    it worked as expected
 * @retval  bufferNotInitialized    the bufferInit() method hasn't been called or failed before
 * @retval  bufferEmpty  the buffer is empty an no more data can be read
 */
bufferStatus_t bufferRead(buffer_t* buffer, bufferDatatype* data){
    if(!bufferIsEmpty(buffer)){
        bufferStatus_t temp = bufferReadFromIndex(buffer, buffer->readPointer, data);
        if(temp == bufferOK){
            buffer->datacount--;
            buffer->readPointer = (buffer->readPointer + 1) % buffer->length;
            return temp;
        }else{
            return temp;
        }
    }else{
        return bufferEmpty;
    }
}

/**
 * @brief fill the whole buffer with given dummy data. 
 * @param   buffer  pointer buffer_t instance
 * @param   data    data to fill the buffer with
 * @param   silent  if this parameter is 1, the buffer will be filled with data, but the write pointer stays at its current position (usefull, if you take the mean but the buffer is not full yet. You can just prefill it, the mean will be taken with the prefilled values)
 * @return  an element of #bufferStatus_t
 * @retval  bufferOK    it worked as expected
 * @retval  bufferNotInitialized    the buffer wasn't initialized
 */
bufferStatus_t bufferFill(buffer_t* buffer, bufferDatatype data, uint8_t silent){
    if(buffer->isInitialized){
        if(silent){
            uint16_t i;
            for(i = 0; i < buffer->length; i++){
                bufferWriteToIndex(buffer, i, data);
            }
        }else{
            bufferStatus_t status = bufferOK;
            while(status == bufferOK){
                status = bufferWrite(buffer, data);
            }
        }
        return bufferOK;
    }else{
        return bufferNotInitialized;
    }
}

/**
 * @brief return, how many elements are stored and available in the buffer
 * @param   buffer      pointer to a buffer_t instance
 * @param   available   pointer to a variable where the number of available elements should be stored
 * @return  an element of #bufferStatus_t
 * @retval  bufferOK                it worked as expected
 * @retval  bufferNotInitialized    the buffer wasn't initialized
 */
bufferStatus_t bufferAvailable(buffer_t* buffer, uint16_t* available){
    if(buffer->isInitialized){
        *available = buffer->datacount;
        return bufferOK;
    }else{
        return bufferNotInitialized;
    }
}

/**
 * @brief have a look at the next element in the buffer, but do not increase the read pointer
 * @param   buffer  pointer to a buffer_t instance
 * @param   data    pointer to a variable where data should be stored
 * @return  a element of #bufferStatus_t
 * @retval  bufferOK    it worked as expected
 * @retval  bufferNotInitialized    the bufferInit() method hasn't been called or failed before
 * @retval  bufferEmpty  the buffer is empty an no more data can be read
 */
bufferStatus_t bufferPeek(buffer_t* buffer, bufferDatatype* data){
    if(!bufferIsEmpty(buffer)){
        bufferStatus_t temp = bufferReadFromIndex(buffer, buffer->readPointer, data);
        return temp;
    }else{
        return bufferEmpty;
    }
}

#ifdef BUFFER_ENABLE_MEAN

/**
 * @brief take the average of the whole buffer
 * @param   buffer  pointer to a buffer_t instance
 * @param   meanOut pointer to a variable, where the mean will be stored
 * @return  a element of #bufferStatus_t
 * @retval  bufferOK    it worked as expected, the mean is stored at the given variable
 * @retval  bufferNotInitialized    the buffer is not initialized
 * @bug The sum of the buffer is taken. Take precautions, that this variable won't overflow
 */
bufferStatus_t bufferMean(buffer_t* buffer, bufferDatatype* meanOut){
    long sum = 0;
    bufferDatatype temp;
    uint16_t i;
    bufferStatus_t status;
    
    for(i = 0; i < buffer->length; i++){
        status = bufferReadFromIndex(buffer, i, &temp);
        if(status != bufferOK){
            return status;
        }
        sum += temp;
    }
    
    *meanOut = sum / buffer->length;
    
    return bufferOK;
}

/**
 * @brief take the root mean square of the whole buffer
 * @param   buffer  pointer to a buffer_t instance
 * @param   meanOut pointer to a variable, where the mean will be stored
 * @return  a element of #bufferStatus_t
 * @retval  bufferOK    it worked as expected, the mean is stored at the given variable
 * @retval  bufferNotInitialized    the buffer is not initialized
 * @bug The sum of squared buffer elements is taken. Take precautions, that this variable won't overflow
 */
bufferStatus_t bufferMeanRMS(buffer_t* buffer, bufferDatatype* meanOut){
    long sum = 0;
    bufferDatatype temp;
    uint16_t i;
    bufferStatus_t status;
    
    for(i = 0; i < buffer->length; i++){
        status = bufferReadFromIndex(buffer, i, &temp);
        if(status != bufferOK){
            return status;
        }
        sum += (temp * temp);
    }
    
    sum = sum / buffer->length;
    
    *meanOut = (bufferDatatype)sqrt(sum);
    
    return bufferOK;
}

#endif