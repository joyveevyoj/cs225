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

#include "appointment_hospital.cpp"
#include "Fb_heap.cpp"
#include "local_queue.cpp"
#include "report.cpp"
#include "reportlist.cpp"
#include "Bptree.cpp"
#include"btree.cpp"
#include"maindata.cpp"
#include"index_structure.cpp"


vector<int> GenerateDiffNumber(int min,int max,int num)
{
    int rnd;
    vector<int> diff;
    vector<int> tmp;    //Store remaining number
    //Initialization
    for(int i = min;i < max+1 ; i++ )
    {
        tmp.push_back(i);
    }
    srand((unsigned)time(0)); //initialize seed
    for(int i = 0 ; i < num ; i++)
    {
        do{
            rnd = min + rand() % (max - min + 1);
        }while(tmp.at(rnd-min)==-1);
        diff.push_back(rnd);
        tmp.at(rnd-min) = -1;
    }
    return diff;
}

vector<int> Generate_secon_key(int min, int max, int num){
    vector<int> sec_key;
    int ran;
    srand((unsigned)time(0));
    for(int i=0; i<num;i++){
        ran=min+rand()%(max-min+1);
        sec_key.push_back(ran);
    }
    return sec_key;
}
int main(){
    vector<int> v=GenerateDiffNumber(0,2000,200);
    vector<int>v2=Generate_secon_key(1,50,200);
    cout<<"Now,we are going to show our index structure operations"<<endl ;
    cout<<"First,We randomly generate 100 relation tuples with primary key ranged in [0,2000] and secondary key ranged in [1,50] "<<endl;
    cout<<"Then, we insert these 100 tuples into our index structure, and print it for you" <<endl;
    cout<<"You should notice that mainblock size  is 15 and overflow block size is 4 "<<endl;
    index_structure<relation, int, int> my_index_structure(15,4);
    for(int i=0; i< 100;i++)
    {
        relation* tem_relation=new relation(v[i],v2[i]);
        my_index_structure.insert_I(tem_relation);
    }
    my_index_structure.prettyprint();

    cout << "--------Single Query Demonstration--------" << endl;
    cout<<"Then we start to present retrieve operations."<<endl ;
    cout<<"In this test, all the primary keys are ranged in (0,2000)."<<endl;
    cout<<"First we retrieve one particular primary key."<<endl;
    int rtype;
    do{
        cout<<"Please type one integer i ranged in (0,2000),so that we can retrieve this key "<<endl;
        while (!(cin >> rtype)){
            cout << "Please enter integer:"<<endl;
            cin.clear();  
            while (cin.get() != '\n') 
                 continue;
        }
    }while(rtype>=2000 || rtype<=0 );
    cout<<"Below are the retrieved tuples:"<<endl;
    printquery( my_index_structure.query(rtype));


    cout << "--------Range Query Demonstration--------" << endl;
    cout<<"Then we retrieve primary key ranged in (a,b) ."<<endl;
    int rtype1,rtype2;
    do
    {
    
        cout<<"Please enter two integers a and b, we will retrieve tuples with primary key between a and b."<<endl;
        cout<<"Ensure that the first number you entered is smaller than the second one, and they are in range (0,2000)."<<endl;
        cout<<"Please enter the first integer:"<<endl;
        while (!(cin >> rtype1)){
            cout << "Please enter integer:"<<endl;
            cin.clear();  
            while (cin.get() != '\n') 
                 continue;
        }

        cout<<"Please enter the second integer:"<<endl;
        while (!(cin >> rtype2)){
            cout << "Please enter integer:";
            cin.clear();  
            while (cin.get() != '\n')  
                 continue;
        }


    }while (!(rtype1>0 &&rtype1<2000&&rtype2>0&&rtype2<2000&&rtype1<rtype2));
    cout<<"Below are the retrieved tuples: "<<endl;
    printquery(my_index_structure.query(rtype1,rtype2));




    cout << "-------- Delete Primary Key Demonstration--------" << endl;
    int type1,type2;
    cout<<"Then we are going to show you delete operation, the first one is delete from B+tree according to the primary key."<<endl;
    do
    {
        cout<<"Please enter two integers a and b, we will delete tuples from index ath to index bth."<<endl;
        cout<<"Ensure that the first number you entered is smaller than the second one, and they are in range [0,99]."<<endl;
        cout<<"Please enter the first integer:"<<endl;
        while (!(cin >> type1)){
            cout << "Please enter integer:"<<endl;
            cin.clear();  
            while (cin.get() != '\n')  
                 continue;
        }

        cout<<"Please enter the second integer:"<<endl;
        while (!(cin >> type2)){
            cout << "Please enter integer:";
            cin.clear();  
            while (cin.get() != '\n')  
                 continue;
        }


    }while (!(type1>=0 &&type1<=99&&type2>=0&&type2<=99&&type1<=type2));


    for(int i=type1;i<=type2;i++){
        my_index_structure.bp_delete_I(v[i]);
    }
    cout<<"Now, some relation tuples have been deleted.\nBelow are the current index structure: "<<endl;
    my_index_structure.prettyprint();

    cout << "--------Delete Secondary Key Demonstration--------" << endl;
    cout<<"Now, we are going to show another delete way, delete from B-tree."<<endl;
    cout<<"It will delete all tuples with same given secondary key"<<endl;
    int m;
    do{
        cout<<"Please type one integer i ranged in [0,99],so that we can delete ith secondary key. "<<endl;
        cout<<"Note that index should not be the one which has been deleted previously "<<endl;
        while (!(cin >> m)){
            cout << "Please enter integer:"<<endl;
            cin.clear();  
            while (cin.get() != '\n')  
                 continue;
        }
    }while(m>99 || m<0 || (m>=type1&&m<=type2));
    cout<<"ith secondary key is "<<v2[m]<<endl;
    my_index_structure.b_delete_I(v2[m]);
    cout<<"Now, all tuples with this secondary key have been deleted.Below are current structure. "<<endl;
    my_index_structure.prettyprint();

    cout<<"Successful! we have done all the test about index structure."<<endl;


}