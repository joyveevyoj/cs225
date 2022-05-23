
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "cassert"
#include <string>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include "head.h"
using namespace std;

hospital::hospital(int idnum, int capacity, double o_time, double c_time)
    : id(idnum), daily_capacity(capacity), open_time(o_time), close_time(c_time)
{
  for (int j = 0; j < 200; j++)
  {
    daily_remaining_capacity[j] = daily_capacity;
  }
  // initalize all the remaining capacity as daily capacity

  treatment_time = (close_time - open_time) / (double)daily_capacity;
}

// this function checks whether the hospital is still available for that day
bool hospital::is_hospital_available(int h_date)
{
  if (daily_remaining_capacity[h_date] > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// this function returns the next available time for the patient, and decrease the remaining capacity
double hospital::get_time_appointed(int h_date)
{
  double atime;
  atime = open_time + ((double)(daily_capacity - daily_remaining_capacity[h_date])) * treatment_time;
  daily_remaining_capacity[h_date]--; // decrease the remaining capacity when one time is appointed
  return atime;
}

// this function add the new appointment to the hospital's apppointment list
void hospital::add_to_applist(appointment *new_app)
{
  daily_appointment_list->insert_I(new_app);
  return;
}

void hospital::printapp()
{
  /*for (int i=0; i<daily_appointment_list.size();i++){
  daily_appointment_list[i]->pretty_print();
  }*/
  for (block<appointment, long long, int> *ptr = daily_appointment_list->first_block; ptr != NULL; ptr = ptr->r_sibling)
  {
    for (int i = 0; i < ptr->mainblock.size(); i++)
    {
      if (ptr->mainblock[i] != ptr->tombptr)
      {
        ptr->mainblock[i]->pretty_print();
      }
    }
    for (int i = 0; i < ptr->overflowblock.size(); i++)
    {
      ptr->overflowblock[i]->pretty_print();
    }
  }
  return;
}

void hospital::withdraw_app(appointment *w_app)
{
  /*for (vector<appointment*>::iterator iter=daily_appointment_list.begin();iter!=daily_appointment_list.end();iter++){
    if(*iter==w_app){
      daily_appointment_list.erase(iter);
      break;
    }
  }//find that appointment and delete it from the appointment list
  */
  daily_appointment_list->bp_delete_I(w_app->pri_key);
  w_app->patient->set_appointment(NULL); // set that person's appointment pointer to be null
}

appointment::appointment(person<int> *a_patient, int date_out_of_queue, vector<hospital *> a_hospital_list)
    : patient(a_patient), hospitals(a_hospital_list)
{
  date = date_out_of_queue + 1; // by default the appointment date should be the next day the person is out of queue
  hospital_id = -1;
  pri_key = a_patient->pri_key;
  secondary_key = -1;
  dataptr.first = NULL;
  dataptr.first = NULL;
  status = 2;
  treatment = a_patient->treatment;
}

void appointment::make_appointment()
{
  while (hospital_id == -1 && date < 200)
  { // keep searching for days until available hospital is found
    int i = 0;
    while (i < patient->hosp_num && (hospitals[i])->is_hospital_available(date) == false)
    {
      i++; 
           // in one day, keep searching for all hospitals in the person's hospital ranking list, until
      // one available is found
    }

    if (i < patient->hosp_num)
    {
      hospital_id = patient->hospital_id[i];
      the_time = (hospitals[hospital_id])->get_time_appointed(date);
    }
    else
    {
      date++;
    }
  }
  secondary_key = date;
  return;
}

void appointment::pretty_print()
{
  cout << "Patient id is:" << patient->show_id() << "\n";
  cout << "Hospital id is: " << hospital_id << "\n";
  cout << "Date is: " << date << "\n";
  cout << "Time is: " << the_time << "\n";
}

bool appointment::is_appointment_passed(double current_hour)
{
  double app_hour = (double)((date - 1) * 24) + the_time;
  if (app_hour <= current_hour)
  {
    return true;
  }
  else
  {
    return false;
  }
}