#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include "local_queue.h"
#include "local_queue.cpp"
using namespace std;

void TableWrite::table_open(string filename)
{
    outfile.open(filename);
    if(!outfile.is_open())
    {
        cout << "Error: Opening a file fails" << endl;
        exit(EXIT_FAILURE);
    }
}
void TableWrite::table_close()
{
    outfile.close();   
}

void TableWrite::table_line(person<int> p)
{
    string line;
    
    

}
void TableWrite::table_create(string filename,int num,int week)
{
    this->table_open(filename);
    //第一行
    string first_row;
    first_row = "ID,Name,Address,Phone,WeChat,Email,Profession,Age,Risk Status,Registration Time,Withdraw,Priority Letter";
    outfile << first_row;
    //创造num 个人
    person<int>* p = new person<int>[num];
    for(int i = 0;i < num;i++)
    {
        p[i].random_generate(i,week);
    }
    
    for(int i = 0;i < num; i++)
    {
        cout << p[i].show_format_time()[0] << endl;
    }
    
    this->table_close();

}
int main()
{
    TableWrite t1;
    t1.table_create("./registry.csv",10,1);
}

