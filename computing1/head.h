#ifndef head_h
#define head_h

#include<vector>
#include <iostream>  //包含输入输出流头文件
#include <fstream>
#include <string>
#include <cstring>
using namespace std;
// a change

class appointment; 
class hospital;

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
    person();       //If no argument, create one,key is set as well
    person(string tableline);   //Create a person from local registry
    void random_generate(int seed); //Used to put random attributes into a person
    void set_key(); 
    T return_key();
    void update_status(int status_number);
    string show_id();
    double* show_hour();
    int* show_intHour();
    int* show_day();
    int* show_half_day();
    int* show_week();
    int* show_month();  //从零开始的统一时间
    string* show_format_time();
    bool is_update(person<int>* oldper); //check whether this is an update
    bool is_newwithdraw(person<int>* oldper);//check whether this is a new withdraw
    bool is_withdraw(); //return withdraw
    void set_appointment(appointment* r_appoint);    //Not written yet
    void punish();   //Not written yet
    void display_all(); //Display all information of this person

    int local_id;   
    int* hospital_id;    //Array with index for the hospital，即hopsital's ranking list 
    int hosp_num;   //Initailized when the person is created
    int status;
    appointment* p_appoint;
private:
    bool letter;    //true if there is letter for ddl
    int risk_id;
    string risk;   //0,1,2,3
    int prof_id;   //0,1,2,3,4,5,6,7, the larger, the higher priority
    string prof;    //String name for profession
    int age_id;
    string age;    //0,1,2,3,4,5,6, the larger, the higher priority
    string id;
    string name;
    string address;
    string phone;
    string Wechat;
    string email;
    bool withdraw;  //true if withdraw
    double* Time; //表示时间的数组，0为registeration，1为appointment,
                //0在初始化设置，1在创建时初始化为-1,具体的1时间应该由set_appoitment写入
    T key;

};

class TableWrite
{
public:
    void table_create(string filename,int num); //num:创建的人的个数,
    string table_line(person<int>& p); //给每一个人创造一行

private:
    ofstream outfile;
    void table_open(string filename);
    void table_close();
};


template<class T> class Fb_heap
{
private :
    struct Fb_node
    {
        T dataitem ;
        struct Fb_node *parent;
        struct Fb_node *child;
        struct Fb_node *left;
        struct Fb_node *right;
        bool mark;
        int degree;
        Fb_node() : dataitem(0), parent(nullptr), child(nullptr), left(this), right(this), mark(false), degree(0){} ;
        Fb_node(T _data,Fb_node* _parent,Fb_node* _child,Fb_node* _left, Fb_node* _right,bool _mark,int _degree) : dataitem(_data), parent(_parent), child(_child), left(_left), right(_right), mark(_mark), degree(_degree){} ;
    };
private :
    void listadd(Fb_node * &r, Fb_node * ptr);// add node ptr to the right of node r
    //int Dn() {return (log2(numnodes) + 1);} 
    void deleteln(Fb_node *p) ; //delete the node in list
    void linkheap(Fb_node * p1, Fb_node * p2); //link p1 to the right of p2
    void cutheap(Fb_node * p1, Fb_node * p2) ;
    void markcut(Fb_node* p);
    Fb_node* minnode;
    int numnodes; 
    //Fb_node* search(Fb_node* nn,T k) ;
public:
    Fb_heap() : minnode(nullptr), numnodes(0) {} ;
    void insert(person<int>* newpatient);
    person<int>* delete_min(void);
    void consolidate(void);
    void decrease(Fb_node *dnode,T k);
    void deletenode(person<int>* oldpatient) ;
    void update(person<int>*newpatient, person<int>* oldpatient) ; //  update dataitem k into nk

};


class appointment
{
public:
  appointment(person<int>* a_patient, int date_out_of_queue);
  //virtual ~appointment();
  void make_appointment();
  void appointment_withdraw();
  void pretty_print();
  bool is_appointment_passed();
  int hospital_id;
  person<int>* patient;
  double the_time;
  int date;
    
private:
};

class hospital
{
public:
  hospital(int idnum, int capacity, double o_time, double c_time);
  bool is_hospital_available(int h_date);
  double get_time_appointed(int h_date);
  int daily_appointment_num;
  int id;//the hospital's unique id number, which marks its position in the hospital list.
  vector<appointment*> daily_appointment_list;
  void add_to_applist(appointment* new_app);
  void printapp();
  int daily_remaining_capacity[100];
private:
  int daily_capacity;
  double open_time;
  double close_time;
  double treatment_time;

};



class reportlist
{
  public:
  void push_new(int r_status, person<int>* patient);//push a new person into the report list, and set his status value
  void push_old(int r_status, person<int>* patient);//use a temperary person to update the person already in the report list and set his status value
  void push_appoi(int r_status, appointment* r_appointment);//put the appointment information into the person stored in the report list and set his status value
  person<int>* exist(person<int>* ); //this function checks the temperary person's id and see whether it is already in the report list, if it is                                 
                                    //return the person's addr, else return null
  vector<person<int>*> rl;// a vector list that stores all the information about that person
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


#endif