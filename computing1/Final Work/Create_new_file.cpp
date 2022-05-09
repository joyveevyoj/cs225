
//You can use this file to create new test sets
//For more information, please refer to head.h and README document
#include "head.h"
#include "local_queue.cpp"
int main()
{
    TableWrite t("./input_big.csv",200,10,10);
    t.table_create();
}