#include "Fibonacci.h"
#include<vector>
using namespace std ;

template<class T> void Fb_heap<T>::listadd(Fb_node * &r, Fb_node * ptr){
    if(r == nullptr){
        r= ptr ;
        r->right = r ;
        r->left = r ;
    }
    else{
        Fb_node* rr=r ;
        ptr->right=rr->right ;
        ptr->left = rr ;
        rr->right->left=ptr ;
        rr->right= ptr ;
    }

}
template<class T> void Fb_heap<T>::deleteln(Fb_node* p){
    p->right->left = p->left ;
    p->left->right = p->right; //delete this node in the list
}

template<class T> void Fb_heap<T>::linkheap(Fb_node * p1, Fb_node * p2){
    deleteln(p1);
    linkheap(p2->child,p1);
    p1->mark = false ;
    p2->degree++;
}

template<class T> void Fb_heap<T>::cutheap(Fb_node * p1, Fb_node * p2){ // p2 is the parent of p1
    deleteln(p1) ;
    p2->degree-- ;
    listadd(minnode,p1);
    p1->mark = false ;
    p1->parent = nullptr ;
}

template<class T> void Fb_heap<T>::markcut(Fb_node* p){
    Fb_node* pnode = p->parent ;
    if (pnode != nullptr){
        if(p->mark == false){
            p->mark = true ;
        }
        else{
            cut(p,pnode) ;
            cutmark (pnode) ;
        }
     }
}



template<class T> void Fb_heap<T>::insert(T k){
    Fb_node * newnd = new Fb_node ;
    newnd->dataitem= k ;
    listadd(minnode,newnd) ;
    if(k< minnode->dataitem){
        minnode= newnd ;
    }
    numnodes++ ; 
}

template<class T> void Fb_heap<T>:: consolidate(void){
    vector<Fb_node*> Tl(Dn(),nullptr) ;
    int dgr ;
    Fb_node *a1, *b1, *c1 ;
    Fb_node *sentry = new Fb_node ;
    listadd( minnode->left,sentry);
    for(a1=minnode; a1!=sentry ; a1=b1){ //check every list node to ensure no two nodes are in same degree;
        b1=a1->right ;
        dgr=a1->degree ;
        while(Tl[dgr]!= nullptr){  // if two nodes have the same degree, add the bigger one into the tree of smaller one;
            c1 = Tl[dgr] ;
            if(c1->dataitem < a1->dataitem){
                Fb_node* temp= c1 ;
                c1 = a1 ;
                a1 = temp ;
            }
            linkheap(c1,a1) ;
            Tl[dgr]= nullptr ;
            dgr++ ;
        }
        Tl[dgr]= a1 ;
    }
    /*then we need to find the minnode */
    minnode = nullptr ;
    for (int i =0; i<Tl.size(); i++){
        if(Tl[i]!= nullptr){
            if(minnode==nullptr){
                listadd(minnode,Tl[i]) ; 
                minnode = Tl[i] ;
            }
            else{
                listadd(minnode,Tl[i]) ;
                if (Tl[i]->dataitem < minnode->dataitem) {
                    minnode=Tl[i] ;
                }
            }
        }
    }
    delete sentry ;
}

template<class T> T Fb_heap<T>:: delete_min(){
    Fb_node* p=minnode ;
    if(p==nullptr){
        return 0 ; // 或者报错
    } 
    T rtval = p->dataitem ;
    if(p->child){ //if p has child, put his children into root list
        Fb_node * x= p->child ;
        Fb_node * y= x.right ;
        for(int i=0;i<p->degree;i++){
            listadd(minnode,x);
            x->parent=nullptr ;
            x=y ;
            y=y->right ;
        }
    }
    if(p==p->right){ //if root list has only one node
        minnode = nullptr ;
    }
    else{
        minnode = p->right ;
        deleteln(p) ;
        consolidate();
    }
    numnodes-- ;
    delete p ;
    return rtval ;
}

template<class T> void Fb_heap<T>:: decrease(Fb_node *dnode,T k){
    if(dnode->dataitem <= k){
        return ;
    }
    dnode->dataitem = k ;
    Fb_node* prtnode= dnode->parent ;
    if((prtnode != nullptr)&&(prtnode->dataitem > dnode->dataitem)){
        cutheap(dnode,prtnode) ;
        markcut(prtnode);
    } 
    if(dnode->dataitem < minnode->dataitem){
        minnode= dnode ;
    }
}

template<class T> void Fb_heap<T>::deletenode(Fb_node *p){
    decrease(p,(minnode->dataitem)-1) ;
    delete_min();
}

/* template<class T> Fb_node* Fb_heap<T>::search(Fb_node* nn,T k){
    if (nn == nullptr){
        return nullptr ;
    }
    Fb_node * x=nn ;
    Fb_node * y ;
    do{
        if(x->dataitem== k){
            return x ;
        }
        else if(k> x->dataitem){
            y = search(x->child,k);
            if(y != nullptr){
                return y ;
            }
        }
        x=x->right ;
    }while(x!= nn) ;

    return nullptr ;
}

template<class T> void Fb_heap<T>::update(T k, T nk){
    Fb_node* x= search(minnode, k) ;
    if(x){
        
    }
    
} */


