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
  hospital(int capacity, double o_time, double c_time);
  bool is_hospital_available();
  double get_time_appointed();
  int daily_appointment_num;
  int id;//the hospital's unique id number, which marks its position in the hospital list.
  vector<appointment*> daily_appointment_list;
  void add_to_applist(appointment* new_app);
private:
  int daily_capacity;
  int daily_remaining_capacity;
  double open_time;
  double close_time;
  double treatment_time;


};



#endif