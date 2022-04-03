#include "report.h"
#include<vector>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <fstream>


void Weeklyreport::save()
{
	string s1 = "File1.txt";
    string s2 = "File2.txt";
    string s3 = "File3.txt";
	//将每个人数据写入到文件中
	for (int i = 0; i < num; i++)
	{   
        if(rl[i]->status==1){
            ofstream ofs;
	        ofs.open(s1, ios::out); // 用输出的方式打开文件  -- 写文件
		    ofs << rl[i]->show_name() << " "
			<< rl[i]->prof << " "
            << rl[i]->age << " "
            << rl[i]->risk << " "
			<< rl[i]->waitingtime << endl;
            ofs.close();
            }
        else if(rl[i]->status==2)
            {
            ofstream ofs;
	        ofs.open(s2, ios::out); // 用输出的方式打开文件  -- 写文件
		    ofs << rl<person*>[i]->name << " "
			<< rl<person*>[i]->prof << " "
            << rl<person*>[i]->age << " "
            << rl<person*>[i]->risk << " "
			<< rl<person*>[i]->waitingtime << endl;
            ofs.close();
            }
        else{
            ofstream ofs;
	        ofs.open(s3, ios::out); // 用输出的方式打开文件  -- 写文件
		    ofs << rl<person<int>*>[i]->name << " "
			<< rl<person<int>*>[i]->prof << " "
            << rl<person<int>*>[i]->age << " "
            << rl<person<int>*>[i]->risk << " "
			<< rl<person<int>*>[i]->waitingtime << endl;
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
        cout << "2、按risk进行排序" << endl;

		int select = 0;
		cin >> select;
		for (int i = 0; i < num; i++)
		{
			int minOrMax = i; //声明最小值 或 最大值下标
			for (int j = i + 1; j < num; j++)
			{
				if (select == 1) //升序
				{
					if (rl[minOrMax]->name > rl[j]->name)
					{
						minOrMax = j;
					}
				}
				elseif (select) //降序
				{
					
				}
			}

			//判断一开始认定 最小值或最大值 是不是 计算的最小值或最大值，如果不是 交换数据
			if (i != minOrMax)
			{
				Worker * temp = this->m_EmpArray[i];
				this->m_EmpArray[i] = this->m_EmpArray[minOrMax];
				this->m_EmpArray[minOrMax] = temp;
			}
			
		}

		cout << "排序成功！排序后的结果为： " << endl;
		this->save(); //排序后结果保存到文件中
		this->Show();//展示所有职工
	
	
}