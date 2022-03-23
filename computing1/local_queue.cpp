// Data structure for local_queue
// Test for git:
#include <iostream>
#include "local_queue.h"
using namespace std;

template<class T> fifo<T>::fifo(int size)
{
    maxsize = size;
    if (size < 10)
        minsize = size;
    else
        minsize = 10;
    numitems = 0;
    first = 0;
    last = -1;
    reprarray = new T[maxsize];
}

template<class T>int fifo<T>::getlength()
{
    return numitems;
}

template<class T>bool fifo<T>::isempty()
{
    if (numitems == 0)
        return true;
    else
        return false;
}

template<class T>T fifo<T>::back()
{
    if(isempty())
    {
        cout << "The queue is empty" << endl;
        exit(EXIT_FAILURE);
    }
    int back_index = (first + numitems - 1) % maxsize;
    return reprarray[back_index];
}

template<class T>T fifo<T>::front()
{
    if (isempty())
    {
        cout << "The queue is empty" << endl;
        exit(EXIT_FAILURE);
    }
    return reprarray[first];
}

template<class T>void fifo<T>::pushback(T value)
{
    if (numitems == maxsize)
        allocate();
    last = ++last % maxsize;
    reprarray[last] = value;
    ++numitems;
    return;
}

template<class T>T fifo<T>::popfront()
{
    if ((numitems == maxsize / 4) && (maxsize > minsize))
        deallocate();
    if(isempty())
    {
        cout << "The queue is empty" << endl;
        exit(EXIT_FAILURE);
    }

    T front_element = reprarray[first];
    first = ++first % maxsize;
    --numitems;
    return front_element;
}

template<class T>void fifo<T>::allocate()
{
    int new_size = 2 * maxsize;
    T* new_array = new T[new_size];
    for(int i = 0; i < numitems; i++)
    {
        int pos = (i + first) % maxsize;
        new_array[i] = reprarray[pos];
    }
    delete[] reprarray;
    reprarray = new_array;
    maxsize = new_size;
    first=  0;
    last = numitems - 1;
    return;
}

template<class T>void fifo<T>::deallocate()
{
    int new_size = maxsize / 2;
    T *new_array = new T[new_size];
    for(int i = 0; i < numitems; i++)
    {
        int pos = (i + first) % maxsize;
        new_array[i] = reprarray[pos];
    }
    delete[] reprarray;
    reprarray = new_array;
    maxsize = new_size;
    first = 0;
    last = numitems - 1;
    return;
}

template<class T> person<T>::person()
{

}
template<class T> void person<T>::set_key()
{
    //525699 is the largest time in minute within a year
    //register time priority
    key = 525699 - calc_time;

    //Withdraw priority
    if ((withdraw == true) && ((risk == 0) || (risk == 1)))
    {
        key += 14 * 24 * 60;
    }

    //risk priority
    if( risk == 2)
    {
        key += 30 * 24 * 60;    //Medium risk extend one month
    }
    if( risk == 3)
    {
        if( isempty() != true)
        {
            key = -1;
            return; //if there are others in queue, set key to minimum
        }
    }

    //age priority
    key += age * 1000000;
    //profession priority
    key += prof * 10000000;
    //letter priority
    if (letter == true)
    {
        key += 100000000;
    }
    
    return;
}

template<class T> T person<T>::rt_key()
{   
    return key;
}
