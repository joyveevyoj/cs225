
//You can use this file to create new test sets
//For more information, please refer to head.h and README document
#include "head.h"
#include "local_queue.cpp"
int main()
{
    TableWrite t("./testfile.csv",500,10,5);
    t.table_create();

    // Test code
    // string aline;
    // ifstream file;
    // file.open("./testfile.csv");
    // getline(file,aline);
    // getline(file,aline);
    // cout << aline << endl;
    // string endmark="end of half day";
    // if(aline != endmark)
    // {
    //     person<int>* p = new person<int>(aline);
    //     p->display_all();
    // }
}