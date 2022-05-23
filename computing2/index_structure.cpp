#include "head.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
//index structure is a class that put b tree, b+ tree, and blocks all together 
//and integrate them to realize retrieve, insert, and delete functions

//constructor that initialize the index structure
template <class T, class G, class H> index_structure<T,G,H>::index_structure(int mainsize, int oversize)
{
    first_block=new block<T,G,H>(mainsize, oversize);
    i_Bptree=new Bptree<T,G,H>;
    i_Bptree->init_insert(first_block);
    i_btree=new btree<T,G,H>;
    first_block->a_Bptree=i_Bptree;
    first_block->a_btree=i_btree;
}
//bp_retrieve first use pri_key to search in bp_tree, use the blockptr found to retrieve tuple in corresponding block
template <class T, class G, class H> T* index_structure<T,G,H>::bp_retrieve_I(G pri_key)
{
    return i_Bptree->retrieve(pri_key)->bp_retrieve(pri_key);
}
//insert_I first search in bp tree, to find the corresponding block
//then insert the tuple in the block 
//use new dataptr to insert into b tree
template <class T, class G, class H> void index_structure<T,G,H>::insert_I(T* a_tuple)
{
    block<T,G,H>* one_b = i_Bptree->retrieve(a_tuple->pri_key);
    one_b->insert(a_tuple->pri_key,a_tuple);
    i_btree->bp_insert_B(a_tuple->dataptr, a_tuple->secondary_key);
    return;
}

//bp_delete_I use the pri_key to first find the right block, delete it in block, and use the 
//dataptr found to delete it in b tree
template <class T, class G, class H> void index_structure<T,G,H>::bp_delete_I(G pri_key)
{
    if(bp_retrieve_I(pri_key)!=NULL)
    {
        H secondary_key = bp_retrieve_I(pri_key)->secondary_key;
        i_btree->bp_delete_B( i_Bptree->retrieve(pri_key)->bp_delete(pri_key), secondary_key );
    }
    return;
}

//use the secondary key to retrieve all the tuples in the block
template <class T, class G, class H> vector<T*>* index_structure<T,G,H>::b_retrieve_I(H secon_key)
{   
    vector<T*>* tuple_list=new vector<T*>; 
    if (i_btree->b_retrieve_B(secon_key)!=NULL)
    {  
    vector<pair<block<T,G,H>*, int> > datapointer_list=i_btree->b_retrieve_B(secon_key)->datapointer;
    for(int i=0; i<datapointer_list.size(); i++)
     {
     tuple_list->push_back((datapointer_list[i]).first->b_retrieve(datapointer_list[i]));
     
      }
       return tuple_list;
    }
  
   return NULL;
}

//delete all the tuples with the same the secon_key
template <class T, class G, class H> void index_structure<T,G,H>::b_delete_I(H secon_key)
{
    pair<block<T,G,H>*, int> dataptr = i_btree->b_delete_B(secon_key);
    while ( dataptr.first != NULL)
    {
        dataptr.first->b_delete(dataptr);
        dataptr = i_btree->b_delete_B(secon_key);
    }
    return; 
}

//return the vector with all the tuples with pri_key ranging from init_key to last_key
template <class T, class G, class H> vector<T*>* index_structure<T,G,H>::range(G init_key, G last_key)
{
    block<T,G,H>* init_block = i_Bptree->retrieve(init_key);
    block<T,G,H>* last_block = i_Bptree->retrieve(last_key);
    vector<T*>* result = new vector<T*>;
    block<T,G,H>* cur_block = init_block;

    while(cur_block != last_block)
    {
        cur_block->sort();
        for(int i = 0; i < cur_block->mainblock.size(); i++)
        {
            T* tuple = cur_block->mainblock[i];
            //If it's the first block, select 
            if(cur_block == init_block)
            {
                if(tuple->pri_key >= init_key)
                    result->push_back(tuple);
            }
            //If it's not the first block, get everything
            if(cur_block != init_block)
            {
                result->push_back(tuple);
            }
        }
        cur_block = cur_block->r_sibling;  
    }
    //Put tuples in the last block
    cur_block->sort();
    for(int i = 0; i < cur_block->mainblock.size(); i++)
    {
        T* tuple = cur_block->mainblock[i];
        if(tuple->pri_key <= last_key&&tuple->pri_key >= init_key)
            result->push_back(tuple);
    }
    return result;
}
// the same as range
template<class T, class G, class H> vector<T*>* index_structure<T,G,H>::query(G init_key, G last_key)
{
    return range(init_key, last_key);
}

//query for the tuple with pri_key
template<class T, class G, class H> vector<T*>* index_structure<T,G,H>::query(G pri_key)
{
    vector<T*>* result = new vector<T*>;
    T* rtval;
    rtval= bp_retrieve_I(pri_key);
    if(rtval)
        result->push_back( rtval );
    return result;
}

template <class T, class G, class H> void index_structure<T,G,H>::prettyprint()
{
    printblock();
    printBptree();
    printbtree();
}

template <class T, class G, class H> void index_structure<T,G,H>::printblock()
{
    cout<<"---Blocklist---\n";
    int j=0;
    
    for(block<T,G,H>* ptr=first_block; ptr!=NULL; ptr=ptr->r_sibling)
    {
        cout << "Block #" << j << endl;
        ptr->prettyprint();
        j++;
    }
}

template <class T, class G, class H> void index_structure<T,G,H>::printBptree()
{
    cout<<"\n---B+ Tree---\n";
    i_Bptree->prettyprint();
}

template <class T, class G, class H>void index_structure<T,G,H>::printbtree()
{
    cout<<"\n---B Tree---";
    i_btree->show();
}

template <class T> void printquery(vector<T*>* result)
{
    int num = result->size();
    if(num==0){
        cout<<"Relevant keys not found"<<endl;
    }
    for(int i = 0; i < num; i++)
    {
        result->at(i)->totalprint();
        cout << endl;
    }
}

//update the secondary key in b tree
template <class T, class G, class H> void index_structure<T,G,H>::b_update_seckey(T* up_tuple){
    i_btree->bp_delete_B(up_tuple->dataptr,up_tuple->old_secon_inB);
    i_btree->bp_insert_B(up_tuple->dataptr, up_tuple->status);
    bp_retrieve_I(up_tuple->pri_key)->old_secon_inB= up_tuple->secondary_key;

}