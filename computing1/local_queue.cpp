// Data structure for local_queue

#include <iostream>
#include <fstream>
#include "head.h"
#include <string>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <cmath>
using namespace std;


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
    updated = false;
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
    int first_index = rand() % 9 + 1;//Id won't start with zero
    id[0] = integer_set[first_index];

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
    first_index = rand() % 9 + 1;//Id wont't start with zero
    phone[0] = integer_set[first_index];

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
    first_index = rand() % 9 + 1;   //Email won't start with zero
    email_prefix[0] = integer_set[first_index];
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
    updated = false;

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
//除了withdraw状态改变的，其他的都算update
template<class T>bool person<T>::is_update(person<int>* oldper)
{
    if (withdraw == oldper->is_withdraw())
    {
        return true;
    }
    else
    {
        return false;
    }
}
template<class T> bool person<T>::is_newwithdraw(person<int>* oldper)
{
    if (withdraw == true && oldper->is_withdraw())
    {
        return true;
    }
    else
    {
        return false;
    }
}
template<class T> bool person<T>::is_withdraw()
{
    return withdraw;
}
template<class T> void person<T>::set_appointment(appointment* r_appoint)
{
    p_appoint = r_appoint;
    Time[1] = r_appoint->the_time;
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
void TableWrite::table_create(string filename,int num,int updated_num,int withdraw_num, int letter_num)
{
    if(num <= 0 || updated_num < 0 || withdraw_num < 0 || letter_num < 0)
    {
        cout << "As a normal person with normal intelligence, at least you shouldn't..." << endl;
        exit(EXIT_FAILURE);
    }
    if(updated_num > num / 2)
    {
        cout << "Too many people are updated! What are you going to do?" << endl;
        exit(EXIT_FAILURE);
    }
    if(withdraw_num > num / 2)
    {
        cout << "Too many people withdraw! People don't regret that often, unless there is something wrong with your mind!" << endl;
        exit(EXIT_FAILURE);
    }
    if(letter_num > 3 * num / 4)
    {
        cout << "If so many people have letters, what's the point of not having one?" << endl;
        exit(EXIT_FAILURE);
    }
    if(updated_num + withdraw_num > 3 * num / 4)
    {
        cout << "Too people who update and withdraw. That's not a good choice, honestly" << endl;
        exit(EXIT_FAILURE);
    }

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

   // 随机取若干人，更新
    for(int i = 0; i < updated_num; i++)
    {
        int p1_index = rand() % (3 * num / 4);  //前3/4人均可能有更新，但不会在num/4人后更新
        int p2_index = p1_index + rand() % (num / 4);
        // if(sorted_p[p2_index].updated == true)
        // {
        //     continue;
        // }   //更新人不再更新考虑
        person<int> p1 = sorted_p[p1_index];
        person<int> p2 = sorted_p[p2_index];
        person<int> new_p2;
        new_p2 = p1;
        new_p2.prof = p2.prof;
        new_p2.prof_id = p2.prof_id;
        new_p2.risk = p2.risk;
        new_p2.risk_id = p2.risk_id;
        new_p2.Time = p2.Time;
        new_p2.updated = true;
        sorted_p[p2_index] = new_p2;
        cout << sorted_p[p2_index].id << " " << sorted_p[p2_index].show_format_time()[0] << " " << p1.id << " " << p1.show_format_time()[0] << endl;
    }
    //随机取若干人，withdraw
    for(int i = 0; i < withdraw_num; i++)
    {
        int p1_index = rand() % (3 * num / 4); //前3/4人可能withdraw，后面均不可能，withdraw的在1/4 个人之内重新注册
        while( sorted_p[p1_index].updated == true )
        {
            p1_index = rand() % (3 * num / 4);  //找到没有更新的人可以withdraw
        }
        person<int> p1 = sorted_p[p1_index];
        int p2_index = p1_index + rand() % (num / 4);
        //p2位置的人被p1覆盖
        person<int> new_p2 = p1;
        new_p2.Time = sorted_p[p2_index].Time;
        new_p2.withdraw = true;
        sorted_p[p2_index] = new_p2;
        //cout << sorted_p[p2_index].id << " " << sorted_p[p2_index].show_format_time()[0] << " " << p1.id << " " << p1.show_format_time()[0] << endl;
    }
    //随机取若干人，赋予priority letter
    for(int i = 0; i < letter_num; i++)
    {
        int index = rand() % num;
        while(sorted_p[index].letter == true)
        {
            index = rand() % num;
        }   //赋予过letter 的人不在赋予
        sorted_p[index].letter = true;        
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

int main()
{
    TableWrite t1;
    t1.table_create("./input.csv",1000,50,50,20);
    vector<person<int>*> person_set;

    ifstream infile;
    string aline;
    int num = 0;
    infile.open("./input.csv");
    getline(infile,aline);

    for(int i = 0; i < 100;i++)
    {
        getline(infile,aline);
        if(aline == "end of half day")
            continue;
        person<int>* p = new person<int>(aline);
        person_set.push_back(p);
        num++;
    }
    for(int i = 0; i < num; i++)
    {
        person_set[i]->display_all();
    }
}
