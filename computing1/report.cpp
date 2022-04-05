#include "report.h"
#include "local_queue.cpp"
#include "local_queue.h"

using namespace std;


double Weeklyreport::waitingtime1(double a, double b){
    return b - a;
}

double Weeklyreport::waitingtime2(double a, double b){
    return b - a;
}

double Weeklyreport::waitingtime3(double a, double b){
    return b - a;
}

int Weeklyreport::getlength(vector<person<int>*> report_list){
    int length=report_list.capacity();
    return length;
}

void Weeklyreport::Save()
{
	string s1 = "WeeklyFile1.txt";
    string s2 = "WeeklyFile2.txt";
    string s3 = "WeeklyFile3.txt";
	//将每个人数据写入到文件中
	for (int i = 0; i < getlength(rl); i++)
	{   
        if(rl[i]->status==1){
            ofstream ofs;
	        ofs.open(s1, ios::out); // 用输出的方式打开文件  -- 写文件
		    ofs << rl[i]->name << " "
			<< rl[i]->prof << " "
            << rl[i]->age << " "
            << rl[i]->risk<< " "
			<< waitingtime1(rl[i]->Time[0], current_time) << endl;
            ofs.close();
            }
        else if(rl[i]->status==2)
            {
            ofstream ofs;
	        ofs.open(s2, ios::out); // 用输出的方式打开文件  -- 写文件
		    ofs << rl[i]->name << " "
			<< rl[i]->prof << " "
            << rl[i]->age << " "
            << rl[i]->risk << " "
			<< waitingtime2(rl[i]->Time[1], rl[i]->Time[0]) << endl;
            ofs.close();
            }
        else if(rl[i]->status==3){
            ofstream ofs;
	        ofs.open(s3, ios::out); // 用输出的方式打开文件  -- 写文件
		    ofs << rl[i]->name << " "
			<< rl[i]->prof << " "
            << rl[i]->age << " "
            << rl[i]->risk << " "
			<< waitingtime3(rl[i]->Time[1], rl[i]->Time[2]) << endl;
            ofs.close();
            }
	}

	//关闭文件
	
    
}

void Weeklyreport::Sort()
{
    	cout << "请选择排序方式：" << endl;
		cout << "1、按name进行排序" << endl;
		cout << "2、按professioin进行排序" << endl;
        cout << "3、按risk进行排序" << endl;

		int select = 0;
		cin >> select;
		for (int i = 0; i < getlength(rl); i++)
		{
			int Min = i; //声明最小值 或 最大值下标
			for (int j = i + 1; j < getlength(rl); j++)
			{
				if (select == 1) //升序
				{
					if (rl[Min]->name > rl[j]->name)
					{
						Min = j;
					}
            	}
                
				else if (select == 2) //升序
				{
					if (rl[Min]->prof > rl[j]->prof)
					{
						Min = j;
					}
            	} 
            
				else if (select == 3) //升序
				{
					if (rl[Min]->risk > rl[j]->risk)
					{
						Min = j;
					}
            	}
            }

			//判断一开始认定 最小值或最大值 是不是 计算的最小值或最大值，如果不是 交换数据
			if (i != Min)
			{
				swap(rl[i], rl[Min]);
			}
			
		}

		cout << "排序成功！请查看文件" << endl;
		this->Save(); //排序后结果保存到文件中
			
}

int Monthlyreport::getlength(vector<person<int>*> report_list){
    int length=report_list.size();
    return length;
}

double Monthlyreport::average_time(vector<person<int>*> report_list){
    int treatment_number;
    int sum;
    for (int i = 0; i < getlength(rl); i++){
        
        if (rl[i]->status==3){
            double waiting_time = rl[i]->Time[2] - rl[i]->Time[0];
            sum += waiting_time;  
            treatment_number++;   
        }
    }
    double mean = sum/treatment_number;
    return mean;
}
    
int Monthlyreport::appointment_num(vector<person<int>*> report_list){    
    int appointment_number;
    for (int i = 0; i < getlength(rl); i++){
        
        if (rl[i]->status==2 || rl[i]->status==3){
            appointment_number++;
        }
    }
    return appointment_number;
}

int Monthlyreport::withdraw_num(vector<person<int>*> report_list){    
    int withdraw_number;
    for (int i = 0; i < getlength(rl); i++){
        
        if (rl[i]->status==4 || rl[i]->status==5){
            withdraw_number++;
        }
    }
    return withdraw_number;
}

int Monthlyreport::treatment_num(vector<person<int>*> report_list){    
    int treatment_number;
    for (int i = 0; i < getlength(rl); i++){
        
        if (rl[i]->status==3){
            treatment_number++;
        }
    }
    return treatment_number;
}

int Monthlyreport::register_waiting_num(int a, int b){//register_number-appoint_number
    int num = a - b;
    return num;
}

int Monthlyreport::intotal_waiting_num(int a, int b){//register_number-treatment_number
    int num = a - b;
    return num;
}

void Monthlyreport::Save()
{
	//将每个人数据写入到文件中
	for (int i = 0; i < getlength(rl); i++)
	{   
            ofstream ofs;
	        ofs.open("monthly_report.txt", ios::out); // 用输出的方式打开文件  -- 写文件
			ofs << "rigister_number " << getlength(rl) << " "
            << "register_waiting_number " << register_waiting_num(getlength(rl), appointment_num(rl)) << " "
            << "intotal_waiting_number " << register_waiting_num(getlength(rl), treatment_num(rl)) << " "
            << "appointment_number " << appointment_num(rl) << " "
            << "average_waiting_time " << average_time(rl) << " "
			<< "withdraw_number " << withdraw_num(rl) << endl;
            ofs.close();
     }
}

	//关闭文件
    

