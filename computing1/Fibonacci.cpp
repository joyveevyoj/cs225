
template<class T> class Fb_heap{
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
        Fb_node() : dataitem(0), parent(nullptr), child(nullptr), left(this), right(this), mark(false), degree(0){}
        Fb_node(T _data,Fb_node* _parent,Fb_node* _child,Fb_node* _left, Fb_node* _right,bool _mark,int _degree) : dataitem(_data), parent(_parent), child(_child), left(_left), right(_right), mark(_mark), degree(_degree){}
    };
private :
    Fb_node* minnode;
    int numnodes; 
public:
    void insert(Fb_node *newnode);
    void delete_min(void);
    void heapunion(Fb_heap* heap1);
    void consolidate(void);
    void decrease(Fb_node *dnode,T k);

};


