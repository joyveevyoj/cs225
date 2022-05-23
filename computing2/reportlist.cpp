#include <stdio.h>
#include <iostream>
#include<vector>
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

void reportlist::push_new(int r_status, person<int>* patient){
    rl->insert_I(patient) ;
    patient->status=r_status ; // push new patient into list and update status
    patient->secondary_key=r_status;
    rl->b_update_seckey(patient);
    
}

void reportlist::push_old(int r_status, person<int>* patient){
    patient->status=r_status; // update status
    patient->secondary_key=r_status;
    person<int>* find=exist(patient);
    if(find!=NULL){*find=*patient;}
    rl->b_update_seckey(patient);
    
}

void reportlist::push_appoi(int r_status, appointment* r_appointment){
    person<int>*  apperson= exist(r_appointment->patient) ;
    if(apperson==NULL){
        return ; //if not find this person
    }
    apperson->set_appointment(r_appointment); 
    apperson->status= r_status;   // push the appointment and update status
    apperson->secondary_key=r_status;
    rl->b_update_seckey(apperson);
}

person<int>* reportlist::exist(person<int>* patient){
     //if not find , return null
   return rl->bp_retrieve_I(patient->pri_key);
}