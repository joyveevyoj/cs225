#include <stdio.h>
#include <iostream>
#include<vector>
#include "head.h"
#include "fstream"
#include "cassert"
#include <string>

using namespace std;

TableWrite a_table;
int person_num;
a_table.table_create("input.csv", person_num);//create an input file

int main(){
  int month=0;
  int week=0;
  int day=0;
  int halfday=0; 
  double hour=0;//initialize all the parameters that represents time as zero

  fifo<person<int>*>* localqueue_0 = new fifo<person<int>*>* localqueue_0;
  fifo<person<int>*>* localqueue_1 = new fifo<person<int>*>* localqueue_0;
  fifo<person<int>*>* localqueue_2 = new fifo<person<int>*>* localqueue_0;//set up three empty local queues;
  vector<fifo<person<int>*>*> localqueues;
  localqueues.push_back(localqueue_0);
  localqueues.push_back(localqueue_1);
  localqueues.push_back(localqueue_2);
  

  hospital* hospital_0 = new hospital(0, 10, 8.0, 16.0);//set up first hospital with 10 daily capacity, open at 8am, close at 16pm
  hospital* hospital_1 = new hospital(1, 5, 9, 16.5);////set up second hospital with 5 daily capacity, open at 9am, close at 16:30pm
  hospital* hospital_2 = new hospital(2, 15, 8.0, 17.0 );//set up third hospital with 15 daily capacity, open at 8am, close at 17pm
  vector<hospital*> hospital_list;
  hospital_list.push_back(hospital_0);
  hospital_list.push_back(hospital_1);
  hospital_list.push_back(hospital_2);//put all three hospital's pointer in a hospital list.

  Fb_heap<int> fibo_h;//set up an empty fibonacciheap

  reportlist a_reportlist;//set up a report list that stores all the patient's information, and is used to generate report
 
  ifstream infile;
  infile.open("input.csv");   //connect  stream with inputfile  
	assert(infile.is_open());   //if fail, output error and exit the programm
  string aline;
  string endmark="end of half day";//the string that marks end of halfday input

if(getline(infile, aline)==false){return 0;}//first skip the first line, which is the table head
while (getline(infile, aline)){
  
if(aline.find(endmark)!=0  ){//if a endmark string is not found at the start of aline, the line contain register information
  person<int>* a_patient = new person<int>(aline);//create a tempary person from the input file line
  int local_index=a_patient.local_id;
  (*(localqueues[local_index])).pushback(a_patient);//push that person's adress into the local queue
}
  

else{//if a endmark string is found at the start of aline, the program has already finished reading halfday information 
  halfday++;
  hour+=12.0;
  for(int k=0; k<a_reportlist.size(); k++){
    if(a_reportlist[k]->p_appoint->is_appointment_passed(hour)==true){
     a_reportlist[k]->status=3; //check all people in report list, if that person's appointment has passed, mark it as status 3 
    }
  }

  for(int i=0; i<3; i++){
    while(  (*localqueues[i]).isempty() == false ){//pop all the elements from local queue twice daily
    person<int>* temp_person = (*localqueues[i]).popfront();
    if(a_reportlist.exist(temp_person)==NULL){//after checking id in report list, if that person is not already in the report list
    fibo_h.insert(temp_person);//insert that person into the fibonacci heap 
    a_reportlist.push_new(1,temp_person);//push that person to reportlist and change status to be in the fibonacci heap
    }
    else{//if that person is already in the report list
   
    old_person_ptr = a_reportlist.exist(temp_person);
    if(old_person_ptr->status == 1){//if that person's status is 1, it means it is still in the heap
      if(temp_person->is_update(old_person_ptr)==true){// if this is an update 
      a_reportlist.push_old(1,temp_person);//update the information in the reportlist and change status to be in the fibonacci heap
      fibo_h.update(temp_person, old_person_ptr );//update the fibonacci heap with the updated old person accordingly
     }
      if(temp_person->is_newwithdraw(old_person_ptr)==true){
      fibo_h.remove(old_person_ptr);//if that person withdraws, delete him from fibonacci heap
      a_reportlist.push_old(5,temp_person);//update the information in the report list and change status to be after treated
      }
    }
    if(old_person_ptr->status == 2){
      if(temp_person->is_update(old_person_ptr)==true){
      a_reportlist.push_old(2,temp_person);
      }
      if(temp_person->is_newwithdraw(old_person_ptr)==true){
        a_reportlist.push_old(5, temp_person);
      }
      
    }
    if(old_person_ptr->status == 3){
      if(temp_person->is_update(old_person_ptr)==true){
      a_reportlist.push_old(3,temp_person);
      }
      if(temp_person->is_newwithdraw(old_person_ptr)==true){
        //mark that this person do not go to appointment, which is status 4
      a_reportlist.push_old(4,temp_person);


        }
       }
    if(old_person_ptr->status == 4 || old_person_ptr->status == 5){
       old_person_ptr->punish();//需要写的函数 use a person's member fuction to change the register time, and set new key value
       fibo_h.insert(temp_person);//insert that person into the fibonacci heap 
       a_reportlist.push_new(1,temp_person);//push that person to reportlist and change status to be in the fibonacci heap
     
     }  
      }
     }
    }
   }
  }
 }
  if(halfday%2 == 0){
  day++;
  for(int j=0; j<fib_h.getnum()/2; j++){
  // extract the half number of people with min key from the fibonacci heap everday
  person<int>* temp_person = fib_h.delete_min();
  appointment* a_appointment= new appointment(temp_person, day, hospital_list);
  a_appointment->make_appointment();
  int hos_id=a_appointment->hospital_id;
  hospital_list[hos_id]->add_to_applist(a_appointment);//make appointment and add them to the fibonacci heap
  a_reportlist.push_appoi(2,a_appointment);//update that person's status and set his appointment
  }
  }
  if(halfday%14==0){
   week++;
   Weekyreport tem_weeklyreport(a_reportlist.rl, hour);
   tem_weeklyreport.save();//generate the three weekly report 
   tem_weeklyreport.sort();//and sort them according to the user's preference
  }
  if(halfday%56==0){
  month++;
  Monthlyreport tem_monthlyreport(a_reportlist.rl, hour);
  tem_monthlyreport.save();//generate the monthly report
  }//increment the time relavent counters and output weekly and monthly report

  infile.close();
  return 0;

}
 
 



  
                                        





