#include <iostream>  //包含输入输出流头文件
using namespace std; //使用标准命名空间
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
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
    person();   //If no argument, create a random one
    person(string tableline);   //Create a person from local registry
    void set_key();
    T return_key();
    void update_status(int status_number);
    string show_name();
    int show_prof();
    int show_age();
    int show_risk();

    double show_register_time();
    double show_appointment_time();
    double show_treatment_time();
    double* show_hour();
    int* show_day();
    int* show_week();
    int* show_month();  //从零开始的统一时间
    string* show_format_time();

    bool is_withdraw();
    bool is_appointed();
    bool is_treatment();
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

class Weeklyreport
{
public:
    double current_time;
    Weeklyreport(vector<person<int>*> report_list, double Time){
        rl = report_list;
        current_time = Time;
    }

    
    int getlength(vector<person<int>*> report_list);

    double waitingtime1(double a, double b);

    double waitingtime2(double a, double b);

    double waitingtime3(double a, double b);

    void Save();

    void Sort();

private:

    vector<person<int>*> rl;   
};



class Monthlyreport
{
public:
    double current_time;
    Monthlyreport(){ cout << "constructor" << endl;} ;

    Monthlyreport(vector<person<int>*> report_list, double Time){
        rl = report_list;
        current_time = Time;
    }

    int getlength(vector<person<int>*> report_list);

    int appointment_num(vector<person<int>*> report_list);

    int treatment_num(vector<person<int>*> report_list);

    int withdraw_num(vector<person<int>*> report_list);

    double average_time(double, double, int);

    int register_waiting_num(int a, int b);

    int intotal_waiting_num(int a, int b);

private:

    vector<person<int>*> rl;
};



