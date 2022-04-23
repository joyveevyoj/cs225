#ifndef head_h
#define head_h

#include <stdio.h>
#include <iostream>


using namespace std;
template <class T, class G, class H> class block{//T is the type of the relation(the tuple), G is the type of the primary key, H is the type of the secondary key
public:
block(int mainsize, int oversize);//constructor
T bp_retrieve(G pri_key);//retrieve using primary key and return the relation tuple
tuple<G, T*, int> insert(G pri_key, T tuple);//insert takes the primary key and relation tuple as input, return a new seperate key when split is done, and a datapointer for b tree insersion, type of split(0 no split, 1 split)
tuple<G, T*, int> bp_delete(G pri_key);//delete using primary key, return the new seperate key if merge, the datapointer for b tree deletion and the type of merge(0 no merge, 1 redistribution, 2 delete a block)
T b_retrieve(T* dataptr);//direct retrieve using datapointer found from B tree
void b_delete(T* dataptr);//direct delete using datapointer found from B tree
private:
vector<T*> mainblock;
vector<T*> overflowblock;
int mainb_size;
int overb_size;
int total_num;
int tomb_num;
int valid_num;
maindata* a_maindata;
block* nextblock;
block* prevblock;
//ptr to a b tree
//ptr to a b++ tree
T split();//split return a new seperate key
T merge(); //merge retrurn a new seperate key 
void sort();

};

template <class T, class G, class H> class maindata{
public:
vector<block<T,G,H>*> blocklist;//maindata contains a list of ptr of blocks
};
#endif