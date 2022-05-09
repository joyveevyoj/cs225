#ifndef head_h
#define head_h

#include <stdio.h>
#include <iostream>
#include<vector>
#include <fstream>
#include "cassert"
#include <string>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iomanip>
#include <cstdlib>
using namespace std;


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
    person();       //If no argument, create an empty person
    person(string tableline);   //Create a person from local registry
    void random_generate(int seed); //Used to put random attributes into a person
    void set_key(); 
    T return_key();
    string show_id();
    double* show_hour();
    int* show_intHour();
    int* show_day();
    int* show_half_day();
    int* show_week();
    int* show_month();  
    string* show_format_time(); //All Time starts at zero
    bool is_update(person<int>* oldper); //check whether this is an update
    bool is_newwithdraw(person<int>* oldper);//check whether this is a new withdraw
    bool is_withdraw(); //return withdraw
    void set_appointment(appointment* r_appoint);
    void set_assign_appointment_time(double hour);
    void punish();   
    void display_all(); //Display all information of this person
    string show_risk();
    void withdraw_time();

    int local_id;   
    int* hospital_id;    //Array with index for the hospital，即hopsital's ranking list 
    int hosp_num;   //Initailized when the person is created
    int status;
    appointment* p_appoint;
    void withdraw_time(double hour);
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
    bool wont_withdraw;  //Used only for creating peson who withdraw. Don't get confused
    double* Time;    //Array representing time. 0 is registeration time，1 is appointment time,2 is the time when appointment is assigned
                     //If not initialized, set as -1;
    int updated_half_day; //更新的某一半天  -1表示没有更新
    T key;

};

class TableWrite
{
public:
    TableWrite(string filename_,int num_,int updated_num_,int withdraw_num_);
    //num:创建的人的个数, updated_num: 更新的人的个数,withdraw_num: 表格中withdraw值为1的个数,letter_num:有letter的个数
    void table_create();
    string table_line(person<int>& p); //给每一个人创造一行
private:
    ofstream outfile;
    string filename;
    int num;
    int updated_num;
    int withdraw_num;
    void table_open(string filename);
    void table_close();
    person<int>* SelectionSort(person<int>* p,int num_of_person,int way);   //way=0，根据注册时间排序，way=1，根据更新的半天排序
};



template <class T> class Fb_node 
{
public:
    int key;                // use calculated key to have its priority ;
    person<int>* patient ;     
    Fb_node<T> *left;       // point to left node 
    Fb_node<T> *right;      //  point to right node
    Fb_node<T> *child;      //  point to its child
    Fb_node<T> *parent;     // point to its parent
    bool       mark;        // mark whether it has lost child
    int        degree;      //indicate the number of children
    Fb_node(person<int>* tpatient):key(tpatient->return_key()), patient(tpatient),mark(false),left(NULL),right(NULL),child(NULL),parent(NULL),degree(0) {
        key    = tpatient->return_key();  // use constructor to initialize, key is the person's calculate key
        patient = tpatient ;  // person is this patient
        left   = this; 
        right  = this;       // left and right both point to itself
        parent = NULL;       //parent and child are none
        child  = NULL;
        degree = 0;         
        mark = false;
    }
};

template <class T> class Fb_heap {
private:
    int numnodes;      //number of nodes in the heap
    Fb_node<T> *min;    // points to the node with minimum value
    int maxdrg;      // the maxdegree in the heap , in order to compute list space
    Fb_node<T> **drglist;    // use to store the list of degree 

private:
    void addlistNode(Fb_node<T> *ptr, Fb_node<T> *root); // add node into the left of root in the list
    void rmlistNode(Fb_node<T> *ptr); // remove this nodes' pointer from the list of nodes
    void insert(Fb_node<T> *ptr); // insert new node ptr into the heap
    Fb_node<T>* getminnode(); // get the tree of the minnode
    void nodelink(Fb_node<T>* ptr, Fb_node<T>* r); //link ptr to root r and becomes r's child
    void renewdrg(Fb_node<T> *tparent, int degree); // renew the degree
    void cut(Fb_node<T> *node, Fb_node<T> *parent); // cut the node into the root list
    void markcut(Fb_node<T> *ptr) ; // check the mark of the node who lost his child
    void consolidate(); // let the degree of the nodes in root list are different
    Fb_node<T>* search(Fb_node<T> *root, person<int>* patient); // from root to search patient
    Fb_node<T>* search(person<int>* patient); // search the patient
    void decrease(Fb_node<T> *ptr, T newkey); // decrease key into newkey
    void increase(Fb_node<T> *ptr, T newkey); // increase key into newkey
    void update(Fb_node<T> *ptr, T newkey); // update the key in ptr into new key
    void remove(Fb_node<T> *ptr); // remove ptr from the heap

    void buildlsarea(){ //allocate area for the degree list
    int olddrg = maxdrg; //store old degree
    maxdrg = (log(numnodes)/log(2.0)) + 1;
    if (olddrg >= maxdrg){return;}  // if the old degree is bigger than new maxdgree, return
    drglist = (Fb_node<T> **)realloc(drglist, sizeof(Fb_heap<T> *) * (maxdrg + 1)); // allocate area
    }; 

public:
    Fb_heap(){
    numnodes = 0;
    maxdrg = 0;  // initialize numnodes and max of degree to 0
    min = NULL;
    drglist = NULL; //constructor
    };
    /*~Fb_heap();  // deconstructor*/
    void insert(person<int>* patient); //insert a new patient into the fibonacci heap
    person<int>* delete_min(); // delete the minnode and return the pointer to this deleted person
    void update(person<int>*newpatient, person<int>* oldpatient); // update the old patient's key value into new value and new pointer
    void remove(person<int>* oldpatient);//delete this old patient
    int getnum(){return numnodes;}; // return number of nodes in the heap
    person<int>* rtminperson(void){return min->patient ;};
    void showpriority(void);
};


class appointment
{
public:
    appointment(person<int>* a_patient, int date_out_of_queue, vector<hospital*> a_hospital_list);
    //virtual ~appointment();
    vector<hospital*> hospitals;
    void make_appointment();
    void appointment_withdraw();
    void pretty_print();
    bool is_appointment_passed(double current_hour);
    int hospital_id;
    person<int>* patient;
    double the_time;
    int date;
};

class hospital
{
public:
    hospital(int idnum, int capacity, double o_time, double c_time);
    bool is_hospital_available(int h_date);
    double get_time_appointed(int h_date);
    int id;//the hospital's unique id number, which marks its position in the hospital list.
    vector<appointment*> daily_appointment_list;
    void add_to_applist(appointment* new_app);
    void printapp();
    void withdraw_app(appointment* w_app);//新改的，头文件里忘记加了
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

    double waitingtime(double a, double b);

    void Save(int week,string file_head);

    void Sort(int week);

private:

    vector<person<int>*> rl;   
    
};



class Monthlyreport
{
public:
    double current_time;
   
    Monthlyreport(vector<person<int>*> report_list, double Time){
        rl = report_list;
        current_time = Time;
    }

    int getlength(vector<person<int>*> report_list);

    int getlength_m(vector<person<int>*> report_list, int month);

    int appointment_num(vector<person<int>*> report_list, int month);

    int treatment_num(vector<person<int>*> report_list, int month);

    int withdraw_num(vector<person<int>*> report_list, int month);

    double average_time(vector<person<int>*> report_list, int month);

    int register_waiting_num(int id, int month);

    int intotal_waiting_num(int a, int b, int c);

    void Save(int month);

private:

    vector<person<int>*> rl;
};


#endif