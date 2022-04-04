#include <iostream>  //包含输入输出流头文件
using namespace std; //使用标准命名空间
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include "local_queue.h"

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

    double average_time(vector<person<int>*> report_list);

    int register_waiting_num(int a, int b);

    int intotal_waiting_num(int a, int b);

    void Save();

private:

    vector<person<int>*> rl;
};



