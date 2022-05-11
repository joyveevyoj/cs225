//#pragma once
#ifndef BTREE_H
#define BTREE_H
#include <iostream>
#include <vector>

using namespace std;

#define mindegree 15
template<class T,class G,class H> class spider;

template<class T,class G,class H> class btree
{
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

template<class T,class G,class H> class spider
{
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
#endif