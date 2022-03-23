#include <stdio.h>
#include <iostream>
#include<vector>
#include "head.h"


using namespace std;
int main(){
    hospital hospital1(10, 8.0, 16.0);
    hospital1.id=0;
    hospital hospital2(1, 9.0, 16.0);
    hospital2.id=1;
    vector<hospital*> hospital_list;
    hospital_list.push_back(&hospital1);
    hospital_list.push_back(&hospital2);//build a hospital list with two hosptals

    vector<hospital*> hospital_rl;
    hospital_rl.push_back(&hospital2);
    hospital_rl.push_back(&hospital1);
 
    person patient1(2,hospital_rl );
    person patient2(2, hospital_rl);
    person patient3(2, hospital_rl);

    appointment appointment1(&patient1, 1);
    appointment appointment2(&patient2, 1);
    appointment appointment3(&patient3, 1);

    appointment1.make_appointment();
    appointment1.pretty_print();
    appointment2.make_appointment();
    appointment2.pretty_print();
    appointment3.make_appointment();
    appointment3.pretty_print();




  

  return 0;  
}
