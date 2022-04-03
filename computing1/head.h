#ifndef head_h
#define head_h

#include<vector>
using namespace std;
// a change

class appointment; 
class hospital;


class person
{public:
person(int n, vector<hospital*> l);
int hosp_num;
vector<hospital*> hospital_ranking_list;//the hospital's  ranking list

};

class appointment
{
public:
  appointment(person* a_patient, int date_out_of_queue);
  //virtual ~appointment();
  void make_appointment();
  void appointment_withdraw();
  void pretty_print();
    
private:
  person patient;
  int hospital_id;
  int date;
  double time;
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
  void printapp(int h_date);
  int daily_remaining_capacity[100];
private:
  int daily_capacity;
  double open_time;
  double close_time;
  double treatment_time;

};

#endif

template <class T> class reportlist
{
  public:
  void push_new(int r_status, person<int>* patient);//push a new person into the report list, and set his status value
  void push_old(int r_status, person<int>* patient);//use a temperary person to update the person already in the report list and set his status value
  void push_appoi(int r_status, appointment*) r_appointment);//put the appointment information into the person stored in the report list and set his status value
  private:
  vector<person<int>*> rl;// a vector list that stores all the information about that person
};