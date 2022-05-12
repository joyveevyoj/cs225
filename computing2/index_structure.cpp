#include "head.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template <class T, class G, class H> index_structure<T,G,H>::index_structure(int mainsize, int oversize)
{
    first_block = new block<T,G,H>(mainsize, oversize);
    i_Bptree = new Bptree<T,G,H>;
    i_Bptree->init_insert(first_block);
    i_btree = new btree<T,G,H>;
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
        // block<T,G,H>* blk = i_Bptree->retrieve(pri_key);
        // // cout << "---This block before delete---" << endl;
        // // blk->prettyprint();
        // // cout << "---Left block before delete---" << endl;
        // // blk->l_sibling->prettyprint();
        // blk->bp_delete(pri_key);//.first->prettyprint();
        // cout << "---This block after delete---" << endl;
        // blk->prettyprint();
        // cout << "---Left block after delete---" << endl;
        // blk->l_sibling->prettyprint();
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

//只删除spider里面的一个，返回被删的tuple的dataptr，再在index_structure删除里对block删除，并在block里对相关的tuple更新b树的dataptr
//然后循环，直到这个spider全部删完
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
            //如果是第一个block，选择放
            if(cur_block == init_block)
            {
                if(tuple->pri_key >= init_key)
                    result->push_back(tuple);
            }
            //如果不是第一个block，全部放
            if(cur_block != init_block)
            {
                result->push_back(tuple);
            }
        }
        cur_block = cur_block->r_sibling;  
    }
    //获取左后一个block的tuple
    cur_block->sort();
    for(int i = 0; i < cur_block->mainblock.size(); i++)
    {
        T* tuple = cur_block->mainblock[i];
        if(tuple->pri_key <= last_key)
            result->push_back(tuple);
    }
    return result;
}

template<class T, class G, class H> vector<T*>* index_structure<T,G,H>::query(G init_key, G last_key)
{
    return range(init_key, last_key);
}

template<class T, class G, class H> vector<T*>* index_structure<T,G,H>::query(G pri_key)
{
    vector<T*>* result = new vector<T*>;
    result->push_back( bp_retrieve_I(pri_key) );
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
    
    for(block<relation,int,int>* ptr=first_block; ptr!=NULL; ptr=ptr->r_sibling)
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
    for(int i = 0; i < num; i++)
    {
        result->at(i)->prettyprint();
        cout << endl;
    }
}