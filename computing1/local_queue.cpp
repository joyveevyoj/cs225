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

//Profession priority increase from this order
string prof_set[8] = {"Teacher","Actor","Farmer","Businessman","Journalist","Student","Engineer","Programmer"};
//Age priority increase from this order
string age_set[7] = {"Children","Adolescent","Young Adult","Adult","Senior","Elderly Person","Old Person"};
string risk_set[4] ={"No risk","Low risk","Medium risk","High risk"};
template<class T>fifo<T>::fifo(int size)
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
//Taking tabline as argument to create a person from input file
template<class T>person<T>::person(string tableline)
{
    //Separate the string
    int index = 0;  //The first position after comma
    int next_index; //The position of the second comma
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
    Time[3] = -1;


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
    status = 0; //Status initialized to 0
    wont_withdraw = false;
    //Given the known information, set the key
    set_key();
}

//Push random attributes to a person. Used to generate the input file
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
    int first_index = rand() % 9 + 1;//id won't start with zero
    id[0] = integer_set[first_index];

    //Create address
    vector<string>address_set;
    //Four possible addresses. Can be modified if necessary.
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
    index = rand() % 8;
    prof = prof_set[index];
    prof_id = index;

    //Create age;
    index = rand() % 7;
    age = age_set[index];
    age_id = index;

    //Create risk
    index = rand() % 4;
    risk = risk_set[index];
    risk_id = index;

    //Create Time
    Time = new double[2];
    Time[0] = rand() % 2016 + 0.1 * (rand() % 10);//2016h is not considered
    Time[1] = -1;
    Time[2] = -1;
    Time[3] = -1;

    //Create Withdraw and priority letter and other parameters;
    withdraw = false;
    letter = false;
    updated_half_day = -1;
    wont_withdraw = false;

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
    //Turn maximum to minimum for Fibonacci heap
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
template<class T>string person<T>::show_risk()
{
    return this->risk;
}

template<class T> double* person<T>::show_hour()
{
    return Time;
}
template<class T> int* person<T>::show_day()
{
    //Convert hour into day
    int* Day = new int[4];
    for(int i = 0;i < 4; i++)
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
    //convert hour into half day
    int* half_day = new int[4];
    for(int i = 0; i < 4;i++)
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
    //convert hour into week
    int* Week = new int[4];
    for(int i = 0; i < 4; i++)
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
    //convert hour into month
    int* Month = new int[4];
    for(int i = 0; i < 4;i++)
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
    //Get the integer part of hour
    int* intHour = new int[4];
    for(int i = 0; i < 4;i++)
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
    //convert to to standardized form like 2022-01-23 6:00
    int* Month = new int[4];
    int* Day = new int[4];
    double* Hour = new double[4];
    int* Minute = new int[4];
    int* intHour = new int[4];
    string* format_time = new string[4];

    //Set month,day,hour,minute
    Month = this->show_month();
    Day = this->show_day();
    Hour = this->show_hour();
    intHour = this->show_intHour();

    //Solve the problem of imprecision caused by using double type numbers
     for(int i = 0; i < 4;i++)
    {
        if(((int)this->Time[i]) == -1)
        {
            Minute[i] = -1;
            continue;
        }
        double dif = Hour[i] - intHour[i];
        if(abs(dif - 0) < 0.001){ Minute[i] = 0;}
        if(abs(dif - 0.1) < 0.001){ Minute[i] = 6;}
        if(abs(dif - 0.2) < 0.001){Minute[i] = 12;}
        if(abs(dif - 0.3) < 0.001){Minute[i] = 18;}
        if(abs(dif - 0.4) < 0.001){Minute[i] = 24;}
        if(abs(dif - 0.5) < 0.001){Minute[i] = 30;}
        if(abs(dif - 0.6) < 0.001){Minute[i] = 36;}
        if(abs(dif - 0.7) < 0.001){Minute[i] = 42;}
        if(abs(dif - 0.8) < 0.001){Minute[i] = 48;}
        if(abs(dif - 0.9) < 0.001){Minute[i] = 54;}
    }
    for(int i = 0; i < 4;i++)
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
        
        if(Minute[i] == 0)
            format_time[i] = format_time[i] + to_string(Minute[i]) + to_string(Minute[i]);
    }
    return format_time;
}

template<class T>bool person<T>::is_update(person<int>* oldper)
{
    //Except those with a withdraw, everything else is update
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
    if (withdraw == true && oldper->is_withdraw() == false)
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
//Set the assignment to this person
template<class T> void person<T>::set_appointment(appointment* r_appoint)
{
    p_appoint = r_appoint;
    if(r_appoint != NULL)
    {
        Time[1] = r_appoint->the_time;

    }
}


template<class T>void person<T>::withdraw_time(double hour)
{
    Time[3] = hour;
}

//Set the time when the appointment is assigned
template<class T>void person<T>::set_assign_appointment_time(double hour)
{
    Time[2] = hour;
}
//For those who withdrawed but registered, punish two weeks extentions time
template<class T>void person<T>::punish()
{
    if ((risk_id == 0) || (risk_id == 1))
    {
        this->Time[0] += 14 * 24;
        this->set_key();    
    }

}

//Display almost all the information of the person.
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

//Create a tableline based on attributes of the person
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
//Use selection sort to sort the set of people based on registration or update time
person<int>* TableWrite::SelectionSort(person<int>* p,int num_of_person,int way)
{
    person<int> pmin = p[0]; 
    int pmin_index = 0;
    person<int>* sorted_p = new person<int>[num_of_person];
    for(int i =0; i < num_of_person;i++)
    {
        //Find the minimum
        for(int j = 0; j < num_of_person - i; j++)
        {
            if(way == 0)
            {
                if(p[j].Time[0] < pmin.Time[0])
                {
                pmin = p[j];
                pmin_index = j;
                }
            }
            if(way == 1)
            {
                if(p[j].updated_half_day < pmin.updated_half_day)
                {
                    pmin = p[j];
                    pmin_index = j;
                }
            }
        }
        //Add it to sorted_p
        sorted_p[i] = pmin;
        //Delete pmin from the original list
        for(int j = pmin_index; j < num_of_person - i - 1;j++)
        {
            p[j] = p[j+1];
        }
        //update pmin
        pmin = p[0];
        pmin_index = 0;
    }
    return sorted_p;

}

TableWrite::TableWrite(string filename_,int num_,int updated_num_,int withdraw_num_)
{
    //Requirements for the input
    if(num <= 0 || updated_num < 0 || withdraw_num < 0)
    {
        cout << "As a normal person with normal intelligence, at least you shouldn't..." << endl;
        exit(EXIT_FAILURE);
    }
    if(updated_num > 3 * num / 4 )
    {
        cout << "Too many people are updated! Things don't change that often within three months. What are you going to do?" << endl;
        exit(EXIT_FAILURE);
    }
    if(withdraw_num > 1 * num / 2)
    {
        cout << "Too many people withdraw! People don't regret that often, unless there is something wrong with your mind!" << endl;
        exit(EXIT_FAILURE);
    }
    if(updated_num + withdraw_num > 4 * num / 5)
    {
        cout << "Too people who update and withdraw. That's not a good choice, honestly" << endl;
        exit(EXIT_FAILURE);
    }
    filename = filename_;
    num = num_;
    updated_num = updated_num_;
    withdraw_num = withdraw_num_;
}

//The function to create a table
void TableWrite::table_create()
{
    this->table_open(filename);
    srand((unsigned)(time(NULL)));
    string first_row;
    //Print the first row
    first_row = "ID,Name,Address,Phone,WeChat,Email,Profession,Age,Risk Status,Registration Time,Withdraw,Priority Letter,Local Registry ID,Hopsital ID\n" ;
    outfile << first_row;
    //Create num + withdraw_num people
    num = num + withdraw_num;
    person<int>* p = new person<int>[num];
    int last_halfday_num = 0;   //Number of people on the last half day
    for(int i = 0;i < num;i++)
    {
        p[i].random_generate(i);
        if(p[i].show_half_day()[0] == 167) {last_halfday_num++; }
    }
    if(last_halfday_num == 0)
    {
        p[num - 1].Time[0] = rand() % 12 + 2004 + 0.1 * (rand() % 9 + 1);  //To avoid some exceptions, there should be at least one person in the last day
        last_halfday_num = 1;
    }
    //selection sort based on registration time
    person<int>* sorted_p = this->SelectionSort(p,num,0);

    //Randomly select withdraw_num people and change them into withdraw
    person<int>* withdraw_p = new person<int>[withdraw_num];
    for(int i = 0; i < withdraw_num; i++)
    {
        int p1_index = rand() % (num - last_halfday_num); //People on the last day won't withdraw
        while(sorted_p[p1_index].wont_withdraw == true || sorted_p[p1_index].withdraw == true)
        {
            p1_index = rand() % (num - last_halfday_num);   //People who withdrawn won't withdraw
        }
        sorted_p[p1_index].wont_withdraw = true;
        person<int> p1 = sorted_p[p1_index];
        int p2_index = p1_index + rand() % ( 6 * num / 168) + 1;    //Additional 3 days on average
        //If index overflows or the same person is chosen or the covered person has withdrawn, choose again
        while(p2_index >= num || p2_index == p1_index || sorted_p[p2_index].wont_withdraw == true || sorted_p[p2_index].withdraw == true)
        {
            p2_index = p1_index + rand() % (20 * num / 168) + 1; //Find a better range
            //While loop forever attention! If all of the new person has withdrawn, it's possible to loop forever
        }
        //person in p2 is covered by p1 with witdraw set.
        person<int> new_p2 = p1;
        new_p2.Time = sorted_p[p2_index].Time;
        new_p2.withdraw = true;
        new_p2.wont_withdraw = false;
        sorted_p[p2_index] = new_p2;
    }
   //Randomly select updated_num people and update them
    person<int>* updated_p = new person<int>[updated_num];
    for(int i = 0; i < updated_num;i++)
    {
        int index = rand() % (num - last_halfday_num); //People on the last day won't update
        while(sorted_p[index].withdraw == true || (sorted_p[index].wont_withdraw == true))
        {
            index = rand() % (num - last_halfday_num);  //Those who have withdrawn won't update
        }
        updated_p[i] = sorted_p[index];
        //Update new attributes
        int prof_index = rand() % 8;
        updated_p[i].prof = prof_set[prof_index];
        updated_p[i].prof_id = prof_index;
        int risk_index = rand() % 4;
        updated_p[i].risk = risk_set[risk_index];
        updated_p[i].risk_id = risk_index;
        bool letter_set[4] = {false,false,false,true};  //The possibility to get a priority letter is 1/4
        int letter_index = rand() % 4;
        updated_p[i].letter = letter_set[letter_index];
        //Set the halfday when this person is updated
        int updated_half_day = sorted_p[index].show_half_day()[0] + rand() % 20;
        //If time overflows or this person is updated on the same halfday, choose again 
        while(updated_half_day > 167 || updated_half_day == sorted_p[index].show_half_day()[0])
        {
            updated_half_day = sorted_p[index].show_half_day()[0] + rand() % 10 + 1;
        }
        updated_p[i].updated_half_day = updated_half_day;
    }
    //selection sort based on update time
    updated_p = SelectionSort(updated_p,updated_num,1);

    //Every half day, print end of half day
    int half_day_index;
    int p_index = 0;
    int updated_half_day_index = 0;;
    for(half_day_index = 0; half_day_index < 168; half_day_index++)
    {
        //Print people who register on this half day
        while( (p_index < num) && (sorted_p[p_index].show_half_day()[0] == half_day_index) && (half_day_index < 168))
        {
            outfile << this->table_line(sorted_p[p_index]) << "\n";
            p_index++;
        }
        //Print people who update on this half day
        while( (updated_half_day_index < updated_num) && (updated_p[updated_half_day_index].updated_half_day == half_day_index) && (half_day_index < 168))
        {
            outfile << this->table_line(updated_p[updated_half_day_index]) << "\n";
            updated_half_day_index++;
        }        
        outfile << "end of half day" << "\n";
    } 
    this->table_close();
}

// int main()
// {
//     TableWrite t1("./input.csv",200,50,50);
//     t1.table_create();
//     // ifstream infile;
//     // string aline;
//     // int num = 0;
//     // infile.open("./input.csv");
//     // getline(infile,aline);

//     // for(int i = 0; i < 100;i++)
//     // {
//     //     getline(infile,aline);
//     //     if(aline == "end of half day")
//     //         continue;
//     //     person<int>* p = new person<int>(aline);
//     //     person_set.push_back(p);
//     //     num++;
//     // }
//     // for(int i = 0; i < num; i++)
//     // {
//     //     person_set[i]->display_all();
//     // }
// }
