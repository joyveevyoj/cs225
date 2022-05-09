#ifndef head_h
#define head_h

#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;
#define mindegree 15
template <class T, class G, class H> class block;
template<class T,class G,class H> class spider;

template<class T,class G,class H> class btree {
private:
	struct bnode {
		spider<T,G,H>* key[2*mindegree-1];	        //create a list to store key,the maximum key number is 2*mindegree-1, the minimum key store is mindegree
		bnode*	child[2*mindegree];		//store the child pointer, each node create 2*mindegree child pointer
		bool	isleaf;					//determine whether it is leaf node
		int		numkey;					//record number of keys
		bnode(){                        //initialize the bnode when created, set isleaf as false, 0 numkey, and all child pointers are null.
			isleaf = false;
			numkey = 0;
			for (int i= 0; i<2*mindegree; i++){
				child[i] = NULL;
				key[i] = NULL ;
			}                          
		}
		H getkey(int i){ 
			return key[i]->second_key; 
		} // get member ith key in key list
		bnode* getchild(int i){ 
			return child[i]; 
		} // get member ith child pointer in child list
		bool getleaf(){ 
			return isleaf; 
		} // get member isleaf
		int getnumk(){ 
			return numkey; 
		}  // get member numkey
		spider<T,G,H>* getspider(int i){
			return key[i] ;
		}

		void setKey(int i,H keyval){ 
			key[i]->second_key = keyval; 
		} // set ith key in list as keyval
		void setChild(int i, bnode* ptr){
			child[i] = ptr; 
		} // set ith child pointer in list as bnode ptr
		void setLeaf(bool boolval){ 
			isleaf= boolval; 
		}  // set isleaf value as boolval
		void setNumk(int num){ 
			numkey = num; 
		} // set numkey as num
		void setspider(int i,spider<T,G,H>* sp){
			key[i]=sp ;
		}

	};
	bnode* root;		//the pointer of root
public:
	btree();
	~btree();
	bool search(H k);								//search k in root
	void split(bnode* x, int i);							//split the child whose index is i of node x
	void insertNon(bnode* x, H k);					//insert the k into the subtree whose root is node x
	void insert(H k);								//insert the k into root
	void merge(bnode* x, int i, bnode* y, bnode* z);		//merge node y, key i and node z, x is the parent of y and z
	void deletek(H k);								//delete the k from root
	void delNon(bnode* x, H k);					//delete the k from the subtree whose root is node x
	spider<T,G,H>* searchPre(bnode* y);							//get the pre of node y
	spider<T,G,H>* searchSuc(bnode* z);							//get the suc of node z
	void shiftRTL(bnode* x, int i, bnode* y, bnode* z);	//x's right child y borrows a key and a child from x's left child of z
	void shiftLTR(bnode* x, int i, bnode* y, bnode* z);	//...
	void doShow(bnode* root, int d);
	void show();										//API for showing the btrees
	void doClear(bnode* root);
	void clear();										//API for free the sources we apply
	spider<T,G,H>* b_retrieve_B(H k);
	spider<T,G,H>* b_delete_B(H k);
	void bp_insert_B(pair<block<T,G,H>*, int> dataptr, H k);
	void bp_delete_B(pair<block<T,G,H>*, int> dataptr,H k);
	void b_update(H k, pair<block<T,G,H>*, int> oldptr, pair<block<T,G,H>*, int> newptr);
	
};

template<class T,class G,class H> class spider{
public:
H second_key ;
vector<pair<block<T,G,H>*, int>> datapointer ;
void dp_show(void){
	cout<< "below is the elements in the vector:  " ;
	for(int i=0;i<datapointer.size();i++){
		cout<<datapointer[i]<<" " ;
	}
	cout << endl ;
}
} ;

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
    void setleaf(bool status);
    void setkey(G key, int index);
    void setchild(Bpnode<T,G,H>* child, int index);
    void setblock(block<T,G,H>* block, int index);
    void setparent(Bpnode<T,G,H>* parent_node);
    void printkey();
    void printblock();  //for test
    int child_binary_search(G pri_key);         //Find the index for the new primary key,返回的是对应的child/block的index
    int retrieve_binary_search(G pri_key);      //和child_binary_search基本一样，只是 = 的位置不同，从而使key间隔的为(]而不是[) don't get confused
private:
    bool leaf_status;                           //true if this node is leaf. false by default
    int order;                                  //order of this node
    int num_key;                                //Current number of keys 
    int max_key_num;                            //Maximum key number of this node
    int min_key_num;                            //Minimum key number of this node
    int max_children_num;                       //Maximum children number of this node
    int min_children_num;    
    G* keys;                                    //Array of keys in this node
    Bpnode<T,G,H>** children;                   //Array of pointers to children of this node, null for leave
    block<T,G,H>** blocks;                      //Array of pointers to blocks for leave nodes.
    Bpnode<T,G,H>* parent;                      //Parent node of this node, NULL for root
   
};

template<class T,class G,class H> class Bptree
{
public: 
    Bptree(int tree_order = 5);
    block<T,G,H>* retrieve(G pri_key);
    void init_insert(block<T,G,H>* blockptr);           //第一次插入的时候需要，之后都不要用
    void insert(G pri_key, block<T,G,H>* blockptr);     //与kd不同，不是插入一个tuple，而是只插入一个primary key和它对应的block
    void delete1(G oldpri_key, G newpri_key);           //update a primary key
    void delete2(G pri_key, block<T,G,H>* blockptr);
    //range(G pri_key1, G pri_key2) //return all tuples with primary from pri_key1 to pri_key2
    void prettyprint();
private:
    int order;
    Bpnode<T,G,H>* root;
    void _insert(Bpnode<T,G,H>* cur_node, G pri_key, block<T,G,H>* blockptr);   //auxiliary functions
    block<T,G,H>* _retrieve(Bpnode<T,G,H>* cur_node, G pri_key);
    void split(Bpnode<T,G,H>* node);
};

template <class T, class G, class H> class block{//T is the type of the relation(the tuple), G is the type of the primary key, H is the type of the secondary key
public:
block(int mainsize, int oversize);//constructor
T* bp_retrieve(G pri_key);//retrieve using primary key and return the pointer to relation tuple
pair<block<T,G,H>*, int> insert(G pri_key, T* tuple);//insert takes the primary key and relation tuple as input, return a datapointer for b tree insersion, which consists of the index for the blovk and the index in the block
pair<block<T,G,H>*, int> bp_delete(G pri_key);//delete using primary key, return the datapointer for b tree deletion 
T* b_retrieve(pair<block<T,G,H>*,int> dataptr);//direct retrieve using datapointer found from B tree
void b_delete(pair<block<T,G,H>*,int> dataptr);//direct delete using datapointer found from B tree
void prettyprint();
vector<T*> mainblock;
vector<T*> overflowblock;
G l_seperate;
G r_seperate;
block<T,G,H>* l_sibling; 
block<T,G,H>* r_sibling;
btree<T,G,H>* a_btree;//ptr to a b tree
Bptree<T,G,H>* a_Bptree;//ptr to a b+ tree
private:
T* tombptr;
int mainb_size;//in the datapointer pair, the index for block is the index for mainblock if it is smaller than mainblock size, if its bigger, index-mianblocksize is index in the overflow block
int overb_size;
int tomb_num;
double h_fillfactor;
double l_fillfactor;
void sort();
void split();
void merge();

};
class relation{
public:
relation(int prikey, int seckey){pri_key=prikey;secondary_key=seckey;dataptr.first=NULL; dataptr_old.first=NULL;};
pair<block<relation, int, int>*, int> dataptr; 
pair<block<relation, int, int>*, int> dataptr_old;
void prettyprint()
{cout<<"the primary key is: ";
cout<<pri_key<<"\n"; 
cout<<"the secondary key is: ";
cout<<secondary_key<<"\n"; 
cout<<"the position in block is: ";
cout<<dataptr.second<<"\n";
};
int pri_key;
int secondary_key;
//int getpri_key(){return pri_key;};
};

template<class T, class G, class H> class index_structure{
public:
block<T,G,H>* first_block;
Bptree<T,G,H>* i_Bptree;
btree<T,G,H>* i_btree;
index_structure(int mainsize, int oversize);
T* bp_retrieve_I(G pri_key);
void insert_I(T* a_tuple);
void bp_delete_I(G pri_key);
vector<T*>* b_retrieve_I(H secon_key);
void b_delete_I(H secon_key);
void prettyprint();
};


#endif