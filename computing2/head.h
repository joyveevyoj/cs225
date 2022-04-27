#ifndef head_h
#define head_h

#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;
template <class T, class G, class H> class block{//T is the type of the relation(the tuple), G is the type of the primary key, H is the type of the secondary key
public:
block(int mainsize, int oversize);//constructor
T* bp_retrieve(G pri_key);//retrieve using primary key and return the pointer to relation tuple
pair<block<T,G,H>*, int> insert(G pri_key, T* tuple);//insert takes the primary key and relation tuple as input, return a datapointer for b tree insersion, which consists of the index for the blovk and the index in the block
pair<block<T,G,H>*, int> bp_delete(G pri_key);//delete using primary key, return the datapointer for b tree deletion 
T* b_retrieve(pair<int,int> dataptr);//direct retrieve using datapointer found from B tree
void b_delete(pair<int,int> dataptr);//direct delete using datapointer found from B tree
vector<T*> mainblock;
vector<T*> overflowblock;
block<T,G,H>* l_sibling;
block<T,G,H>* r_sibling;
private:
T* tombptr;
int mainb_size;//in the datapointer pair, the index for block is the index for mainblock if it is smaller than mainblock size, if its bigger, index-mianblocksize is index in the overflow block
int overb_size;
int tomb_num;
double h_fillfactor;
double l_fillfactor;
//ptr to a b tree
//ptr to a b+ tree
void split();//split is only triggered by insert, so it return the dataptr second index in the bock
void merge();
void sort();

};


#endif