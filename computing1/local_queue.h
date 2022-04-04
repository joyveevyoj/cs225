// Data structures for local queue

#ifndef local_queue_h
#define local_queue_h
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

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
    friend class TableWrite;
    friend class Weeklyreport;
    friend class Monthlyreport;

    person();       //If no argument, create one
    person(string tableline);   //Create a person from local registry
    void random_generate(int seed,int week); //Used to put random attributes into a person. Seed表示这个人的种子，用于随机生成,week表示生成在第几周的人
    void set_key();
    T return_key();
    void update_status(int status_number);
    // string show_id();
    // string show_Wechat();
    // string show_phone());
    // string show_address();
    // string show_email();
    // string show_name();
    // string show_prof();
    // int show_prof_id();
    // string show_age();
    // int show_age_id();
    // string show_risk();
    // int show_risk_id();  //对重要的类来说这些都可以直接友元访问，若其他情况下需要再添加所需函数即可

    double* show_hour();
    int* show_day();
    int* show_half_day();
    int* show_week();
    int* show_month();  //从零开始的统一时间
    string* show_format_time();

    bool is_withdraw();
    bool is_appointed();
    void update_appointed();
    int status;
private:
    bool initial_register;  //true if register for the first time
    bool letter;    //true if there is letter for ddl
    int risk_id;
    string risk;   //0,1,2,3
    int prof_id;   //0,1,2,3,4,5,6,7, the larger, the higher priority
    string prof;    //String name for profession
    int age_id;
    string age;    //0,1,2,3,4,5,6, the larger, the higher priority
    bool withdraw;  //true if withdraw
    bool appointed; //True if apppointment is made
    
    fifo<person<T>*> local_registry_id;    //Which Local registry this guy is in
    double* Time; //表示时间的数组，0为registeration，1为appointment， 2为treatment
    //appointment_list
    //hospital
    //appointment_time出堆时间
    //waiting_time;
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


class people
{

};

class TableWrite
{
public:
    void table_create(string filename,int num,int week); //num:创建的人的个数, 这个文件对应第几周
    void table_line(person<int> p); //给每一个人创造一行


private:
    string tableline;
    ofstream outfile;
    void table_open(string filename);
    void table_close();
};


#endif