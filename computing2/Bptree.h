#ifndef BPTREE_H
#define BPTREE_H

#include <vector>
//#include "block.h"
using namespace std;
//T is tuple
//G is primary
//H is secondary

//测试临时用的block，请不要在意
template<class T,class G, class H> class block
{
public:
    void setid(int id);
    int getid();
private:
    int block_id;
};

template<class T,class G, class H> class Bptree;
template<class T,class G, class H> class Bpnode
{
    friend class Bptree<T,G,H>;
public:
    Bpnode(int node_order = 5);
    bool is_leaf();
    G getkey(int index);    
    Bpnode<T,G,H>* getchild(int index); //Start from 0
    block<T,G,H>* getblock(int index);
    Bpnode<T,G,H>* getparent();
    Bpnode<T,G,H>* getprev();
    Bpnode<T,G,H>* getnext();
    void setleaf(bool status);
    void setkey(G key, int index);
    void setchild(Bpnode<T,G,H>* child, int index);
    void setblock(block<T,G,H>* block, int index);
    void setparent(Bpnode<T,G,H>* node);
    void setprev(Bpnode<T,G,H>* node);
    void setnext(Bpnode<T,G,H>* node);
    void printkey();
    void printblock();  //for test
    //请不要太过于在意一下五个search，容易搞混...
    int child_binary_search(G pri_key);         //输入pri_key,返回的是对应的child/block的index
    int retrieve_binary_search(G pri_key);      //和child_binary_search基本一样，只是 = 的位置不同，从而使key间隔的为(]而不是[)
    int key_binary_search(G pri_key);           //供delete使用，用来找到leaf节点上pri_key的index
    int block_search(block<T,G,H>* blockptr);   //供delete使用，采用linear search输入一个blockptr，找到其所在leaf的index，
    int child_search(Bpnode<T,G,H>* node);      //供merge使用，采用linear search，输入一个node，找到这个node所在nonleaf的index
private:
    bool leaf_status;                           //true if this node is leaf. false by default
    int order;                                  //order of this node
    int num_key;                                //Current number of keys 
    int max_key_num;                            //Maximum key number of this node
    int min_key_num;                            //Minimum key number of this node
    G* keys;                                    //Array of keys in this node
    Bpnode<T,G,H>** children;                   //Array of pointers to children of this node, null for leave
    block<T,G,H>** blocks;                      //Array of pointers to blocks for leave nodes.
    Bpnode<T,G,H>* parent;                      //Parent node of this node, NULL for root
    Bpnode<T,G,H>* prev;                        //pointers to left sibling\. NULL if doesn't exist
    Bpnode<T,G,H>* next;                        //...
};

template<class T,class G,class H> class Bptree
{
public: 
    Bptree(int tree_order = 5);
    block<T,G,H>* retrieve(G pri_key);
    void init_insert(block<T,G,H>* blockptr);           //第一次插入的时候需要，之后都不要用
    void last_delete();                                 //当b+树只有一个block时也要删除时使用，否则不要使用
    void insert(G pri_key, block<T,G,H>* blockptr);     //与kd不同，不是插入一个tuple，而是只插入一个primary key和它对应的block
    void delete1(G oldpri_key, G newpri_key);           //update a primary key in leaves，此次更新只会改变这个key，其位置不应改变，key不用重新排序
    void delete2(G pri_key, block<T,G,H>* blockptr);    //delete a primary key and the blockptr
    //range(G pri_key1, G pri_key2) //return all tuples with primary from pri_key1 to pri_key2
    void prettyprint();                                 //显示的括号里的是 key
private:
    int order;
    Bpnode<T,G,H>* root;
    void _insert(Bpnode<T,G,H>* cur_node, G pri_key, block<T,G,H>* blockptr);   //auxiliary functions
    void _delete1(Bpnode<T,G,H>* cur_node, G oldpri_key, G newpri_key);
    void _delete2(Bpnode<T,G,H>* cur_node, G pri_key, block<T,G,H>* blockptr);
    block<T,G,H>* _retrieve(Bpnode<T,G,H>* cur_node, G pri_key);
    void split(Bpnode<T,G,H>* node);
    void merge(Bpnode<T,G,H>* node);
    void merge1(Bpnode<T,G,H>* l_node, Bpnode<T,G,H>* r_node);  //case 1 的merge,auxiliary function
    void merge2(Bpnode<T,G,H>* l_node, Bpnode<T,G,H>* r_node);  //case 2 的merge,auxiliary function
    void _prettyprint(Bpnode<T,G,H>* cur_node);
};


#endif