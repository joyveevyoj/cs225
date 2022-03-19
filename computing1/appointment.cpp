#include <stdio.h>
#include <iostream>
#include "head.h"

using namespace std;
appointment::appointment(person a_patient, int date_out_of_queue){
    patient= a_patient;
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
  
}

void appointment:: appointment_withdraw(){

}