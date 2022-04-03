// Data structures for local queue

#ifndef local_queue_h
#define local_queue_h
#include <string>
#include <cstring>

using namespace std;
template <class T> class fifo
{
public:
    fifo(int size = 10);
    int getlength(void);
    bool isempty(void);
    T back(void);
    T front(void);
    void pushback(T value);
    T popfront();


private:
    int maxsize, minsize;
    int first, last;
    int numitems;
    T* reprarray;
    void allocate(void);
    void deallocate(void);


};


template <class T>class person
{
public:
    person();
    void set_key();
    T return_key();
    int status;
    
private:
    bool initial_register;  //true if register for the first time
    bool letter;    //true if there is letter for ddl
    int risk;   //0,1,2,3
    int prof;   //0,1,2,3,4,5,6,7, the larger, the higher priority
    int age;    //0,1,2,3,4,5,6, the larger, the higher priority
    bool withdraw;  //true if withdraw
    //fifo<person> local_queue_id;    //Which Local queue this guy is in
    //appointment_list
    //hospital
    //appointment_time出堆时间
    //waiting_time
    //有没有来就诊 Bool 
    string id;
    string name;
    string address;
    string phone;
    string Wechat;
    string email;

    int calculate_time;  //Time after calculation, used for key value, unit in minutes; 
    T key;

};

























#endif