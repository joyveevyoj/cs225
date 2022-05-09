#include "head.h"
#include <istream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;


double Weeklyreport::waitingtime(double a, double b){
    return b - a;
}

int Weeklyreport::getlength(vector<person<int>*> report_list){
    int length=report_list.size();
    return length;
}



void Weeklyreport::Save(int week,string file_head)
{
    vector<string*> file_matrix;//create a vector to store the file name string
    for(int i = 0; i < 3;i++)
    {
        string* file_vector = new string[12];
        for(int j = 0 ; j < 12; j++)
        {
            file_vector[j] = file_head + to_string(j+1) + "File" + to_string(i+1) + ".csv";
        }
        file_matrix.push_back(file_vector);
    }

    string* s1 = file_matrix[0];
    string* s2 = file_matrix[1];
    string* s3 = file_matrix[2];
    vector<int*> head_matrix;

    for(int i = 0; i < 3;i++)
    {
        int* head_vector = new int[12];
        for(int j = 0; j < 12;j++)
        {
            head_vector[j] = 0;
        }
        head_matrix.push_back(head_vector);
    }//generate the fime name
	
	for (int i = 0; i < getlength(rl); i++)
	{   
        if(rl[i]->status==1 ){//status1 means the first type people without appointments
            ofstream ofs;
	        ofs.open(s1[week-1], ios::app); //open the file
            if(head_matrix[0][week-1] == 0)
            {
                //Print table head
                ofs << "Name,Profession,Age,Risk,Waiting Time" << endl;//create the head of excel
                head_matrix[0][week-1] = 1;
            }
            
		    ofs << rl[i]->name << ","//write the file using these information
			<< rl[i]->prof << ","
            << rl[i]->age << ","
            << rl[i]->risk<< ","
			<< waitingtime(rl[i]->Time[0], current_time) <<endl;//curret time - register time
            ofs.close();
            }
    
        else if(rl[i]->status==2 )//status2 means the second type people with a set appointment
            {
            ofstream ofs;
	        ofs.open(s2[week-1], ios::app); // open the file

            if(head_matrix[1][week-1] == 0)
            {
                //Print table head
                ofs << "Name,Profession,Age,Risk,Waiting Time" << endl;//create the head of excel
                head_matrix[1][week-1] = 1;
            }

		    ofs << rl[i]->name << ","//write the file
			<< rl[i]->prof << ","
            << rl[i]->age << ","
            << rl[i]->risk << ","
			<< waitingtime(rl[i]->Time[0], current_time) <<endl;//curret time - register time
            ofs.close();
            }
    
        else if(rl[i]->status==3 && rl[i]->Time[2]>= (week-1)*7*24){//status3 means the 3rd type people who have been treated
            ofstream ofs;
	        ofs.open(s3[week-1], ios::app); // open the file
            if(head_matrix[2][week-1] == 0)
            {
                //Print table head
                ofs << "Name,Profession,Age,Risk,Waiting Time" << endl;//create the head of excel
                head_matrix[2][week-1] = 1;
            }

		    ofs << rl[i]->name << ","//write data to the file
			<< rl[i]->prof << ","
            << rl[i]->age << ","
            << rl[i]->risk << ","
			<< waitingtime(rl[i]->Time[0], rl[i]->Time[2]) << endl;//treatment time - register time
            ofs.close();
            }
    }        
	

	//关闭文件
	
    
}

void Weeklyreport::Sort(int week)
{
    
    cout << "please select a sort method" << endl;
    cout << "put in '1' if sort by name" << endl;
	cout << "put in '2' if sort by profession" << endl;
    cout << "put in '3' if sort by risk" << endl;

	int select = 0;
	cin >> select;
    while(cin.fail()){
        cout<<"Wrong, you have inputed a wrong type data, please input one of '1', '2', '3'\n"<<endl;
        cin.clear();
        while(cin.get() != '\n'){
            continue;    
        }     
        cout<<"please input again"<<endl;
        cin>>select;
    }//judge if the input data ia int number
    
		for (int i = 0; i < getlength(rl); i++)
		{
			int Min = i; //Declare the minimum subscript Min
			for (int j = i + 1; j < getlength(rl); j++)
			{
				if (select == 1) 
				{
					if (rl[Min]->name > rl[j]->name)//compare name
					{
						Min = j;
					}
            	}
                
				else if (select == 2) 
				{
					if (rl[Min]->prof > rl[j]->prof)//compare profession
					{
						Min = j;
					}
            	} 
            
				else if (select == 3) 
				{
					if (rl[Min]->risk > rl[j]->risk)//compare risk
					{
						Min = j;
					}
            	}
                
            }
            if (i != Min)
		    {
			    swap(rl[i], rl[Min]);
			}

			//Determine if the initial assumed minimum is the calculated minimum, and if not, exchange data
			
			
		}

		cout << "Successful!" << endl;
		this->Save(week,"SortedWeekly"); //Save the sorted results to a file
			
}

int Monthlyreport::getlength(vector<person<int>*> report_list){
    int length=report_list.size();
        
    return length;
}//all people who have registerred

int Monthlyreport::getlength_m(vector<person<int>*> report_list, int month){
    int length=0;
    for (int i = 0; i<report_list.size();i++){
        if (rl[i]->Time[0]>= (month-1)*24*28){
        length++;
        }
    }
    return length;
}//the number of people who have registerred each month 

double Monthlyreport::average_time(vector<person<int>*> report_list,int month){
    int treatment_number = 0;
    int sum = 0;
    for (int i = 0; i < getlength(rl); i++){
        
        if (rl[i]->status==3 && rl[i]->Time[2]>= (month-1)*24*28){
            double waiting_time = rl[i]->Time[2] - rl[i]->Time[0];
            sum += waiting_time;  
            treatment_number++;   
        }
    }
    double mean = sum / treatment_number;
    return mean;
}//the average waiting time of treated people
    
int Monthlyreport::appointment_num(vector<person<int>*> report_list,int month){    
    int appointment_number=0;
    for (int i = 0; i < getlength(rl); i++){
        
        if ((rl[i]->status==2 || rl[i]->status==3)&&rl[i]->Time[1]>= (month-1)*24*28){
            appointment_number++;
        }
    }
    return appointment_number;
}//the number of people who have appointment 

int Monthlyreport::withdraw_num(vector<person<int>*> report_list,int month){    
    int withdraw_number = 0;
    for (int i = 0; i < getlength(rl); i++){
        
        if (((rl[i]->status==4) || (rl[i]->status==5))&& (rl[i]->Time[3]>=(month-1)*28*24) ){
            withdraw_number++;
        }
    }
    return withdraw_number;
}//the number of people who have withdraw 

int Monthlyreport::treatment_num(vector<person<int>*> report_list,int month){    
    int treatment_number = 0;
    for (int i = 0; i < getlength(rl); i++){
        
        if (rl[i]->status==3&&rl[i]->Time[2]>=(month-1)*24*28){
            treatment_number++;
        }
    }
    return treatment_number;
}//the number of people who have been treated

int Monthlyreport::register_waiting_num(int id,int month){//register_number-appoint_number
    int register_waiting_number = 0;
    for (int i = 0; i < getlength(rl); i++){
        
        if (rl[i]->local_id==id && rl[i]->status!=3 && rl[i]->status!=4 && rl[i]->status!=5 &&rl[i]->Time[0]>=(month-1)*24*28){
            register_waiting_number++;
        }
    }
    return register_waiting_number;
}////the number of register-waiting people in each local office

int Monthlyreport::intotal_waiting_num(int a, int b, int c){//register_number-treatment_number
    int num=a+b+c;
    return num;
}//the total number of waiting people

void Monthlyreport::Save(int month)
{
    string monthly[]={"monthly1_report.txt","monthly2_report.txt","monthly3_report.txt"};
    string title[] = {"January Report","February Report","March Report"};
	//write data to files
            ofstream ofs;
	        ofs.open(monthly[month-1], ios::app); 
            ofs << "---" << title[month - 1] << "---" << endl;
			ofs << "Number of people who registered: " << getlength_m(rl, month) << "\n"
            << "Number of people in local registry office 0 who are waiting: " << register_waiting_num(0, month) << "\n"
            << "Number of people in local registry office 1 who are waiting: " << register_waiting_num(1, month) << "\n"
            << "Number of people in local registry office 2 who are waiting: " << register_waiting_num(2, month) << "\n"
            << "Total number of people who are waiting: " << intotal_waiting_num(register_waiting_num(0, month),register_waiting_num(1, month),register_waiting_num(2, month)) << "\n"
            << "Total number of people who have been treated: " << treatment_num(rl,month) << "\n"
            << "The average waiting time: " << average_time(rl,month) << "\n"
			<< "Number of people who withdrawed: " << withdraw_num(rl,month) << endl;
            ofs.close();//close the file
     
}
