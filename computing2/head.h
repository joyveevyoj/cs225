#ifndef head_h
#define head_h

#include <stdio.h>
#include <iostream>
#include<vector>
#include <fstream>
#include "cassert"
#include <string>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iomanip>
#include <cstdlib>
using namespace std;
class appointment; 
class hospital;
#define mindegree 15
template <class T, class G, class H> class block;
template<class T,class G,class H> class spider;
template<class T, class G, class H> class index_structure;
template<class T,class G,class H> class btree
{
private:
	struct bnode
	{
		spider<T,G,H>* key[2*mindegree-1];	        //create a list to store key,the maximum key number is 2*mindegree-1, the minimum key store is mindegree
		bnode*	child[2*mindegree];					//store the child pointer, each node create 2*mindegree child pointer
		bool	isleaf;								//determine whether it is leaf node
		int		numkey;								//record number of keys
		bnode()						 				//initialize the bnode when created, set isleaf as false, 0 numkey, and all child pointers are null.
		{                       
			isleaf = false;
			numkey = 0;
			for (int i= 0; i<2*mindegree; i++)
			{
				child[i] = NULL;
				key[i] = NULL ;
			}                          
		}

		H getkey(int i) { return key[i]->second_key; }	 	            // get member ith key in key list
		bnode* getchild(int i) { return child[i]; }		                // get member ith child pointer in child list
        bool getleaf()  { return isleaf; }		                        // get member isleaf
        int getnumk() { return numkey; }                                // get member numkey
        spider<T,G,H>* getspider(int i ) { return key[i];}		                                   
		void setKey(int i,H keyval) { key[i]->second_key = keyval;}     // set ith key in list as keyval
		void setChild(int i, bnode* ptr) { child[i] = ptr; }            // set ith child pointer in list as bnode ptr	 	                                                            
		void setLeaf(bool boolval) { isleaf = boolval; }                // set isleaf value as boolval		                                                                
		void setNumk(int num) { numkey = num; }                         // set numkey as num
		void setspider(int i,spider<T,G,H>* sp) { key[i] = sp; }
	};
	bnode* root;		//the pointer of root
public:
	btree();
	~btree();
	bool search(H se_key);									//search secondary key in root
	void split(bnode* x, int i);							//split the child whose index is i of node x
	void insert_root(bnode* r, H k);						// insert secondary key from given root r
	void insert(H se_key);									//insert the secondary k 
	void merge(bnode* x, int i, bnode* y, bnode* z);		//merge bnode y and z and spider key[i], and x is y z's parent,y is the left child node, z is the right child node
	void deletek(H k);										//delete the k from root
	void delete_root(bnode* r, H k);						//delete the k from the subtree r
	spider<T,G,H>* searchahead(bnode* y);					//get the spider with smallest ahead of y
	spider<T,G,H>* searchbehind(bnode* z);					//get the spider with biggest behind z
	void borrowrl(bnode* x, int i, bnode* y, bnode* z);		//x's right child z borrows a spider and a child from x's left child of y
	void borrowlr(bnode* x, int i, bnode* y, bnode* z);		//x's left child y borrows a spider and a child from x's right child of z
	void doShow(bnode* root, int d);	
	void show();											//Format：A node is represented with (Keynum, seckey1, seckey2...)，-> means children relationship. 
															//A node cantains the following information: 
                                                            //(Sec: current seckey，L_Sep：it's located block's left separate key， R_Sep:it's located block's right separate key，Loc:Index of the tuple in its block)
	spider<T,G,H>* b_retrieve_B(H k);                       //return the spider with secondarykey k in tree
	pair<block<T,G,H>*, int> b_delete_B(H k);               // pop one element in spider with key
	void bp_insert_B(pair<block<T,G,H>*, int> dataptr, H k); //insert dataptr with k into the tree
	void bp_delete_B(pair<block<T,G,H>*, int> dataptr,H k);  //delete one dataptr with k in the tree
	void b_update(H k, pair<block<T,G,H>*, int> oldptr, pair<block<T,G,H>*, int> newptr);//in spider with key,update oldptr into newptr
	
};

template<class T,class G,class H> class spider
{
public:
	H second_key ;
	vector<pair<block<T,G,H>*, int> > datapointer ;
	void dp_show(void)
	{
		for(int i = 0; i < datapointer.size(); i++)
		{
			cout << "(L_Sep:";
			if(datapointer[i].first->l_seperate == -999999)
				cout << "None";
			else
				cout << datapointer[i].first->l_seperate;
			cout << ", ";
			cout << "R_Sep:";
			if(datapointer[i].first->r_seperate == -999999)
				cout << "None";
			else
				cout << datapointer[i].first->r_seperate;
			cout << ", ";
			cout << "Loc:";
			cout << datapointer[i].second << ") ";
		}
	}
};

//T is the type of the relation(the tuple), G is the type of the primary key, H is the type of the secondary key
template<class T,class G, class H> class Bptree;
template<class T,class G, class H> class Bpnode
{
    friend class Bptree<T,G,H>;
public:
    Bpnode(int node_order = 5);
    bool is_leaf();
    G getkey(int index);    
	int getkeynum();								    //For block class to use(though not well set)
    Bpnode<T,G,H>* getchild(int index);         	    //Start from 0
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
    void printblock();                                  
    //These search functions can be confusing. Don't care too much about them
    int child_binary_search(G pri_key);                 //Input: pri_key, return corresponding index of child/block
    int retrieve_binary_search(G pri_key);              //Almost the same as child_binary_search. Difference is the location of = so that key interval is (] instead of [)
    int key_binary_search(G pri_key);                   //For delete to use. Find index of prikey on a leaf node
    int block_search(block<T,G,H>* blockptr);           //For delete to use. Input: blockptr, linear search to find its index on the leaf node
    int child_search(Bpnode<T,G,H>* node);              //For merge to use. Input: node, linear search to find its index on the parent node
private:
    bool leaf_status;                                   //true if this node is leaf. false by default
    int order;                                          //order of this node
    int num_key;                                        //Current number of keys 
    int max_key_num;                                    //Maximum key number of this node
    int min_key_num;                                    //Minimum key number of this node
    G* keys;                                            //Array of keys in this node
    Bpnode<T,G,H>** children;                           //Array of pointers to children of this node, null for leave
    block<T,G,H>** blocks;                              //Array of pointers to blocks for leave nodes.
    Bpnode<T,G,H>* parent;                              //Parent node of this node, NULL for root
    Bpnode<T,G,H>* prev;                                //pointers to left sibling\. NULL if doesn't exist
    Bpnode<T,G,H>* next;                                //...
};

template<class T,class G,class H> class Bptree
{
public: 
    Bptree(int tree_order = 5);
	Bpnode<T,G,H>* block_retrieve(block<T,G,H>* blockptr);	//Auxiliary function for block. Input: blockptr, return it's the leaf containing this block
    block<T,G,H>* retrieve(G pri_key);                      //Input: prikey, return block containing this prikey
    void init_insert(block<T,G,H>* blockptr);               //Required to insert the first block. Otherwise don't use this function
    void insert(G pri_key, block<T,G,H>* blockptr);
    void last_delete();                                     //Delete the block if there is only one block left in the tree. Otherwise don't use this function    void insert(G pri_key, block<T,G,H>* blockptr);         //与kd不同，不是插入一个tuple，而是只插入一个primary key和它对应的block
    void delete1(G oldpri_key, G newpri_key);               //Update a primary key in leaves. Only udpate this key so no need to rearrange key.
    void delete2(G pri_key, block<T,G,H>* blockptr);        //Uelete a primary key and the blockptr
    void prettyprint();                                     //Format: Use primary keys to represent a node as follows (key1, key2, key3...). [l_seperate, r_seperate] represents block in a leaf node
private:
    int order;
    Bpnode<T,G,H>* root;
    //These are all auxiliary functions
    void _insert(Bpnode<T,G,H>* cur_node, G pri_key, block<T,G,H>* blockptr);   
    void _delete1(Bpnode<T,G,H>* cur_node, G oldpri_key, G newpri_key);
    void _delete2(Bpnode<T,G,H>* cur_node, G pri_key, block<T,G,H>* blockptr);
    block<T,G,H>* _retrieve(Bpnode<T,G,H>* cur_node, G pri_key);
	Bpnode<T,G,H>* _block_retrieve(Bpnode<T,G,H>* cur_node, block<T,G,H>* blockptr);
    void split(Bpnode<T,G,H>* node);
    void merge(Bpnode<T,G,H>* node);
    void merge1(Bpnode<T,G,H>* l_node, Bpnode<T,G,H>* r_node);  //Case 1 merge
    void merge2(Bpnode<T,G,H>* l_node, Bpnode<T,G,H>* r_node);  //case 2 merge
    void _prettyprint(Bpnode<T,G,H>* cur_node, int level);
};

template <class T, class G, class H> class block
{	
public:
	block(int mainsize, int oversize);						//constructor
	T* bp_retrieve(G pri_key);								//retrieve using primary key and return the pointer to relation tuple
	pair<block<T,G,H>*, int> insert(G pri_key, T* tuple);	//insert takes the primary key and relation tuple as input, return a datapointer for b tree insersion, which consists of the index for the blovk and the index in the block
	pair<block<T,G,H>*, int> bp_delete(G pri_key);			//delete using primary key, return the datapointer for b tree deletion 

	T* b_retrieve(pair<block<T,G,H>*,int> dataptr);			//direct retrieve using datapointer found from B tree
	void b_delete(pair<block<T,G,H>*,int> dataptr);			//direct delete using datapointer found from B tree
	void prettyprint();										//Format：a tuple is represented as （Loc: tuple's index in block，Pri: tuple's prikey, Sec: tuple's seckey in block)
	vector<T*> mainblock;
	vector<T*> overflowblock;
	G l_seperate;
	G r_seperate;
	block<T,G,H>* l_sibling; 
	block<T,G,H>* r_sibling;
	btree<T,G,H>* a_btree;									//ptr to a b tree
	Bptree<T,G,H>* a_Bptree;								//ptr to a b+ tree
	void sort();                                            //index structure has to borrow this function to implement range operation, though
    T* tombptr;											    
private:
	int mainb_size;											//in the datapointer pair, the index for block is the index for mainblock if it is smaller than mainblock size, if its bigger, index-mianblocksize is index in the overflow block
	int overb_size;
	int tomb_num;
	double h_fillfactor;
	double l_fillfactor;

	void split();
	void merge();

};



// A class model for testing our index structure
class relation
{
public:
	relation(int prikey, int seckey){pri_key=prikey;secondary_key=seckey;dataptr.first=NULL; dataptr_old.first=NULL;};
    relation(){};
	pair<block<relation, int, int>*, int> dataptr; 
	pair<block<relation, int, int>*, int> dataptr_old;
	void totalprint()                                       //Used if tuples are to printed separatedly
	{
		cout << "(Loc:" << dataptr.second << " ,Pri:" << pri_key << " ,Sec:" << secondary_key << ") in block [";
		if(dataptr.first->l_seperate == -999999)
			cout << "None";
		else
			cout << dataptr.first->l_seperate;
		cout << ", ";
		if(dataptr.first->r_seperate == -999999)
			cout << "None";
		else
			cout << dataptr.first->r_seperate;
        cout << "] ";
	};
    void prettyprint()                                      //Used if tuples are stored in the same block
    {
	    cout << "(Loc:" << dataptr.second << " ,Pri:" << pri_key << " ,Sec:" << secondary_key << ")";
    };
	int pri_key;
	int secondary_key;
};

template<class T, class G, class H> class index_structure
{
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
																//Attention: query puts everything into mainblock. b tree dataptr updates as well.
	vector<T*>* range(G init_key, G last_key);					//Find all tuples bewtween initkey and last kek. Return a pointer to the vector containing them
	vector<T*>* query(G init_key, G last_key);					
	vector<T*>* query(G pri_key);								//Two different cases pf query
	void prettyprint();
	void printblock();
	void printBptree();
	void printbtree();
	void b_update_seckey(T* up_tuple);
};




template <class T> class fifo
{
public:
    fifo(int size = 10);
    int getlength(void);
    bool isempty(void);
    T back(void);
    T front(void);
    void pushback(T value);
    T popfront();
private:
    int maxsize, minsize;
    int first, last;
    int numitems;
    T* reprarray;
    void allocate(void);
    void deallocate(void);
};

class Medical_status;
class Registration;
//For person, T is the type of key and set is integer
template <class T>class person
{
public:
    friend class TableWrite;
    friend class Weeklyreport;
    friend class Monthlyreport;
    friend class Medical_status;
    friend class Registration;
    friend class appointment;
    person();                                       //If no argument, create an empty person
    person(string tableline);                       //Create a person from local registry
    void random_generate(int seed);                 //Used to put random attributes into a person
    void set_key(); 
    T return_key();
    string show_id();
    double* show_hour();
    int* show_intHour();
    int* show_day();
    int* show_half_day();
    int* show_week();
    int* show_month();  
    string* show_format_time();                     //All Time starts at zero
    bool is_update(person<int>* oldper);            //check whether this is an update
    bool is_newwithdraw(person<int>* oldper);       //check whether this is a new withdraw
    bool is_withdraw(); //return withdraw
    void set_appointment(appointment* r_appoint);
    void set_assign_appointment_time(double hour);
    void punish();   
    void display_all();                             //Display all information of this person
    string show_risk();
    void withdraw_time();
    int treatment_id;                               //What kind of treatment this person is
    string treatment;
    int local_id;  
    int* hospital_id;                               //Array with index for the hospital，即hopsital's ranking list 
    int hosp_num;                                   //Initailized when the person is created
    int status;
    appointment* p_appoint;
    void withdraw_time(double hour);

    //for index_structure use
    long long  pri_key;
    int secondary_key;
    int old_secon_inB;
    pair<block<person<int>, long long, int>*, int> dataptr; 
    pair<block<person<int>, long long, int>*, int> dataptr_old;
    void prettyprint()
    {cout << "(Loc:" << dataptr.second << " ,Pri:" << pri_key << " ,Sec:" << secondary_key << ") ";};
    T key;
private:
    bool letter;                                    //true if there is letter for ddl
    int risk_id;
    string risk;                                    //0,1,2,3
    int prof_id;                                    //0,1,2,3,4,5,6,7, the larger, the higher priority
    string prof;                                    //String name for profession
    int age_id;
    string age;                                     //0,1,2,3,4,5,6, the larger, the higher priority
    string id;
    string name;
    string address;
    string phone;
    string Wechat;
    string email;
    bool withdraw;                                  //true if withdraw
    bool wont_withdraw;                             //Used only for creating peson who withdraw. Don't get confused
    double* Time;                                   //Array representing time. 0 is registeration time，1 is appointment time,2 is the time when appointment is assigned
                                                    //If not initialized, set as -1;
    int updated_half_day;                           //On which halfday this person updates. -1 means no update
    bool time_exist(double item);                   //To avoid registration time repetition

};

class TableWrite
{
public:
    TableWrite(string filename_,int num_,int updated_num_,int withdraw_num_);   //num: total number of person, updated_num: number of people who update, withdraw_num: number of people who withdraw
                                                                                
    void table_create();
    string table_line(person<int>& p);                                          //Create a tableline for every person
private:
    ofstream outfile;
    string filename;
    int num;
    int updated_num;
    int withdraw_num;
    void table_open(string filename);
    void table_close();
    person<int>* SelectionSort(person<int>* p,int num_of_person,int way);       //way = 0: order according to registration time，way = 1: order occording which half day this person is in
};




template <class T> class Fb_node 
{
public:
    int key;                // use calculated key to have its priority ;
    person<int>* patient ;     
    Fb_node<T> *left;       // point to left node 
    Fb_node<T> *right;      //  point to right node
    Fb_node<T> *child;      //  point to its child
    Fb_node<T> *parent;     // point to its parent
    bool       mark;        // mark whether it has lost child
    int        degree;      //indicate the number of children
    Fb_node(person<int>* tpatient):key(tpatient->return_key()), patient(tpatient),mark(false),left(NULL),right(NULL),child(NULL),parent(NULL),degree(0) {
        key    = tpatient->return_key();  // use constructor to initialize, key is the person's calculate key
        patient = tpatient ;    // person is this patient
        left   = this; 
        right  = this;          // left and right both point to itself
        parent = NULL;          //parent and child are none
        child  = NULL;
        degree = 0;         
        mark = false;
    }
};

template <class T> class Fb_heap {
private:
    int numnodes;                                               //number of nodes in the heap
    Fb_node<T> *min;                                            // points to the node with minimum value
    int maxdrg;                                                 // the maxdegree in the heap , in order to compute list space
    Fb_node<T> **drglist;                                       // use to store the list of degree 

private:
    void addlistNode(Fb_node<T> *ptr, Fb_node<T> *root);        // add node into the left of root in the list
    void rmlistNode(Fb_node<T> *ptr);                           // remove this nodes' pointer from the list of nodes
    void insert(Fb_node<T> *ptr);                               // insert new node ptr into the heap
    Fb_node<T>* getminnode();                                   // get the tree of the minnode
    void nodelink(Fb_node<T>* ptr, Fb_node<T>* r);              //link ptr to root r and becomes r's child
    void renewdrg(Fb_node<T> *tparent, int degree);             // renew the degree
    void cut(Fb_node<T> *node, Fb_node<T> *parent);             // cut the node into the root list
    void markcut(Fb_node<T> *ptr) ;                             // check the mark of the node who lost his child
    void consolidate();                                         // let the degree of the nodes in root list are different
    Fb_node<T>* search(Fb_node<T> *root, person<int>* patient); // from root to search patient
    Fb_node<T>* search(person<int>* patient);                   // search the patient
    void decrease(Fb_node<T> *ptr, T newkey);                   // decrease key into newkey
    void increase(Fb_node<T> *ptr, T newkey);                   // increase key into newkey
    void update(Fb_node<T> *ptr, T newkey);                     // update the key in ptr into new key
    void remove(Fb_node<T> *ptr);                               // remove ptr from the heap

    void buildlsarea()                                          //allocate area for the degree list
    { 
        int olddrg = maxdrg; //store old degree
        maxdrg = (log(numnodes)/log(2.0)) + 1;
        if (olddrg >= maxdrg) { return; }  // if the old degree is bigger than new maxdgree, return
        drglist = (Fb_node<T> **)realloc(drglist, sizeof(Fb_heap<T> *) * (maxdrg + 1)); // allocate area
    }; 

public:
    Fb_heap(){
    numnodes = 0;
    maxdrg = 0;                                                     // initialize numnodes and max of degree to 0
    min = NULL;
    drglist = NULL;                                                 //constructor
    };
    /*~Fb_heap();  // deconstructor*/
    void insert(person<int>* patient);                              //insert a new patient into the fibonacci heap
    person<int>* delete_min();                                      // delete the minnode and return the pointer to this deleted person
    void update(person<int>*newpatient, person<int>* oldpatient);   // update the old patient's key value into new value and new pointer
    void remove(person<int>* oldpatient);                           //delete this old patient
    int getnum(){return numnodes;};                                 // return number of nodes in the heap
    person<int>* rtminperson(void){return min->patient ;};
    void showpriority(void);
};


class appointment
{
public:
    appointment(person<int>* a_patient, int date_out_of_queue, vector<hospital*> a_hospital_list);
    //virtual ~appointment();
    appointment(){};
    vector<hospital*> hospitals;
    void make_appointment();
    void appointment_withdraw();
    void pretty_print();
    bool is_appointment_passed(double current_hour);
    int hospital_id;
    person<int>* patient;
    double the_time;
    int date;
    //for index structure use
    string treatment;
    int status;                                             //the state shows whether this person go to the appointment
    long long pri_key;                                      //primary key is the person id
    int secondary_key;                                      //secondary key is the date
    pair<block<appointment, long long, int>*, int> dataptr;
    pair<block<appointment, long long, int>*, int> dataptr_old;
    void prettyprint()
    {   
        cout << "(Loc:" << dataptr.second << " ,Pri:" << pri_key << " ,Sec:" << secondary_key << ") ";
    }
};

class hospital
{
public:
    hospital(int idnum, int capacity, double o_time, double c_time);
    bool is_hospital_available(int h_date);
    double get_time_appointed(int h_date);
    int id;                                             //the hospital's unique id number, which marks its position in the hospital list.
    index_structure<appointment, long long, int>* daily_appointment_list =new index_structure<appointment, long long, int>(15,4);//index structure that stores all the appointment tuples
    void add_to_applist(appointment* new_app);
    void printapp();
    void withdraw_app(appointment* w_app);
    int daily_remaining_capacity[200];
private:
    int daily_capacity;
    double open_time;
    double close_time;
    double treatment_time;

};


class reportlist
{
  public:
  void push_new(int r_status, person<int>* patient);            //push a new person into the report list, and set his status value
  void push_old(int r_status, person<int>* patient);            //use a temperary person to update the person already in the report list and set his status value
  void push_appoi(int r_status, appointment* r_appointment);    //put the appointment information into the person stored in the report list and set his status value
  person<int>* exist(person<int>* );                            //this function checks the temperary person's id and see whether it is already in the report list, if it is                                 
                                                                //return the person's addr, else return null
  index_structure<person<int>, long long, int>* rl =new index_structure<person<int>, long long, int>(15,4);// a index structure that stores all the information about that person
};

class Monthlyreport
{
public:
    double current_time;
    Monthlyreport(index_structure<person<int>, long long, int>* report_list, double Time){
        rl = report_list;
        current_time = Time;
        rl_0 = merge(rl,rl->b_retrieve_I(1),rl->b_retrieve_I(2),rl->b_retrieve_I(3),rl->b_retrieve_I(4),rl->b_retrieve_I(5));
    }
    vector<person<int>*>* merge(index_structure<person<int>, long long, int>* rl,vector<person<int>*>* rl_1,vector<person<int>*>* rl_2,vector<person<int>*>* rl_3,vector<person<int>*>* rl_4,vector<person<int>*>* rl_5);
    int getlength(vector<person<int>*> report_list);
    int getlength_m(vector<person<int>*> report_list, int month);
    int appointment_num(vector<person<int>*> report_list, int month);
    int treatment_num(vector<person<int>*> report_list, int month);
    int withdraw_num(vector<person<int>*> report_list, int month);
    double average_time(vector<person<int>*> report_list, int month);
    int register_waiting_num(int id, int month);
    int intotal_waiting_num(int a, int b, int c);
    void Save(int month);
private:
    index_structure<person<int>, long long, int>* rl;
    vector<person<int>*>* rl_0;
};


class report_by_choice
{
public:
    report_by_choice(index_structure<Medical_status, long long, int>* report_list_m, 
    index_structure<Registration, double, int>* report_list_r, 
    index_structure<appointment, long long, int>* report_list_t0, 
    index_structure<appointment, long long, int>* report_list_t1, 
    index_structure<appointment, long long, int>* report_list_t2){
        rl_m=report_list_m;
        rl_r=report_list_r;
        rl_t0=report_list_t0;
        rl_t1=report_list_t1;
        rl_t2=report_list_t2;
    };
    
    void choice();
    void print_by_m();
    void print_by_r();
    void print_by_t();
    void print_by_hour();
    int getlength(vector<Medical_status*> report_list);
    int getlength(vector<Registration*> report_list);
    int getlength(vector<appointment*> report_list);
    vector<person<int>*>* merge(index_structure<person<int>, long long, int>* rl,vector<person<int>*>* rl_1,vector<person<int>*>* rl_2,vector<person<int>*>* rl_3,vector<person<int>*>* rl_4,vector<person<int>*>* rl_5);

private:
    index_structure<Medical_status, long long, int>* rl_m;
    index_structure<Registration, double, int>* rl_r;
    index_structure<appointment, long long, int>* rl_t0;
    index_structure<appointment, long long, int>* rl_t1;
    index_structure<appointment, long long, int>* rl_t2;

};


class Weeklyreport
{
public:
    double current_time;
    Weeklyreport(index_structure<person<int>, long long, int>* report_list, double Time){
        rl = report_list;
        current_time = Time;
        rl_0 = merge(rl,rl->b_retrieve_I(1),rl->b_retrieve_I(2),rl->b_retrieve_I(3),rl->b_retrieve_I(4),rl->b_retrieve_I(5));
    }

    int getlength(vector<person<int>*> report_list);
    double waitingtime(double a, double b);
    void Save(int week,string file_head, bool is_sorted);
    void Sort(int week);
    vector<person<int>*>* merge(index_structure<person<int>, long long, int>* rl,vector<person<int>*>* rl_1,vector<person<int>*>* rl_2,vector<person<int>*>* rl_3,vector<person<int>*>* rl_4,vector<person<int>*>* rl_5);

private:
    index_structure<person<int>, long long, int>* rl;   
    vector<person<int>*>* rl_0;
};



class Medical_status{
public:    
    Medical_status(){};
    Medical_status(person<int>* a_person)
    {
        risk_id=a_person->risk_id;
        risk=a_person->risk;
        id=a_person->pri_key;
        pri_key=id;
        secondary_key=risk_id;
        dataptr.first=NULL;
        dataptr_old.first=NULL;
    };
    int risk_id;
    string risk;   //0,1,2,3
    long long id;
   //use in index structure
    long long pri_key;  //pimary key is the person's id
    int secondary_key;  //secondary key is the  risk id
    pair<block<Medical_status, long long, int>*, int> dataptr;
    pair<block<Medical_status, long long, int>*, int> dataptr_old;
    void prettyprint(){
    cout << "(Loc:" << dataptr.second << " ,Pri:" << pri_key << " ,Sec:" << secondary_key << ") ";
    }

};

class Registration{
public:
    Registration(){};
    Registration(person<int>* a_person)
    {
        time=a_person->Time[0];
        Day = a_person->show_day()[0];
        local_id=a_person->local_id;
        hospital_id=a_person->hospital_id;
        hosp_num=a_person->hosp_num;
        id=a_person->pri_key;
        pri_key=time;
        secondary_key= (int)(time/24);
        dataptr.first=NULL;
        dataptr_old.first=NULL;
    };
    double time;
    int Day;  
    int local_id;   
    int* hospital_id;       //Array with index for the hospital，即hopsital's ranking list 
    int hosp_num;           //Initailized when the person is created
    long long id;
    //use in index structure
    double pri_key;     //pri_key is the registration hour
    int secondary_key;  //secondary key is the registration date
    pair<block<Registration,double, int>*, int> dataptr;
    pair<block<Registration,double, int>*, int> dataptr_old;
    void prettyprint(){
    cout << "(Loc:" << dataptr.second << " ,Pri:" << pri_key << " ,Sec:" << secondary_key << ") ";
    };
    
};

#endif