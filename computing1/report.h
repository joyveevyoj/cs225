#include <iostream>  //包含输入输出流头文件
using namespace std; //使用标准命名空间
#include <vector>
#include "local_queue.h"
#include <fstream>


template <class T>class person
{
public:
    person();
    person(string tableline);
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

class reportlist{
    public:
    void push_newp(person* patient);
    void push_oldp(person* temp_person);
    void push_a(appointment* appoint);
    bool exist(person* patient);
    private:
    vector<person<int>*> replist;


};

class Weeklyreport
{
public:

    Weeklyreport(vector<person<int>*> report_list){
        rl = report_list;
    }

    int num;

    void save();

    void Sort();

    void Show();
private:

    vector<person<int>*> rl;

    
    
};

