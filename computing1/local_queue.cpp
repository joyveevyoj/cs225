// Data structure for local_queue

#include <iostream>
#include "local_queue.h"
#include <string>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <vector>
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
    return;
}

template<class T>void person<T>::random_generate(int seed)
{
    srand(clock() * seed);
    string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string integer_set = "0123456789";
    int i;//index for loop
    int index;//index for alphabet,interger_set and other vectors;
    //Create random name
    int namelength = rand() % 5 + 2;
    name.resize(namelength);
    for(i = 0; i < namelength;i++)
    {
        index = rand() % 26;
        name[i] = alphabet[index];
    }

    //Create id
    int idlength = 10;
    id.resize(idlength);
    for(i = 0; i < idlength; i++)
    {
        index = rand() % 10;
        id[i] = integer_set[index];
    }

    //Create address
    vector<string>address_set;
    address_set.push_back("Xiashi"); address_set.push_back("Haizhou"); address_set.push_back("Haichang"); address_set.push_back("Maqiao");
    int address_set_id = rand() % address_set.capacity();
    address = address_set[address_set_id];

    //Create phone
    int phonelength = 11;
    phone.resize(phonelength);
    for(i = 0; i < phonelength; i++)
    {
        index = rand() % 10;
        phone[i] = integer_set[index];
    }

    //Create Wechat
    int Wechatlength = 19;
    Wechat = "wxid_";
    Wechat.resize(Wechatlength);
    for(i = 0; i < Wechatlength - 5;i++)
    {
        index = rand() % 26;
        Wechat[i+5] = alphabet[index];
    }
    //Change three letters to digits
    for(i = 0;i < 3;i++)
    {
        index = rand() % 13 + 5;
        int int_index = rand() % 10;
        Wechat[index] = integer_set[int_index];
    }

    //Create email
    vector<string>email_suffix_set;
    email_suffix_set.push_back("@qq.com"); email_suffix_set.push_back("@gmail.com"); email_suffix_set.push_back("@163.com");  email_suffix_set.push_back("@sina.com");
    int emaillength = 10;
    string email_prefix;
    email_prefix.resize(emaillength);
    for(i = 0; i < emaillength; i++)
    {
        index = rand() % 10;
        email_prefix[i] = integer_set[index];
    }
    index = rand() % 4;
    email = email_prefix + email_suffix_set[index];
    
    //Create prof
    vector<string>prof_set;
    //Profession priority increase from this order
    prof_set.push_back("Teacher"); prof_set.push_back("Actor"); prof_set.push_back("Farmer"); prof_set.push_back("Businessman");
    prof_set.push_back("Journalist"); prof_set.push_back("Student"); prof_set.push_back("Engineer"); prof_set.push_back("Programmer");
    index = rand() % 8;
    prof = prof_set[index];
    prof_id = index;

    //Create age;
    vector<string>age_set;
    age_set.push_back("Children"); age_set.push_back("Adolescent"); age_set.push_back("Young Adult"); age_set.push_back("Adult");
    age_set.push_back("Senior"); age_set.push_back("Elderly Person"); age_set.push_back("Old Person");
    index = rand() % 7;
    age = age_set[index];
    age_id = index;

    //Create risk
    vector<string>risk_set;
    risk_set.push_back("No risk"); risk_set.push_back("Low risk"); risk_set.push_back("Medium risk"); risk_set.push_back("High risk");
    index = rand() % 4;
    risk = risk_set[index];
    risk_id = index;

    //Create Time
    Time = new double[3];
    Time[0] = rand() % 2015 + 0.1 * (rand() % 10);//2016时暂不考虑  
    Time[1] = -1;
    Time[2] = -1;   //-1表示不知道目前appointment time和treatment time为什么时候


    //Create Withdraw and priority letter;
    withdraw = false;
    letter = false;


}


template<class T> double* person<T>::show_hour()
{
    return Time;
}

template<class T> int* person<T>::show_day()
{
    int* Day = new int[3];
    for(int i = 0;i < 3; i++)
    {
        if(Time[i] == -1){
            Day[i] = -1;
            continue;
        }   
        Day[i] = (int)(Time[i] / 24);
    }
    return Day;
}

template<class T> int* person<T>::show_week()
{
    int* Week = new int[3];
    for(int i = 0; i < 3; i++)
    {
        if(Time[i] == -1){
            Week[i] = -1;
            continue;
        }
        Week[i] = (int)(Time[i] / (24 * 7));   
    }
    return Week;
}

template<class T> int* person<T>::show_month()
{
    int* Month = new int[3];
    for(int i = 0; i < 3;i++)
    {
        if(Time[i] == -1){
            Month[i] = -1;
            continue;;
        }
        Month[i] = (int)(Time[i] / (24*28));    
    }
    return Month;
}


template<class T>string* person<T>::show_format_time()
{
    int* Month = new int[3];
    int* Day = new int[3];
    double* Hour = new double[3];
    int* Minute = new int[3];
    int* intHour = new int[3];
    string* format_time = new string[3];

    //Set month,day,hour,minute
    Month = this->show_month();
    Day = this->show_day();
    Hour = this->show_hour();
    for(int i = 0; i < 3;i++)
    {
        if(Time[i] == -1)
        {
            Minute[i] = -1;
            intHour[i] = -1;
            continue;
        }
        intHour[i] = (int)Hour[i];
        Minute[i] = (int) ((Hour[i] - intHour[i]) * 60);
    }
    for(int i = 0; i < 3;i++)
    {
        if(Time[i] == -1)
        {
            format_time[i] = "N/A";
            continue;
        }
        format_time[i] = to_string(2022) + "-" + to_string(1+Month[i]) + "-" + to_string(Day[i] % 28 + 1) + " " + to_string(intHour[i] % 24 ) + ":" + to_string(Minute[i]);
        //Format: 2022-3-27 19:22
    }
    return format_time;



}

template<class T> void person<T>::set_key()
{
    //525699 is the largest time in minute within a year
    //register time priority
    key = 525699 - calculate_time;

    //Withdraw priority
    if ((withdraw == true) && ((risk_id == 0) || (risk_id == 1)))
    {
        key += 14 * 24 * 60;
    }

    //risk priority
    if( risk_id == 2)
    {
        key += 30 * 24 * 60;    //Medium risk extend one month
    }
    if( risk_id == 3)
    {
        //if( local_queue_id.is != true)
        {
            key = -1;
            return; //if there are others in queue, set key to minimum
        }
    }

    //age priority
    key += age_id * 1000000;
    //profession priority
    key += prof_id * 10000000;
    //letter priority
    if (letter == true)
    {
        key += 100000000;
    }
    return;
}

template<class T> T person<T>::return_key()
{   
    return key;
}

int main()
{
    person<int> p1[10];
    for(int i =0; i < 10; i++)
    {
        p1[i].random_generate(i);

    }
    for(int i = 0;i < 10; i++)
    {
        cout << p1[i].show_format_time()[0] << endl;
    }
}