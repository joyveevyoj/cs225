#include <stdio.h>
#include <iostream>
#include "head.h"

using namespace std;
hospital::hospital(int capacity, int o_time, int c_time)
:daily_capacity(capacity), open_time(o_time), close_time(c_time){
daily_remaining_capacity=daily_capacity;
treatment_time=(close_time - open_time)/daily_capacity;
daily_appointment_num=0;

}

//this function checks whether the hospital is still available for that day
bool hospital::is_hospital_available(){
if(daily_remaining_capacity>0){return true;}
else{return false;}
}


//this function returns the next available time for the patient, and decrease the remaining capacity
int hospital::get_time_appointed(){
    int time;
    time=open_time+(daily_capacity-daily_remaining_capacity);
    daily_remaining_capacity--; //decrease the remaining capacity when one time is appointed
    return time;
  }
//this function add the new appointment to the hospital's apppointment list
  void hospital::add_to_applist(appointment* new_app){
daily_appointment_list.push_back(new_app);
daily_appointment_num++;
  }
