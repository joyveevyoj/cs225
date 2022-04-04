#include <stdio.h>
#include <iostream>
#include<vector>
#include "head.h"
#include "fstream"
#include "cassert"
#include <string>

using namespace std;

int main(){
  int month=0;
  int week=0;
  int day=0;
  int halfday=0; 
  double hour=0;//initialize all the parameters that represents time as zero


  
  fifo<person<int>*> localqueue_0;
  fifo<person<int>*> localqueue_1;
  fifo<person<int>*> localqueue_2;//set up three empty local queues;
  vector<fifo<person<int>*>*> localqueues;
  localqueues.pushback(&localqueue_0);
  localqueues.pushback(&localqueue_1);
  localqueues.pushback(&localqueue_2);
  

  hospital hospital_0(0, 10, 8.0, 16.0);//set up first hospital with 10 daily capacity, open at 8am, close at 16pm
  hospital hospital_1(1, 5, 9, 16.5);////set up second hospital with 5 daily capacity, open at 9am, close at 16:30pm
  hospital hospital_2(2, 15, 8.0, 17.0 );//set up third hospital with 15 daily capacity, open at 8am, close at 17pm
  vector<hospital*> hospital_list;
  hospital_list.pushback(&hospital_0);
  hospital_list.pushback(&hospital_1);
  hospital_list.pushback(&hospital_2);//put all three hospital's pointer in a hospital list.

  Fb_heap<int> fibo_h;//set up an empty fibonacciheap

  reportlist a_reportlist;//set up a report list that stores all the patient's information, and is used to generate report
 
  ifstream infile;
  infile.open("input.txt");   //将文件流对象与文件连接起来 
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
  string aline;
  string endmark="end of half day";//the string that marks end of halfday input


while (getline(infile, aline)){
  
if(aline.find(endmark)!=0){//if a endmark string is not found at the start of aline, the line contain register information
  person<int>* a_patient = new person<int>(aline);//create a tempary person from the input file line
  int local_index=a_patient.local_id;
  (*(localqueues[local_index])).pushback(a_patient);//push that person's adress into the local queue
}
  

else{//if a endmark string is found at the start of aline, the program has already finished reading halfday information 
  halfday++;
  hour+=12.0;
  for(int i=0; i<3; i++){
    while(  (*localqueues[i]).isempty() == false ){//pop all the elements from local queue twice daily
    person<int>* temp_person = (*localqueues[i]).popfront();
    if(a_reportlist.exist(temp_person)==NULL){//after checking id in report list, if that person in not already in the report list
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
      fibo_h.delete(old_person_ptr);//if that person withdraws, delete him from fibonacci heap
      a_reportlist.push_old(3,temp_person);//update the information in the report list and change status to be after treated
      }
    }
    if(old_person_ptr->status == 2){
      if(temp_person->is_update(old_person_ptr)==true){
      a_reportlist.push_old(2, old_person_ptr);
      }
      if(temp_person->is_newwithdraw(old_person_ptr)==true){
        //kick that person out of appointment list
      }
      
    }
    if(old_person_ptr->status == 3){
      if(temp_person->is_update(old_person_ptr)==true){
      a_reportlist.push_old(3, old_person_ptr);
      }
      if(temp_person->is_newwithdraw(old_person_ptr)==true){
        //mark that this person do not go to appointment


      }
      if(temp_person->is_register_after_withdraw()==true){
      //use a person's member fuction to change the register time, and set new key value


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
  appointment* a_appointment=new appointment(fib_h.delete_min(),day);
  a_appointment->make_appointment();
  int hos_id=a_appointment->hospital_id;
  hospital_list[hos_id]->add_to_applist(a_appointment);//make appointment and add them to the fibonacci heap
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
 
 



  
                                        





