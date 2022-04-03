#include<vector>
#include <stdio.h>
#include <iostream>
using namespace std;
#include "head.h"


hospital::hospital(int capacity, double o_time, double c_time)
:daily_capacity(capacity), open_time(o_time), close_time(c_time){
for (int j=0; j<100; j++){daily_remaining_capacity[j]=daily_capacity;
cout<<"daily:"<< daily_remaining_capacity[j]<<"\n";}
//initalize all the remaining capacity as daily capacity

treatment_time=(close_time - open_time)/(double)daily_capacity;


}

//this function checks whether the hospital is still available for that day
bool hospital::is_hospital_available(int h_date){
if(daily_remaining_capacity[h_date]>0){return true;}
else{return false;}
}


//this function returns the next available time for the patient, and decrease the remaining capacity
double hospital::get_time_appointed(int h_date){
    double time;
    time=open_time+((double)(daily_capacity-daily_remaining_capacity[h_date]))*treatment_time;
    daily_remaining_capacity[h_date]--; //decrease the remaining capacity when one time is appointed
    return time;
  }

//this function add the new appointment to the hospital's apppointment list
  void hospital::add_to_applist(appointment* new_app){
daily_appointment_list.push_back(new_app);

  }

void hospital::printapp(int date){
  
}



appointment::appointment(person* a_patient, int date_out_of_queue):patient(*a_patient){
    date= date_out_of_queue+1;//by default the appointment date should be the next day the person is out of queue
    hospital_id=-1;
}

void appointment:: make_appointment(){
while(hospital_id==-1&&date<100){//keep searching for days until available hospital is found
int i=0;
while (i< patient.hosp_num && (*(patient.hospital_ranking_list[i])).is_hospital_available(date) == false){i++;
    //in one day, keep searching for all hospitals in the person's hospital ranking list, until 
    //one available is found
}
if(i<patient.hosp_num){
    hospital_id=(*(patient.hospital_ranking_list[i])).id;
    time=(*(patient.hospital_ranking_list[i])).get_time_appointed(date);}
else{date++;
}
}
return;
}

void appointment::pretty_print(){
     cout<<"hospital id is: "<<hospital_id<<"\n";
     cout<<"date is: "<<date<<"\n";
     cout<<"time is: "<<time<<"\n";
  
}

void appointment:: appointment_withdraw(){

}



person::person(int n, vector<hospital*> l)
{hosp_num=n; 
 hospital_ranking_list=l;}

 
