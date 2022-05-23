
//You can use this file to create new test sets
//For more information, please refer to head.h and README document
//Attention:
//Since Registration time is unique, there are at most 3 months and 0.1 hour is the smallest unit in time
//You shouldn't try to generate too much data, which may lead to repeated registration time
#include "head.h"
#include "local_queue.cpp"
int main()
{
    TableWrite t("./Data/input_big.csv",1000,10,10);
    t.table_create();
}