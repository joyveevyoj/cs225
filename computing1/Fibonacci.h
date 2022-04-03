#ifndef Fibonacci_h
#define Fibonacci_h
#include<cmath>
#include<vector>
template<class T> class Fb_heap
{
private :
    struct Fb_node
    {
        T dataitem ;
        struct Fb_node *parent;
        struct Fb_node *child;
        struct Fb_node *left;
        struct Fb_node *right;
        bool mark;
        int degree;
        Fb_node() : dataitem(0), parent(nullptr), child(nullptr), left(this), right(this), mark(false), degree(0){} ;
        Fb_node(T _data,Fb_node* _parent,Fb_node* _child,Fb_node* _left, Fb_node* _right,bool _mark,int _degree) : dataitem(_data), parent(_parent), child(_child), left(_left), right(_right), mark(_mark), degree(_degree){} ;
    };
private :
    void listadd(Fb_node * &r, Fb_node * ptr);// add node ptr to the right of node r
    int Dn() {return (log2(numnodes) + 1);} 
    void deleteln(Fb_node *p) ; //delete the node in list
    void linkheap(Fb_node * p1, Fb_node * p2); //link p1 to the right of p2
    void cutheap(Fb_node * p1, Fb_node * p2) ;
    void markcut(Fb_node* p);
    Fb_node* minnode;
    int numnodes; 
    //Fb_node* search(Fb_node* nn,T k) ;
public:
    Fb_heap() : minnode(nullptr), numnodes(0) {} ;
    void insert(T k);
    T delete_min(void);
    void consolidate(void);
    void decrease(Fb_node *dnode,T k);
    void deletenode(Fb_node *p) ;
    //void update(T k, T nk) ; //  update dataitem k into nk

};

#endif


