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


template <class T>
class Fb_node {
public:
    int key;                // 关键字(键值)
    int degree;            // 度数
    Fb_node<T> *left;    // 左兄弟
    Fb_node<T> *right;    // 右兄弟
    Fb_node<T> *child;    // 第一个孩子节点
    Fb_node<T> *parent;    // 父节点
    bool marked;        // 是否被删除第一个孩子
    person<int>* patient ;
    

    Fb_node(person<int>* tpatient):key(tpatient->return_key()), degree(0), marked(false),
                     left(NULL),right(NULL),child(NULL),parent(NULL), patient(tpatient) {
        key    = tpatient->return_key();
        degree = 0;
        marked = false;
        left   = this;
        right  = this;
        parent = NULL;
        child  = NULL;
        patient = tpatient ;
    }
};

template <class T>
class Fb_heap {
private:
    int keyNum;         // 堆中节点的总数
    int maxDegree;      // 最大度
    Fb_node<T> *min;    // 最小节点(某个最小堆的根节点)
    Fb_node<T> **cons;    // 最大度的内存区域

public:
    Fb_heap();
    ~Fb_heap();

    // 新建key对应的节点，并将其插入到斐波那契堆中
    void insert(person<int>* patient);
    // 移除斐波那契堆中的最小节点
    person<int>* delete_min();
    // 将斐波那契堆中键值oldkey更新为newkey
    void update(person<int>*newpatient, person<int>* oldpatient);
    // 删除键值为key的节点
    void remove(person<int>* oldpatient);
    // 斐波那契堆中是否包含键值key
    void print();
    // 销毁
    void destroy();
    int getnum(){return keyNum;};

private:
    // 将node从双链表移除
    void removeNode(Fb_node<T> *node);
    // 将node堆结点加入root结点之前(循环链表中)
    void addNode(Fb_node<T> *node, Fb_node<T> *root);
    // 将双向链表b链接到双向链表a的后面
    void catList(Fb_node<T> *a, Fb_node<T> *b);
    // 将节点node插入到斐波那契堆中
    void insert(Fb_node<T> *node);
    // 将"堆的最小结点"从根链表中移除，
    Fb_node<T>* extractMin();
    // 将node链接到root根结点
    void link(Fb_node<T>* node, Fb_node<T>* root);
    // 创建consolidate所需空间
    void makeCons();
    // 合并斐波那契堆的根链表中左右相同度数的树
    void consolidate();
    // 修改度数
    void renewDegree(Fb_node<T> *parent, int degree);
    // 将node从父节点parent的子链接中剥离出来，并使node成为"堆的根链表"中的一员。
    void cut(Fb_node<T> *node, Fb_node<T> *parent);
    // 对节点node进行"级联剪切"
    void cascadingCut(Fb_node<T> *node) ;
    // 将斐波那契堆中节点node的值减少为key
    void decrease(Fb_node<T> *node, T key);
    // 将斐波那契堆中节点node的值增加为key
    void increase(Fb_node<T> *node, T key);
    // 更新斐波那契堆的节点node的键值为key
    void update(Fb_node<T> *node, T key);
    // 在最小堆root中查找键值为key的节点
    Fb_node<T>* search(Fb_node<T> *root, person<int>* patient);
    // 在斐波那契堆中查找键值为key的节点
    Fb_node<T>* search(person<int>* patient);
    // 删除结点node
    void remove(Fb_node<T> *node);
    // 销毁斐波那契堆
    void destroyNode(Fb_node<T> *node);
    // 打印"斐波那契堆"
    void print(Fb_node<T> *node, Fb_node<T> *prev, int direction);
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
  bool is_appointment_passed();
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

    void Save();

private:

    vector<person<int>*> rl;
};


#endif