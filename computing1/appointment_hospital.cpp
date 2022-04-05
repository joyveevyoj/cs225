#include<vector>
#include <stdio.h>
#include <iostream>
using namespace std;
#include "head.h"
#include <string>
#include <cstring>


hospital::hospital(int idnum, int capacity, double o_time, double c_time)
:id(idnum), daily_capacity(capacity), open_time(o_time), close_time(c_time){
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
    double atime;
    atime=open_time+((double)(daily_capacity-daily_remaining_capacity[h_date]))*treatment_time;
    daily_remaining_capacity[h_date]--; //decrease the remaining capacity when one time is appointed
    return atime;
  }

//this function add the new appointment to the hospital's apppointment list
  void hospital::add_to_applist(appointment* new_app){
daily_appointment_list.push_back(new_app);
return;
  }

void hospital::printapp(){
  for int(i=0; i<daily_appointment_listsize();i++){
  daily_appointment_list[i]->pretty_print();
  }
  return;
}
void hospital::withdraw_app(appointment* w_app){
for (vector<appointment*>::iterator iter=daily_appointment_list.begin();iter!=daily_appointment_list.end();iter++){
  if(*iter==w_app){
    daily_appointment_list.erase(iter);
    break;
  }
}//find that appointment and delete it from the appointment list
w_app->patient->set_appointment(NULL);//set that person's appointment pointer to be null

}



appointment::appointment(person<int>* a_patient, int date_out_of_queue)
:patient(a_patient):hospitals(a_hospital_list){
    date= date_out_of_queue+1;//by default the appointment date should be the next day the person is out of queue
    hospital_id=-1;
}

void appointment:: make_appointment(){
while(hospital_id==-1&&date<100){//keep searching for days until available hospital is found
int i=0;
while (i< patient->hosp_num && (hospitals[hospital_id])->is_hospital_available(date) == false){i++;
    //in one day, keep searching for all hospitals in the person's hospital ranking list, until 
    //one available is found
}
if(i<patient->hosp_num){
    hospital_id=patient->hospital_ranking_list[i];
    the_time=(hospitals[hospital_id])->get_time_appointed(date);}
else{date++;
}
}
return;
}

void appointment::pretty_print(){
     cout<<"patient id is:"<<patient->show_id()<<"\n";
     cout<<"hospital id is: "<<hospital_id<<"\n";
     cout<<"date is: "<<date<<"\n";
     cout<<"time is: "<<the_time<<"\n";
  
}



bool appointment::is_appointment_passed(double current_hour){
double app_hour = (double)(date*24)+time;
if(app_hour<=current_hour){return true;}
else{return false;}

}


 







 
