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
using namespace std;

void reportlist::push_new(int r_status, person<int>* patient){
    rl.push_back(patient) ;
    patient->status=r_status ;
}

void reportlist::push_old(int r_status, person<int>* patient){
    int rlsize= rl.size() ;
    patient->status=r_status;
    for(int i=0 ; i<rlsize;i++){
        if(rl[i]->show_id()==patient->show_id()){
            rl[i]=patient;   //if find this patient in the reportlist, then update it
    
        }
    }
}

void reportlist::push_appoi(int r_status, appointment* r_appointment){
    person<int>*  apperson= exist(r_appointment->patient) ;
    if(apperson==NULL){
        return ; //if not find this person
    }
    apperson->set_appointment(r_appointment); //需要加函数
    apperson->status= r_status;
}

person<int>* reportlist::exist(person<int>* patient){
    int rlsize= rl.size() ;
    for(int i=0 ; i<rlsize;i++){
        if(rl[i]->show_id()==patient->show_id()){
            return rl[i] ;   //if find this patient in the reportlist, then return person's addr
        }
    }
    return NULL ; //if not find , return null

}

