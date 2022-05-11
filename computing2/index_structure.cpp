#include "head.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template <class T, class G, class H> index_structure<T,G,H>::index_structure(int mainsize, int oversize)
{
    first_block=new block<T,G,H>(mainsize, oversize);
    i_Bptree=new Bptree<T,G,H>;
    i_Bptree->init_insert(first_block);
    i_btree=new btree<T,G,H>;
    first_block->a_Bptree=i_Bptree;
    first_block->a_btree=i_btree;
}

template <class T, class G, class H> T* index_structure<T,G,H>::bp_retrieve_I(G pri_key)
{
    return i_Bptree->retrieve(pri_key)->bp_retrieve(pri_key);
}

template <class T, class G, class H> void index_structure<T,G,H>::insert_I(T* a_tuple)
{
    block<T,G,H>* one_b = i_Bptree->retrieve(a_tuple->pri_key);
    one_b->insert(a_tuple->pri_key,a_tuple);

    //i_Bptree->retrieve(a_tuple->pri_key)->insert(a_tuple->pri_key,a_tuple);//还要用，前面测试用
    i_btree->bp_insert_B(a_tuple->dataptr, a_tuple->secondary_key);
    return;
}

template <class T, class G, class H> void index_structure<T,G,H>::bp_delete_I(G pri_key)
{
    if(bp_retrieve_I(pri_key)!=NULL)
    {
        H secondary_key = bp_retrieve_I(pri_key)->secondary_key;
        i_btree->bp_delete_B( i_Bptree->retrieve(pri_key)->bp_delete(pri_key), secondary_key );
    }
    return;
}

template <class T, class G, class H> vector<T*>* index_structure<T,G,H>::b_retrieve_I(H secon_key)
{
    vector<T*>* tuple_list=new vector<T*>;
    vector<pair<block<T,G,H>*, int>> datapointer_list=i_btree->b_retrieve_B(secon_key)->datapointer;
    for(int i=0; i<datapointer_list.size(); i++){
        tuple_list->push_back((datapointer_list[i]).first->b_retrieve(datapointer_list[i]));
    }
    return tuple_list;
}

template <class T, class G, class H> void index_structure<T,G,H>::b_delete_I(H secon_key)
{
    vector<pair<block<T,G,H>*, int>> datapointer_list=i_btree->b_delete_B(secon_key)->datapointer;
    for(int i=0; i<datapointer_list.size(); i++){
        (datapointer_list[i]).first->b_delete(datapointer_list[i]);
    }
    return; 
}

template <class T, class G, class H> void index_structure<T,G,H>::prettyprint(){
    cout<<"---Blocklist---\n";
    int j=0;
    
    for(block<relation,int,int>* ptr=first_block; ptr!=NULL; ptr=ptr->r_sibling)
    {
        cout << "Block #" << j << endl;
        ptr->prettyprint();
        j++;
    }
    cout<<"---B+ Tree---\n";
    i_Bptree->prettyprint();
    cout<<"\n---B Tree---\n";
    i_btree->show();
}