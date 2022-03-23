#include<vector>
#include <stdio.h>
#include <iostream>
using namespace std;
#include "head.h"


hospital::hospital(int capacity, double o_time, double c_time)
:daily_capacity(capacity), open_time(o_time), close_time(c_time){
daily_remaining_capacity=daily_capacity;
treatment_time=(close_time - open_time)/(double)daily_capacity;
daily_appointment_num=0;

}

//this function checks whether the hospital is still available for that day
bool hospital::is_hospital_available(){
if(daily_remaining_capacity>0){return true;}
else{return false;}
}


//this function returns the next available time for the patient, and decrease the remaining capacity
double hospital::get_time_appointed(){
    double time;
    time=open_time+((double)(daily_capacity-daily_remaining_capacity))*treatment_time;
    daily_remaining_capacity--; //decrease the remaining capacity when one time is appointed
    return time;
  }

//this function add the new appointment to the hospital's apppointment list
  void hospital::add_to_applist(appointment* new_app){
daily_appointment_list.push_back(new_app);
daily_appointment_num++;
  }




appointment::appointment(person* a_patient, int date_out_of_queue):patient(*a_patient){
    date= date_out_of_queue+1;//by default the appointment date should be the next day the person is out of queue
    hospital_id=-1;
}
void appointment:: make_appointment(){
int i=0;
while(hospital_id==-1){//keep searching for days until available hospital until one is found
while (i< patient.hosp_num && (*(patient.hospital_ranking_list[i])).is_hospital_available() == false){i++;
    //in one day, keep searching for all hospitals in the person's hospital ranking list, until 
    //one available is found
}
if(i<patient.hosp_num){
    hospital_id=(*(patient.hospital_ranking_list[i])).id;
    time=(*(patient.hospital_ranking_list[i])).get_time_appointed();}
else{date++;}
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
/*
 int main(){
    hospital hospital1(10, 8.0, 16.0);
    hospital hospital2(1, 9.0, 16.0);
    vector<hospital*> hospital_list;
    hospital_list.push_back(&hospital1);
    hospital_list.push_back(&hospital2);//build a hospital list with two hosptals

    vector<hospital*> hospital_rl;
    hospital_rl.push_back(&hospital2);
    hospital_rl.push_back(&hospital1);
 
    person patient1(2,hospital_rl );
    person patient2(2, hospital_rl);//build two patient with the same hospital ranking list.

    appointment appointment1(&patient1, 1);
    appointment appointment2(&patient2, 1);//build two appopintment

    appointment1.make_appointment();
    appointment1.pretty_print();
    appointment2.make_appointment();
    appointment2.pretty_print();




  

  return 0;  
}*/


 