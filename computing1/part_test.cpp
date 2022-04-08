
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
#include <iomanip>//fibo堆加的
#include <cstdlib>//fibo堆加的
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
    double* Time;    //Array representing time. 0 is registeration time，1 is appointment time,2 is the time when appointment is assigned
                     //If not initialized, set as -1;
    int updated_half_day; //更新的某一半天  -1表示没有更新
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

    int getlength(vector<person<int>*> report_list, int month);

    int appointment_num(vector<person<int>*> report_list, int month);

    int treatment_num(vector<person<int>*> report_list, int month);

    int withdraw_num(vector<person<int>*> report_list, int month);

    double average_time(vector<person<int>*> report_list, int month);

    int register_waiting_num(int id, int month);

    int intotal_waiting_num(int a, int b);

    void Save(int month);

private:

    vector<person<int>*> rl;
};

double Weeklyreport::waitingtime(double a, double b){
    return b - a;
}

int Weeklyreport::getlength(vector<person<int>*> report_list){
    int length=report_list.size();
    return length;
}

template<class T>void person<T>::set_assign_appointment_time(double hour)
{
    Time[2] = hour;
}

void Weeklyreport::Save(int week,string file_head)
{
    vector<string*> file_matrix;
    for(int i = 0; i < 3;i++)
    {
        string* file_vector = new string[12];
        for(int j = 0 ; j < 12; j++)
        {
            file_vector[j] = file_head + to_string(j+1) + "File" + to_string(i+1) + ".txt";
        }
        file_matrix.push_back(file_vector);
    }

    string* s1 = file_matrix[0];
    string* s2 = file_matrix[1];
    string* s3 = file_matrix[2];
	//string s1[] = {"Weekly1File1.txt","Weekly2File1.txt","Weekly3File1.txt","Weekly4File1.txt","Weekly5File1.txt","Weekly6File1.txt","Weekly7File1.txt","Weekly8File1.txt","Weekly9File1.txt","Weekly10File1.txt","Weekly11File1.txt","Weekly12File1.txt"};
    //string s2[] = {"Weekly1File2.txt","Weekly2File2.txt","Weekly3File2.txt","Weekly4File2.txt","Weekly5File2.txt","Weekly6File2.txt","Weekly7File2.txt","Weekly8File2.txt","Weekly9File2.txt","Weekly10File2.txt","Weekly11File2.txt","Weekly12File2.txt"};
    //string s3[] = {"Weekly1File3.txt","Weekly2File3.txt","Weekly3File3.txt","Weekly4File3.txt","Weekly5File3.txt","Weekly6File3.txt","Weekly7File3.txt","Weekly8File3.txt","Weekly9File3.txt","Weekly10File3.txt","Weekly11File3.txt","Weekly12File3.txt"};
	//将每个人数据写入到文件中
	for (int i = 0; i < getlength(rl); i++)
	{   
        if(rl[i]->status==1 ){
            ofstream ofs;
	        ofs.open(s1[week-1], ios::app); 
            // 用输出的方式打开文件  -- 写文件
		    ofs << rl[i]->name << " "
			<< rl[i]->prof << " "
            << rl[i]->age << " "
            << rl[i]->risk<< " "
			<< waitingtime(rl[i]->Time[0], current_time) << " aaa"<<endl;
            ofs.close();
            }
    
        else if(rl[i]->status==2 )
            {
            ofstream ofs;
	        ofs.open(s2[week-1], ios::app); // 用输出的方式打开文件  -- 写文件
		    ofs << rl[i]->name << " "
			<< rl[i]->prof << " "
            << rl[i]->age << " "
            << rl[i]->risk << " "
			<< waitingtime(rl[i]->Time[0], current_time) << " bbb"<<endl;
            ofs.close();
            }
    
        else if(rl[i]->status==3 && rl[i]->Time[2]>= (week-1)*7*24){
            ofstream ofs;
	        ofs.open(s3[week-1], ios::app); // 用输出的方式打开文件  -- 写文件
		    ofs << rl[i]->name << " "
			<< rl[i]->prof << " "
            << rl[i]->age << " "
            << rl[i]->risk << " "
			<< waitingtime(rl[i]->Time[1], rl[i]->Time[2]) << " ccc"<< endl;
            ofs.close();
            }
    }        
	

	//关闭文件
	
    
}

void Weeklyreport::Sort(int week)
{
    
    cout << "please select a sort method" << endl;
    cout << "put in '1' if sort by name" << endl;
	cout << "put in '2' if sort by profession" << endl;
    cout << "put in '3' if sort by risk" << endl;

	int select = 0;
	cin >> select;
		for (int i = 0; i < getlength(rl); i++)
		{
			int Min = i; //声明最小值下标n
			for (int j = i + 1; j < getlength(rl); j++)
			{
				if (select == 1) //升序
				{
					if (rl[Min]->name > rl[j]->name)
					{
						Min = j;
					}
            	}
                
				else if (select == 2) //升序
				{
					if (rl[Min]->prof > rl[j]->prof)
					{
						Min = j;
					}
            	} 
            
				else if (select == 3) //升序
				{
					if (rl[Min]->risk > rl[j]->risk)
					{
						Min = j;
					}
            	}
                
            }
            if (i != Min)
		    {
			    swap(rl[i], rl[Min]);
			}

			//判断一开始认定 最小值或最大值 是不是 计算的最小值或最大值，如果不是 交换数据
			
			
		}

		cout << "Successful!" << endl;
		this->Save(week,"SortedWeekly"); //排序后结果保存到文件中
			
}

int Monthlyreport::getlength(vector<person<int>*> report_list, int month){
    int length=0;
    for (int i = 0; i<report_list.size();i++){
        if (rl[i]->Time[0]>= (month-1)*24*28){
        length++;
        }
    }
    return length;
}

double Monthlyreport::average_time(vector<person<int>*> report_list,int month){
    int treatment_number = 0;
    int sum = 0;
    for (int i = 0; i < getlength(rl,month); i++){
        
        if (rl[i]->status==3){
            double waiting_time = rl[i]->Time[2] - rl[i]->Time[0];
            sum += waiting_time;  
            treatment_number++;   
        }
    }
    double mean = sum / treatment_number;
    return mean;
}
    
int Monthlyreport::appointment_num(vector<person<int>*> report_list,int month){    
    int appointment_number=0;
    for (int i = 0; i < getlength(rl, month); i++){
        
        if (rl[i]->status==2 || rl[i]->status==3){
            appointment_number++;
        }
    }
    return appointment_number;
}

int Monthlyreport::withdraw_num(vector<person<int>*> report_list,int month){    
    int withdraw_number = 0;
    for (int i = 0; i < getlength(rl, month); i++){
        
        if (rl[i]->status==4 || rl[i]->status==5){
            withdraw_number++;
        }
    }
    return withdraw_number;
}

int Monthlyreport::treatment_num(vector<person<int>*> report_list,int month){    
    int treatment_number = 0;
    for (int i = 0; i < getlength(rl,month); i++){
        
        if (rl[i]->status==3){
            treatment_number++;
        }
    }
    return treatment_number;
}

int Monthlyreport::register_waiting_num(int id,int month){//register_number-appoint_number
    int register_waiting_number = 0;
    for (int i = 0; i < getlength(rl,month); i++){
        
        if (rl[i]->local_id==id && rl[i]->status!=3){
            register_waiting_number++;
        }
    }
    return register_waiting_number;
}

int Monthlyreport::intotal_waiting_num(int a, int b){//register_number-treatment_number
    int num = a - b;
    return num;
}

void Monthlyreport::Save(int month)
{
    string monthly[]={"monthly1_report.txt","monthly2_report.txt","monthly3_report.txt"};
	//将每个人数据写入到文件中 
            ofstream ofs;
	        ofs.open(monthly[month-1], ios::app); // 用输出的方式打开文件  -- 写文件
			ofs << "rigister_number: " << getlength(rl,month) << "\n"
            << "Local_register_office0_waiting_number: " << register_waiting_num(0, month) << "\n"
            << "Local_register_office1_waiting_number: " << register_waiting_num(1, month) << "\n"
            << "Local_register_office2_waiting_number: " << register_waiting_num(2, month) << "\n"
            << "intotal_waiting_number: " << intotal_waiting_num(getlength(rl,month), treatment_num(rl,month)) << "\n"
            << "appointment_number: " << appointment_num(rl,month) << "\n"
            << "average_waiting_time: " << average_time(rl,month) << "\n"
			<< "withdraw_number: " << withdraw_num(rl,month) << endl;
            ofs.close();
     
}

	//关闭文件
    


string prof_set[8] = {"Teacher","Actor","Farmer","Businessman","Journalist","Student","Engineer","Programmer"};
string age_set[7] = {"Children","Adolescent","Young Adult","Adult","Senior","Elderly Person","Old Person"};
string risk_set[4] ={"No risk","Low risk","Medium risk","High risk"};
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
template<class T>person<T>::person(string tableline)
{
    //Separate the string
    int index = 0;  //逗号后的第一个位置
    int next_index; //逗号的2位置
    string separate_str[14];
    for(int i = 0; i < 14;i++)
    {
        next_index = tableline.find(",",index);
        separate_str[i] = tableline.substr(index,next_index-index);
        index = next_index + 1;
    }
    //Assign separate string to parameter of the person
    id = separate_str[0];
    name = separate_str[1];
    address = separate_str[2];
    phone = separate_str[3];
    Wechat = separate_str[4];
    email = separate_str[5];
    prof = separate_str[6];
    for(int i = 0; i < 8;i++)
    {
        if(prof == prof_set[i])
        {
            prof_id = i;
            break;
        }
    }
    age = separate_str[7];
    for(int i = 0; i < 7;i++)
    {
        if(age == age_set[i])
        {
            age_id = i;
            break;
        }
    }
    risk = separate_str[8];
    for(int i = 0; i < 4;i++)
    {
        if(risk == risk_set[i])
        {
            risk_id = i;
            break;
        }
    }
    string format_time = separate_str[9];
    int month = atoi(format_time.substr(5,1).c_str()) - 1;
    int day = atoi(format_time.substr(7,1).c_str()) - 1;
    int intHour = atoi(format_time.substr(9,2).c_str());

    double Minute = atof(format_time.substr(12,2).c_str());
    double Hour = Minute / 60 + intHour + day * 24 + month * 28 * 24;
    Time = new double[3];
    Time[0] = Hour;
    Time[1] = -1;
    Time[2] = -1;

    if(separate_str[10] == "0")
        withdraw = false;
    else
        withdraw = true;
    if(separate_str[11] == "0")
        letter = false;
    else
        letter = true;
    local_id = atoi(separate_str[12].c_str());
    hosp_num = separate_str[13].length();
    hospital_id = new int[hosp_num];
    for(int i = 0; i < hosp_num;i++)
    {
        hospital_id[i] = atoi(separate_str[13].substr(i,1).c_str());
    }
    //Set other necessary information
    status = 0; //Status initialized to 0

    //Set key
    set_key();
}
template<class T>void person<T>::random_generate(int seed)
{
    srand((unsigned)((time(NULL)) * (seed * seed)));
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    string Upper_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string integer_set = "0123456789";
    int i;//index for loop
    int index;//index for alphabet,interger_set and other vectors;
    //Create random name
    int namelength = rand() % 5 + 3;
    name.resize(namelength);
    for(i = 0; i < namelength;i++)
    {
        index = rand() % 26;
        name[i] = alphabet[index];
    }
    index = rand() % 26;
    name = Upper_alphabet[index] + name;


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
    // vector<string>prof_set;
    // //Profession priority increase from this order
    // prof_set.push_back("Teacher"); prof_set.push_back("Actor"); prof_set.push_back("Farmer"); prof_set.push_back("Businessman");
    // prof_set.push_back("Journalist"); prof_set.push_back("Student"); prof_set.push_back("Engineer"); prof_set.push_back("Programmer");
    index = rand() % 8;
    prof = prof_set[index];
    prof_id = index;

    //Create age;
    // vector<string>age_set;
    // age_set.push_back("Children"); age_set.push_back("Adolescent"); age_set.push_back("Young Adult"); age_set.push_back("Adult");
    // age_set.push_back("Senior"); age_set.push_back("Elderly Person"); age_set.push_back("Old Person");
    index = rand() % 7;
    age = age_set[index];
    age_id = index;

    //Create risk
    // vector<string>risk_set;
    // risk_set.push_back("No risk"); risk_set.push_back("Low risk"); risk_set.push_back("Medium risk"); risk_set.push_back("High risk");
    index = rand() % 4;
    risk = risk_set[index];
    risk_id = index;

    //Create Time
    Time = new double[2];
    Time[0] = rand() % 2016 + 0.1 * (rand() % 10);//2016时暂不考虑  
    Time[1] = -1;//-1表示不知道目前appointment time为什么时候

    //Create Withdraw and priority letter;
    withdraw = false;
    letter = false;

    //Create local registry id and hopsital id
    local_id = rand() % 3;

    hosp_num = rand() % 3 + 1;
    hospital_id = new int[hosp_num];
    if(hosp_num == 1)
    {
        hospital_id[0] = rand() % 3;
    }
    if(hosp_num == 2)
    {
        //Six cases
        int random_id_set[6] ={1,2,10,12,20,21};
        int chosen_id = random_id_set[rand() % 6];
        hospital_id[0] = chosen_id / 10;
        hospital_id[1] = chosen_id % 10;
    }
    if(hosp_num == 3)
    {
        for(i = 0; i < hosp_num; i++)
        {
            hospital_id[i] = i;
        }
        //Random swap
        for(i = 0; i < hosp_num; i++)
        {
            int rand_i = rand() % 3;
            int temp = hospital_id[i];
            hospital_id[i] = hospital_id[rand_i];
            hospital_id[rand_i] = temp;
        }
    }

}
template<class T> void person<T>::set_key()
{
    //register time priority
    T max_key = 117620161;
    int max_time = 20160;
    key = (int)(max_time - Time[0] * 10);
    

    //Withdraw priority
    if ((withdraw == true) && ((risk_id == 0) || (risk_id == 1)))
    {
        key += 14 * 24 * 10;
    }

    //age priority
    key += age_id * 100000;
    //profession priority
    key += prof_id * 1000000;
    //risk priority
    if( risk_id == 0 || risk_id == 1 || risk_id == 2)
    {
        key += 10000000;
    }
    if( risk_id == 2)
    {
        key += 30 * 24 * 10;    //Medium risk extend one month
    }
    //letter priority
    if (letter == true)
    {
        key += 100000000;
    }
    //最大转为最小
    key = max_key - key;
    return;
}
template<class T> T person<T>::return_key()
{   
    return key;
}

template<class T>string person<T>::show_id()
{
    return this->id;
}
// template<class T>string person<T>::show_name()
// {
//     return this->name;
// }
// template<class T>string person<T>::show_email()
// {
//     return this->email;
// }
// template<class T>string person<T>::show_address()
// {
//     return this->address;
// }
// template<class T> string person<T>::show_prof()
// {
//     return this->prof;
// }
// template<class T> int person<T>::show_prof_id()
// {
//     return this->show_prof_id();
// }
// template<class T> string person<T>::show_age()
// {
//     return this->age;
// }
// template<class T> int person<T>::show_age_id()
// {
//     return this->age_id;
// }
// template<class T> string person<T>::show_risk()
// {
//     return this->risk;
// }
// template<class T> int person<T>::show_risk_id()
// {
//     return this->risk_id;
// }
template<class T> double* person<T>::show_hour()
{
    return Time;
}
template<class T> int* person<T>::show_day()
{
    int* Day = new int[2];
    for(int i = 0;i < 2; i++)
    {
        if(((int)this->Time[i]) == -1){
            Day[i] = -1;
            continue;
        }   
        Day[i] = (int)(this->Time[i] / 24);
    }
    return Day;
}
template<class T>int* person<T>::show_half_day()
{
    int* half_day = new int[2];
    for(int i = 0; i < 2;i++)
    {
        if(((int)this->Time[i]) == -1)
        {
            half_day[i] = -1;
            continue;
        }
        half_day[i] = (int)(this->Time[i] / 12);
    }
    return half_day;
}
template<class T> int* person<T>::show_week()
{
    int* Week = new int[2];
    for(int i = 0; i < 2; i++)
    {
        if(((int)this->Time[i]) == -1){
            Week[i] = -1;
            continue;
        }
        Week[i] = (int)(this->Time[i] / (24 * 7));   
    }
    return Week;
}

template<class T> int* person<T>::show_month()
{
    int* Month = new int[2];
    for(int i = 0; i < 2;i++)
    {
        if(((int)this->Time[i]) == -1){
            Month[i] = -1;
            continue;;
        }
        Month[i] = (int)(this->Time[i] / (24*28));    
    }
    return Month;
}

template<class T>int* person<T>::show_intHour()
{
    int* intHour = new int[2];
    for(int i = 0; i < 2;i++)
    {
        if(((int)Time[i]) == -1)
        {
            intHour[i] = -1;
            continue;
        }
        intHour[i] = (int) Time[i];
    }
    return intHour;

}
template<class T>string* person<T>::show_format_time()
{
    int* Month = new int[2];
    int* Day = new int[2];
    double* Hour = new double[2];
    int* Minute = new int[2];
    int* intHour = new int[2];
    string* format_time = new string[3];

    //Set month,day,hour,minute
    Month = this->show_month();
    Day = this->show_day();
    Hour = this->show_hour();
    intHour = this->show_intHour();

    //修正double计算造成的时间不准确
     for(int i = 0; i < 2;i++)
    {
        if(((int)this->Time[i]) == -1)
        {
            Minute[i] = -1;
            continue;
        }
        double dif = Hour[i] - intHour[i];
        if(abs(dif - 0) < 0.001) {Minute[i] = 0;}
        if(abs(dif - 0.1) < 0.001) {Minute[i] = 6;}
        if(abs(dif - 0.2) < 0.001) {Minute[i] = 12;}
        if(abs(dif - 0.3) < 0.001) {Minute[i] = 18;}
        if(abs(dif - 0.4) < 0.001) {Minute[i] = 24;}
        if(abs(dif - 0.5) < 0.001) {Minute[i] = 30;}
        if(abs(dif - 0.6) < 0.001) {Minute[i] = 36;}
        if(abs(dif - 0.7) < 0.001) {Minute[i] = 42;}
        if(abs(dif - 0.8) < 0.001) {Minute[i] = 48;}
        if(abs(dif - 0.9) < 0.001) {Minute[i] = 54;}
    }
    for(int i = 0; i < 2;i++)
    {
        if(((int)this->Time[i]) == -1)
        {
            format_time[i] = "N/A";
            continue;
        }
        format_time[i] = to_string(2022) + "-" + to_string(1+Month[i]) + "-" + to_string(Day[i] % 28 + 1) + " ";
        //standardize hour
        if((intHour[i] % 24) < 10)
            format_time[i] = format_time[i] + "0" + to_string(intHour[i] % 24) + ":";
        else
            format_time[i] = format_time[i] + to_string(intHour[i] % 24) + ":";

        //standardize minute;
        if(Minute[i] != 0)
            format_time[i] = format_time[i] + to_string(Minute[i]);
        //Format: 2022-3-27 19:22
        if(Minute[i] == 0)
            format_time[i] = format_time[i] + to_string(Minute[i]) + to_string(Minute[i]);
    }
    return format_time;
}
template<class T>bool person<T>::is_update(person<int>* oldper)
{  
    if (withdraw == oldper->is_withdraw()){return true;}
    else{return false;}
}
template<class T> bool person<T>::is_newwithdraw(person<int>* oldper){
      if (withdraw == true && oldper->is_withdraw()==false){return true;}//新改的
      else{return false;}
}
template<class T> bool person<T>::is_withdraw()
{
    return withdraw;
}
template<class T> void person<T>::set_appointment(appointment* r_appoint)
{
    p_appoint = r_appoint;
    if(r_appoint!=NULL){
    Time[1] = r_appoint->the_time;}//改了
}

template<class T>void person<T>::punish()
{
    this->Time[0] += 14 * 24;
    this->set_key();    
}
template<class T>void person<T>::display_all()
{
    cout << endl;
    cout << "--------------------------" << endl;
    cout << "----Personal Information----" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Address: " << address << endl;
    cout << "Phone: " << phone << endl;
    cout << "Wechat: " << Wechat << endl;
    cout << "Email: " << email << endl;
    cout << "Profession: " << prof << endl;
    cout << "Profession ID: " << prof_id << endl;
    cout << "Age: " << age << endl;
    cout << "Age ID: " << age_id << endl;
    cout << "Risk Statu: " << risk << endl;
    cout << "Risk ID: " << risk_id << endl;
    cout << "Registration Time: " << this->show_format_time()[0] << endl;
    cout << "Withdraw: " << withdraw << endl;
    cout << "Priority letter: " << letter << endl;
    cout << "Local Registry ID: " << local_id << endl;
    cout << "Order of Hospital: ";
    if(hosp_num == 1)
        cout << hospital_id[0] << endl;
    if(hosp_num == 2)
        cout << hospital_id[0] << " " << hospital_id[1] << endl;
    if(hosp_num == 3)
        cout << hospital_id[0] << " " << hospital_id[1] << " " << hospital_id[2] << endl;
    cout << "----Further Information----" << endl;
    cout << "Status: " << this->status << endl;
    cout << "Fibonacci key: " << this->return_key() << endl;
    cout << "Readable key: " << 117620161 - this->return_key() << endl;
    cout << "Registration Total Hour: " << this->show_hour()[0] << endl;
    cout << "Registration Total Day: " << this->show_day()[0] << endl;
    cout << "Registration Total Half Day: " << this->show_half_day()[0] << endl;
    cout << "Registration Total Week: " << this->show_week()[0] << endl;
    cout << "Registration Total Month: " << this->show_month()[0] << endl;
    cout << "--------------------------" << endl;
    cout << endl;
}

void TableWrite::table_open(string filename)

{
    outfile.open(filename);
    if(!outfile.is_open())
    {
        cout << "Error: Opening a file fails" << endl;
        exit(EXIT_FAILURE);
    }
}
void TableWrite::table_close()
{
    outfile.close();   
}
string TableWrite::table_line(person<int>& p)
{
    string line;
    string hopsital_line = "";
    for(int i = 0; i < p.hosp_num; i++)
    {
        hopsital_line = hopsital_line + to_string(p.hospital_id[i]);
    }
    line = p.id+","+p.name+","+p.address+","+p.phone+","+p.Wechat+","+p.email+","+p.prof+","+p.age+","+p.risk+","+p.show_format_time()[0]+","+to_string(p.withdraw)+","+to_string(p.letter)+","+to_string(p.local_id)+","+hopsital_line;
    return line;
}
void TableWrite::table_create(string filename,int num)
{
    this->table_open(filename);
    //第一行
    string first_row;
    first_row = "ID,Name,Address,Phone,WeChat,Email,Profession,Age,Risk Status,Registration Time,Withdraw,Priority Letter,Local Registry ID,Hopsital ID\n" ;
    outfile << first_row;
    //创造num 个人
    person<int>* p = new person<int>[num];
    for(int i = 0;i < num;i++)
    {
        p[i].random_generate(i);
    }

    //按注册时间selection sort
    person<int> pmin = p[0]; 
    int pmin_index = 0;
    person<int>* sorted_p = new person<int>[num];

    for(int i =0; i < num;i++)
    {
        //选出最小的
        for(int j = 0; j < num - i; j++)
        {
            if(p[j].Time[0] < pmin.Time[0]){
                pmin = p[j];
                pmin_index = j;
            }
        }
        //加到sorted list去
        sorted_p[i] = pmin;
        //把pmin从原list删除
        for(int j = pmin_index; j < num - i - 1;j++)
        {
            p[j] = p[j+1];
        }
        //更新pmin
        pmin = p[0];
        pmin_index = 0;
    }

    //Every half day, print end of half day
    int half_day_index;
    int p_index = 0;
    for(half_day_index = 0; half_day_index < 168; half_day_index++)
    {
        
        while( (p_index < num) && (sorted_p[p_index].show_half_day()[0] == half_day_index) && (half_day_index < 168))
        {
            outfile << this->table_line(sorted_p[p_index]) << "\n";
            p_index++;
        }
        outfile << "end of half day" << "\n";
    } 
    this->table_close();
}

void reportlist::push_new(int r_status, person<int>* patient){
    rl.push_back(patient) ;
    patient->status=r_status ;
}

void reportlist::push_old(int r_status, person<int>* patient){
    int rlsize= rl.size() ;
    patient->status=r_status;
    for(int i=0 ; i<rlsize;i++){
        if(rl[i]->show_id()==patient->show_id()){
            rl[i]=patient;   //if find this patient in the reportlist, then update it
    
        }
    }
    

    

}

void reportlist::push_appoi(int r_status, appointment* r_appointment){
    person<int>*  apperson= exist(r_appointment->patient) ;
    if(apperson==NULL){
        return ; //if not find this person
    }
    apperson->set_appointment(r_appointment); //需要加函数
    apperson->status= r_status;
}

person<int>* reportlist::exist(person<int>* patient){
    int rlsize= rl.size() ;
    for(int i=0 ; i<rlsize;i++){
        if(rl[i]->show_id()==patient->show_id()){
            return rl[i] ;   //if find this patient in the reportlist, then return person's addr
        }
    }
    return NULL ; //if not find , return null

}

//fibo
template <class T>
Fb_heap<T>::Fb_heap()
{
    keyNum = 0;
    maxDegree = 0;
    min = NULL;
    cons = NULL;
}

template <class T>
Fb_heap<T>::~Fb_heap()
{
}

/*
 * 将node从双链表移除
 */
template <class T>
void Fb_heap<T>::removeNode(Fb_node<T> *node)
{
    node->left->right = node->right;
    node->right->left = node->left;
}

/*
 * 将node堆结点加入root结点之前(循环链表中)
 *   a …… root
 *   a …… node …… root
*/
template <class T>
void Fb_heap<T>::addNode(Fb_node<T> *node, Fb_node<T> *root)
{
    node->left        = root->left;
    root->left->right = node;
    node->right       = root;
    root->left        = node;
}

/*
 * 将节点node插入到斐波那契堆中
 */
template <class T>
void Fb_heap<T>::insert(Fb_node<T> *node)
{
    if (keyNum == 0)
        min = node;
    else
    {
        addNode(node, min);
        if (node->key < min->key)
            min = node;
    }
    keyNum++;
}

/*
 * 新建键值为key的节点，并将其插入到斐波那契堆中
 */
template <class T>
void Fb_heap<T>::insert(person<int>* patient)
{
    Fb_node<T> *node;

    node = new Fb_node<T>(patient);
    if (node == NULL)
        return ;

    insert(node);
}

/*
 * 将双向链表b链接到双向链表a的后面
 *
 * 注意： 此处a和b都是双向链表
 */
template <class T>
void Fb_heap<T>::catList(Fb_node<T> *a, Fb_node<T> *b)
{
    Fb_node<T> *tmp;

    tmp            = a->right;
    a->right       = b->right;
    b->right->left = a;
    b->right       = tmp;
    tmp->left      = b;
}

/*
 * 将"堆的最小结点"从根链表中移除，
 * 这意味着"将最小节点所属的树"从堆中移除!
 */
template <class T>
Fb_node<T>* Fb_heap<T>::extractMin()
{
    Fb_node<T> *p = min;

    if (p == p->right)
        min = NULL;
    else
    {
        removeNode(p);
        min = p->right;
    }
    p->left = p->right = p;

    return p;
}

/*
 * 将node链接到root根结点
 */
template <class T>
void Fb_heap<T>::link(Fb_node<T>* node, Fb_node<T>* root)
{
    // 将node从双链表中移除
    removeNode(node);
    // 将node设为root的孩子
    if (root->child == NULL)
        root->child = node;
    else
        addNode(node, root->child);

    node->parent = root;
    root->degree++;
    node->marked = false;
}

/*
 * 创建consolidate所需空间
 */
template <class T>
void Fb_heap<T>::makeCons()
{
    int old = maxDegree;

    // 计算log2(keyNum)，"+1"意味着向上取整！
    // ex. log2(13) = 3，向上取整为3+1=4。
    maxDegree = (log(keyNum)/log(2.0)) + 1;
    if (old >= maxDegree)
        return ;

    // 因为度为maxDegree可能被合并，所以要maxDegree+1
    cons = (Fb_node<T> **)realloc(cons,
                                  sizeof(Fb_heap<T> *) * (maxDegree + 1));
}

/*
 * 合并斐波那契堆的根链表中左右相同度数的树
 */
template <class T>
void Fb_heap<T>::consolidate()
{
    int i, d, D;
    Fb_node<T> *x, *y, *tmp;

    makeCons();//开辟哈希所用空间
    D = maxDegree + 1;

    for (i = 0; i < D; i++)
        cons[i] = NULL;

    // 合并相同度的根节点，使每个度数的树唯一
    while (min != NULL)
    {
        x = extractMin();                // 取出堆中的最小树(最小节点所在的树)
        d = x->degree;                    // 获取最小树的度数
        // cons[d] != NULL，意味着有两棵树(x和y)的"度数"相同。
        while (cons[d] != NULL)
        {
            y = cons[d];                // y是"与x的度数相同的树"
            if (x->key > y->key)        // 保证x的键值比y小
                swap(x, y);

            link(y, x);    // 将y链接到x中
            cons[d] = NULL;
            d++;
        }
        cons[d] = x;
    }
    min = NULL;

    // 将cons中的结点重新加到根表中
    for (i=0; i<D; i++)
    {
        if (cons[i] != NULL)
        {
            if (min == NULL)
                min = cons[i];
            else
            {
                addNode(cons[i], min);
                if ((cons[i])->key < min->key)
                    min = cons[i];
            }
        }
    }
}

/*
 * 移除最小节点
 */
template <class T>
person<int>* Fb_heap<T>::delete_min()
{
    if (min==NULL)
        return NULL;

    Fb_node<T> *child = NULL;
    Fb_node<T> *m = min;
    person<int>* rtval= min->patient ;
    // 将min每一个儿子(儿子和儿子的兄弟)都添加到"斐波那契堆的根链表"中
    while (m->child != NULL)
    {
        child = m->child;
        removeNode(child);
        if (child->right == child)
            m->child = NULL;
        else
            m->child = child->right;

        addNode(child, min);
        child->parent = NULL;
    }

    // 将m从根链表中移除
    removeNode(m);
    // 若m是堆中唯一节点，则设置堆的最小节点为NULL；
    // 否则，设置堆的最小节点为一个非空节点(m->right)，然后再进行调节。
    if (m->right == m)
        min = NULL;
    else
    {
        min = m->right;
        consolidate();
    }
    keyNum--;

    delete m;
    return rtval ;
}


/*
 * 修改度数
 */
template <class T>
void Fb_heap<T>::renewDegree(Fb_node<T> *parent, int degree)
{
    parent->degree -= degree;
    if (parent-> parent != NULL)
        renewDegree(parent->parent, degree);
}

/*
 * 将node从父节点parent的子链接中剥离出来，
 * 并使node成为"堆的根链表"中的一员。
 */
template <class T>
void Fb_heap<T>::cut(Fb_node<T> *node, Fb_node<T> *parent)
{
    removeNode(node);
    renewDegree(parent, node->degree);
    // node没有兄弟
    if (node == node->right)
        parent->child = NULL;
    else
        parent->child = node->right;

    node->parent = NULL;
    node->left = node->right = node;
    node->marked = false;
    // 将"node所在树"添加到"根链表"中
    addNode(node, min);
}

/*
 * 对节点node进行"级联剪切"
 *
 * 级联剪切：如果减小后的结点破坏了最小堆性质，
 *     则把它切下来(即从所在双向链表中删除，并将
 *     其插入到由最小树根节点形成的双向链表中)，
 *     然后再从"被切节点的父节点"到所在树根节点递归执行级联剪枝
 */
template <class T>
void Fb_heap<T>::cascadingCut(Fb_node<T> *node)
{
    Fb_node<T> *parent = node->parent;
    if (parent != NULL)
    {
        if (node->marked == false)
            node->marked = true;
        else
        {
            cut(node, parent);
            cascadingCut(parent);
        }
    }
}

/*
 * 将斐波那契堆中节点node的值减少为key
 */
template <class T>
void Fb_heap<T>::decrease(Fb_node<T> *node, T key)
{
    Fb_node<T> *parent;

    if (min==NULL ||node==NULL)
        return ;

    if ( key>=node->key)
    {
        cout << "decrease failed: the new key(" << key <<") "
             << "is no smaller than current key(" << node->key <<")" << endl;
        return ;
    }

    node->key = key;
    parent = node->parent;
    if (parent!=NULL && node->key < parent->key)
    {
        // 将node从父节点parent中剥离出来，并将node添加到根链表中
        cut(node, parent);
        cascadingCut(parent);
    }

    // 更新最小节点
    if (node->key < min->key)
        min = node;
}

/*
 * 将斐波那契堆中节点node的值增加为key
 */
template <class T>
void Fb_heap<T>::increase(Fb_node<T> *node, T key)
{
    Fb_node<T> *child, *parent, *right;

    if (min==NULL ||node==NULL)
        return ;

    if (key <= node->key)
    {
        cout << "increase failed: the new key(" << key <<") "
             << "is no greater than current key(" << node->key <<")" << endl;
        return ;
    }

    // 将node每一个儿子(不包括孙子,重孙,...)都添加到"斐波那契堆的根链表"中
    while (node->child != NULL)
    {
        child = node->child;
        removeNode(child);               // 将child从node的子链表中删除
        if (child->right == child)
            node->child = NULL;
        else
            node->child = child->right;

        addNode(child, min);       // 将child添加到根链表中
        child->parent = NULL;
    }
    node->degree = 0;
    node->key = key;

    // 如果node不在根链表中，
    //     则将node从父节点parent的子链接中剥离出来，
    //     并使node成为"堆的根链表"中的一员，
    //     然后进行"级联剪切"
    // 否则，则判断是否需要更新堆的最小节点
    parent = node->parent;
    if(parent != NULL)
    {
        cut(node, parent);
        cascadingCut(parent);
    }
    else if(min == node)
    {
        right = node->right;
        while(right != node)
        {
            if(node->key > right->key)
                min = right;
            right = right->right;
        }
    }
}

/*
 * 更新斐波那契堆的节点node的键值为key
 */
template <class T>
void Fb_heap<T>::update(Fb_node<T> *node, T key)
{
    if(key < node->key)
        decrease(node, key);
    else if(key > node->key)
        increase(node, key);
    else
        cout << "No need to update!!!" << endl;
}

template <class T>
void Fb_heap<T>::update(person<int>*newpatient, person<int>* oldpatient)
{
    Fb_node<T> *node;
    int newkey = newpatient->return_key() ;
    node = search(oldpatient);
    if (node!=NULL){
        update(node, newkey);
        node->patient=newpatient ;
    }
}

/*
 * 在最小堆root中查找键值为key的节点
 */
template <class T>
Fb_node<T>* Fb_heap<T>::search(Fb_node<T> *root, person<int>* spatient)
{
    Fb_node<T> *t = root;    // 临时节点
    Fb_node<T> *p = NULL;    // 要查找的节点
    int tempkey = spatient->return_key() ;
    string tempid= spatient->show_id();
    if (root==NULL)
        return root;

    do
    {
        if ((t->key == tempkey) &&(t->patient->show_id()==tempid))
        {
            p = t;
            break;
        }
        else
        {
            if ((p = search(t->child, spatient)) != NULL)
                break;
        }
        t = t->right;
    } while (t != root);

    return p;
}

/*
 * 在斐波那契堆中查找键值为key的节点
 */
template <class T>
Fb_node<T>* Fb_heap<T>::search(person<int>* patient)
{
    if (min==NULL)
        return NULL;

    return search(min, patient);
}


/*
 * 删除结点node
 */
template <class T>
void Fb_heap<T>::remove(Fb_node<T> *node)
{
    T m = min->key-1;
    decrease(node, m-1);
    delete_min();
}

template <class T>
void Fb_heap<T>::remove(person<int>* oldpatient)
{
   Fb_node<T> *node;

    if (min==NULL)
        return ;

    node = search(oldpatient);
    if (node==NULL)
        return ;

    remove(node);
}

/*
 * 销毁斐波那契堆
 */
template <class T>
void Fb_heap<T>::destroyNode(Fb_node<T> *node)
{
    Fb_node<T> *start = node;

    if(node == NULL)
        return;

    do {
        destroyNode(node->child);
        // 销毁node，并将node指向下一个
        node = node->right;
        delete node->left;
    } while(node != start);
}

template <class T>
void Fb_heap<T>::destroy()
{
    destroyNode(min);
    free(cons);
}

/*
 * 打印"斐波那契堆"
 *
 * 参数说明：
 *     node       -- 当前节点
 *     prev       -- 当前节点的前一个节点(父节点or兄弟节点)
 *     direction  --  1，表示当前节点是一个左孩子;
 *                    2，表示当前节点是一个兄弟节点。
 */
template <class T>
void Fb_heap<T>::print(Fb_node<T> *node, Fb_node<T> *prev, int direction)
{
    Fb_node<T> *start=node;

    if (node==NULL)
        return ;
    do
    {
        if (direction == 1)
            cout << setw(8) << node->key << "(" << node->degree << ") is "<< setw(2) << prev->key << "'s child" << endl;
        else
            cout << setw(8) << node->key << "(" << node->degree << ") is "<< setw(2) << prev->key << "'s next" << endl;

        if (node->child != NULL)
            print(node->child, node, 1);

        // 兄弟节点
        prev = node;
        node = node->right;
        direction = 2;
    } while(node != start);
}

template <class T>
void Fb_heap<T>::print()
{
    int i=0;
   Fb_node<T> *p;

    if (min==NULL)
        return ;

    cout << "== 斐波那契堆的详细信息: ==" << endl;
    p = min;
    do {
        i++;
        cout << setw(2) << i << ". " << setw(4) << p->key << "(" << p->degree << ") is root" << endl;

        print(p->child, p, 1);
        p = p->right;
    } while (p != min);
    cout << endl;
}


hospital::hospital(int idnum, int capacity, double o_time, double c_time)
:id(idnum), daily_capacity(capacity), open_time(o_time), close_time(c_time){
for (int j=0; j<100; j++){daily_remaining_capacity[j]=daily_capacity;
}
//initalize all the remaining capacity as daily capacity

treatment_time=(close_time - open_time)/(double)daily_capacity;


}//测试过，是正确的

//this function checks whether the hospital is still available for that day
bool hospital::is_hospital_available(int h_date){
if(daily_remaining_capacity[h_date]>0){return true;}
else{return false;}
}//测试过，是正确的


//this function returns the next available time for the patient, and decrease the remaining capacity
double hospital::get_time_appointed(int h_date){
    double atime;
    atime=open_time+((double)(daily_capacity-daily_remaining_capacity[h_date]))*treatment_time;
    daily_remaining_capacity[h_date]--; //decrease the remaining capacity when one time is appointed
    return atime;
  }//测试正确

//this function add the new appointment to the hospital's apppointment list
  void hospital::add_to_applist(appointment* new_app){
daily_appointment_list.push_back(new_app);
return;
  }//测试正确

void hospital::printapp(){
  for (int i=0; i<daily_appointment_list.size();i++){//新修改，加了.
  daily_appointment_list[i]->pretty_print();
  }
  return;
}//测试正确

void hospital::withdraw_app(appointment* w_app){
for (vector<appointment*>::iterator iter=daily_appointment_list.begin();iter!=daily_appointment_list.end();iter++){
  if(*iter==w_app){
    daily_appointment_list.erase(iter);
    break;
  }
}//find that appointment and delete it from the appointment list
w_app->patient->set_appointment(NULL);//set that person's appointment pointer to be null
 //这里不行啊，make appointment 里面是不是没放人啊，不是放了吗，set appointment 里面不能是0
}//现在测试这个


appointment::appointment(person<int>* a_patient, int date_out_of_queue, vector<hospital*> a_hospital_list)
:patient(a_patient),hospitals(a_hospital_list){
    date= date_out_of_queue+1;//by default the appointment date should be the next day the person is out of queue
    hospital_id=-1;
    
    
    
}//测试正确

void appointment:: make_appointment(){
while(hospital_id==-1&&date<100){//keep searching for days until available hospital is found
int i=0;
while (i< patient->hosp_num && (hospitals[i])->is_hospital_available(date) == false){i++;//新的修改，[]里应该是i
    //in one day, keep searching for all hospitals in the person's hospital ranking list, until 
    //one available is found
}

if(i<patient->hosp_num){
    hospital_id=patient->hospital_id[i]; //新的修改,不叫ranking list了
    the_time=(hospitals[hospital_id])->get_time_appointed(date);}
else{date++;
}
}

return;
}//测试正确

void appointment::pretty_print(){
     cout<<"patient id is:"<<patient->show_id()<<"\n";
     cout<<"hospital id is: "<<hospital_id<<"\n";
     cout<<"date is: "<<date<<"\n";
     cout<<"time is: "<<the_time<<"\n";
  
}//测试正确



bool appointment::is_appointment_passed(double current_hour){
double app_hour = (double)((date-1)*24)+the_time;//新修改，改为date-1了,time-the_time
if(app_hour<=current_hour){return true;}
else{return false;}

}//测试正确

int main(){

    cout << ".." << endl;
  int month=0;
  int week=0;
  int day=0;
  int halfday=0; 
  double hour=0;//initialize all the parameters that represents time as zero

  fifo<person<int>*>* localqueue_0 = new fifo<person<int>*> ;
  fifo<person<int>*>* localqueue_1 = new fifo<person<int>*> ;
  fifo<person<int>*>* localqueue_2 = new fifo<person<int>*> ;//set up three empty local queues;
  vector<fifo<person<int>*>*> localqueues;
  localqueues.push_back(localqueue_0);
  localqueues.push_back(localqueue_1);
  localqueues.push_back(localqueue_2);
  

  hospital* hospital_0 = new hospital(0, 20, 8.0, 16.0);//set up first hospital with 10 daily capacity, open at 8am, close at 16pm
  hospital* hospital_1 = new hospital(1, 2, 9, 16.5);//测试用set up second hospital with 5 daily capacity, open at 9am, close at 16:30pm
  hospital* hospital_2 = new hospital(2, 15, 8.0, 17.0 );//set up third hospital with 15 daily capacity, open at 8am, close at 17pm
  vector<hospital*> hospital_list;
  hospital_list.push_back(hospital_0);
  hospital_list.push_back(hospital_1);
  hospital_list.push_back(hospital_2);//put all three hospital's pointer in a hospital list.
  

  Fb_heap<int> fibo_h;//set up an empty fibonacciheap

  reportlist a_reportlist;//set up a report list that stores all the patient's information, and is used to generate report
 
  ifstream infile;
  infile.open("./input.csv");   //connect  stream with inputfile  
	assert(infile.is_open());   //if fail, output error and exit the programm
  string aline;
  string endmark="end of half day";//the string that marks end of halfday input

getline(infile, aline);//first skip the first line, which is the table head
int x=0; //测试用
while (getline(infile, aline)){//测试用十八行数据，第三次end of day，测试state 2,withdraw
x++;
if(aline.find(endmark)!=0  ){//if a endmark string is not found at the start of aline, the line contain register information
  person<int>* a_patient = new person<int>(aline);//create a tempary person from the input file line
  int local_index=a_patient->local_id;
  (*(localqueues[local_index])).pushback(a_patient);//push that person's adress into the local queue
  
}//都对了
  

else{//if a endmark string is found at the start of aline, the program has already finished reading halfday information 
  halfday++;
  hour+=12.0;
  for(int k=0; k<a_reportlist.rl.size(); k++){
    if((a_reportlist.rl)[k]->p_appoint!=NULL&&(a_reportlist.rl)[k]->p_appoint->is_appointment_passed(hour)==true){
     (a_reportlist.rl)[k]->status=3; //check all people in report list, if that person's appointment has passed, mark it as status 3 
    }
  } //测试正确

  for(int i=0; i<3; i++){
    while(  (*localqueues[i]).isempty() == false ){//pop all the elements from local queue twice daily
    person<int>* temp_person = (*localqueues[i]).popfront();
    if(a_reportlist.exist(temp_person)==NULL){//after checking id in report list, if that person is not already in the report list
    fibo_h.insert(temp_person);//insert that person into the fibonacci heap 
    a_reportlist.push_new(1,temp_person);//push that person to reportlist and change status to be in the fibonacci heap
    
    }//如果这个人是第一此登记的这个if测试正确，fibo顺序未测
    
    
    else{//if that person is already in the report list
   
    person<int>* old_person_ptr = a_reportlist.exist(temp_person);//新改的
    if(old_person_ptr->status == 1){//if that person's status is 1, it means it is still in the heap
      if(temp_person->is_update(old_person_ptr)==true){// if this is an update 
      cout<<temp_person->show_id()<<" information updated\n";//到时候要加吗
      a_reportlist.push_old(1,temp_person);//update the information in the reportlist and change status to be in the fibonacci heap
      fibo_h.update(temp_person, old_person_ptr );//update the fibonacci heap with the updated old person accordingly
     }//测试正确
      if(temp_person->is_newwithdraw(old_person_ptr)==true){
      cout<<temp_person->show_id()<<" withdraw when waiting in heap\n";//到时候要加吗
      fibo_h.remove(old_person_ptr);//if that person withdraws, delete him from fibonacci heap
      a_reportlist.push_old(5,temp_person);//update the information in the report list and change status to be after treated
      }//测试正确
    }
    if(old_person_ptr->status == 2){
      if(temp_person->is_update(old_person_ptr)==true){
      a_reportlist.push_old(2,temp_person);//不行呀，这样appointment 岂不是没了
      a_reportlist.push_appoi(2,old_person_ptr->p_appoint);//改了 merge update information and appointment information

      }//还没测试
      if(temp_person->is_newwithdraw(old_person_ptr)==true){
        cout<<temp_person->show_id()<<" withdraws after treatment but before appointment time\n";//测试用
        a_reportlist.push_old(5, old_person_ptr);//change old person's status to be 5,改了
        int hos_id=old_person_ptr->p_appoint->hospital_id;//改了if that person withdraws after making appointment but before appointment date passed,
        hospital_list[hos_id]->withdraw_app(old_person_ptr->p_appoint);// remove him from hospital appointment list and set person's appoitnemnt to be null
      }//测试成功了
      
    }/*
    if(old_person_ptr->status == 3){
      if(temp_person->is_update(old_person_ptr)==true){
      a_reportlist.push_old(3,temp_person);
      a_reportlist.push_appoi(3,old_person_ptr->p_appoint);
      }
      if(temp_person->is_newwithdraw(old_person_ptr)==true){
        //mark that this person do not go to appointment, which is status 4
      a_reportlist.push_old(4,temp_person);


        }
       }
    if(old_person_ptr->status == 4 || old_person_ptr->status == 5){
       old_person_ptr->punish();//use a person's member fuction to change the register time, and set new key value
       fibo_h.insert(temp_person);//insert that person into the fibonacci heap 
       a_reportlist.push_new(1,temp_person);//push that person to reportlist and change status to be in the fibonacci heap
     
     }  */
      }//else
      
     }//while pop未空
    }//for i=0-3
   
  
  if(halfday%2 == 0){
  day++;
  int f_num=fibo_h.getnum();
  for(int j=0; j<f_num/2; j++){//比例最好大于一半？,现在一天pop掉所有
  // extract the half number of people with min key from the fibonacci heap everday
  person<int>* temp_person = fibo_h.delete_min(); 
  appointment* a_appointment= new appointment(temp_person, day, hospital_list);
  a_appointment->make_appointment();
  int hos_id=a_appointment->hospital_id;
  hospital_list[hos_id]->add_to_applist(a_appointment);//make appointment and add them to the fibonacci heap

  a_reportlist.push_appoi(2,a_appointment);//update that person's status and set his appointment
  temp_person->set_assign_appointment_time(hour);
  
  }
  }
  if(halfday%14==0){
   week++;
   Weeklyreport tem_weeklyreport(a_reportlist.rl, hour);
   tem_weeklyreport.Save(week,"Weekly");//generate the three weekly report 
   
   tem_weeklyreport.Sort(week);//and sort them according to the user's preference
  }
  if(halfday%56==0){
  month++;
  Monthlyreport tem_monthlyreport(a_reportlist.rl, hour);
  tem_monthlyreport.Save(month);//generate the monthly report
  }//increment the time relavent counters and output weekly and monthly report
  
 }//else 半天结束
 

}//所有行读取结束
  infile.close();
return 0;
}



