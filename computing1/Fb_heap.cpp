
#ifndef _FB_HEAP_CPP_
#define _FB_HEAP_CPP_
#include "head.h"
#include <iomanip>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
using namespace std;

/*
this file is the code of fibonacci heap, I wrote this code followed by the idea of blog in https://www.cnblogs.com/skywang12345/p/3659069.html
I define two class Fb_node and Fb_heap; In the Fb_heap, you can five functions in public , insert , deletemin, remove, update and getnum
in the private scope, it has many functions to help achieve these five public functions.
And I wrote comments for each of functions to help read and understand and prove my understanding of the ideas in blog I referenced.
*/


template <class T> void Fb_heap<T>::rmlistNode(Fb_node<T> *ptr){
    ptr->left->right = ptr->right;
    ptr->right->left = ptr->left;  // remove from list
}

template <class T> void Fb_heap<T>::addlistNode(Fb_node<T> *ptr, Fb_node<T> *root){ // add the ptr to the left of the root in the list of
    ptr->left        = root->left;
    root->left->right = ptr;
    ptr->right       = root;
    root->left        = ptr;
}

template <class T> void Fb_heap<T>::insert(Fb_node<T> *ptr){ // insert a new node into the heap
    if (numnodes!=0){
        addlistNode(ptr, min);
        if (min->key >ptr->key){   //if heap is not empty, add ptr to left of minnode
            min = ptr;
        }
    }else{
        min = ptr;  // if heap is empty, the minnodes is ptr
    }
    numnodes++;
}

template <class T> void Fb_heap<T>::insert(person<int>* patient){ // insert a patient into the heap
    Fb_node<T> *newnode;
    newnode = new Fb_node<T>(patient); // creat a newnode for patient
    if (newnode == NULL){
        return ;           // if creat the newnode fails, retutn or insert newnode
    }
    insert(newnode);
}

template <class T> Fb_node<T>* Fb_heap<T>::getminnode(){ // remove and get the tree of the minnode
    Fb_node<T> *rtptr = min; // let return pointer to be  minnode 
    if (rtptr == rtptr->right ){  // if heap only has one node in rootlist, let min equal null
        min = NULL;
    }else{
        rmlistNode(rtptr);     //extract tree of minnode 
        min = rtptr->right;
    }
    rtptr->left = rtptr ; 
    rtptr->right = rtptr; // update left and right
    return rtptr; // return minnode
}

template <class T> void Fb_heap<T>::nodelink(Fb_node<T>* ptr, Fb_node<T>* r){ // link the node ptr to the root ,and become r's child
    rmlistNode(ptr); // first remove this node in the list
    if (r->child != NULL){ // if root has child, add this node into child list
        addlistNode(ptr, r->child);
    }else{
        r->child = ptr;  //or let this node become child
    }
    ptr->parent = r;
    ptr->mark = false;
    r->degree =r->degree+1 ; // handle some change of the new child
}

template <class T> void Fb_heap<T>::renewdrg(Fb_node<T> *tparent, int degree){
    tparent->degree = tparent->degree - degree;
    if (tparent-> parent != NULL){
        renewdrg(tparent->parent, degree); // use recursion to renew all parent and parent's parent's degree
    }
}

template <class T> void Fb_heap<T>::cut(Fb_node<T> *node, Fb_node<T> *parent){// cut the node from list of parent's child and insert into root list
    rmlistNode(node);
    renewdrg(parent, node->degree);
    if (node == node->right){
        parent->child = NULL;        // if it is the only child
    }else{
        parent->child = node->right;  
    }
    node->parent = NULL;
    node->left = node->right = node;
    node->mark = false;   //update node's member and add to the root list
    addlistNode(node, min);
}

template <class T> void Fb_heap<T>::markcut(Fb_node<T> *ptr){ // check the delete node's parent is marked or not, if it is marked, this node should also be cut
    Fb_node<T> *thisparent = ptr->parent;
    if (thisparent != NULL){
        if (ptr->mark == false){
            ptr->mark = true;     // if this node is not marked ,just mark it
        }else{
            cut(ptr, thisparent); // else cut this node and check the mark of its parent
            markcut(thisparent);
        }
    }
}

template <class T> void Fb_heap<T>::consolidate(){  // this part I follow the idea of blog on csdn
    int i, d;
    buildlsarea();// create area for degree list
    int tempd = maxdrg + 1;
    for (i = 0; i < tempd; i++){
        drglist[i] = NULL;
    }                            // initialize degree list
    Fb_node<T> *a, *b, *tmp;
    while (min != NULL){          // extract everynode in the rootlist and merge the node with same degree
        a = getminnode();         //extract minnode tree
        d = a->degree;                    
        while (drglist[d] != NULL){ // determine whether the degree has existed in the degree list
            b = drglist[d];                // b is the node whose degree is same as a
            if (a->key > b->key){        // ensure a's key is smaller than b
                swap(a, b);
            }
            nodelink(b, a);    // link b into the root a 
            drglist[d] = NULL;
            d++;
        }
        drglist[d] = a; // store a into the new degree place
    }
    min = NULL;
    for (i=0; i<tempd; i++){ // rebuild the heap, let all node tree in degree list add back to the heap
        if (drglist[i] != NULL){
            if (min != NULL){ // add node tree into heap
                addlistNode(drglist[i], min); 
                if (min->key >(drglist[i])->key){  // renew minnode if find smaller
                    min = drglist[i];
                }
            } else {
                    min = drglist[i];  // if heap is empty, just add it into heap
            }
        }
    }
}

template <class T> person<int>* Fb_heap<T>::delete_min(){
    if (min==NULL){
        return NULL;
    }
    Fb_node<T> *tempchild = NULL;
    Fb_node<T> *p = min;
    person<int>* rtval= min->patient ;
    while (p->child != NULL){   //add all the child of minnode into the root list
        tempchild = p->child;
        rmlistNode(tempchild); // remove this temporary child
        if (tempchild->right != tempchild){ // move to the next child
            p->child = tempchild->right;
        }else{
            p->child = NULL;    // if all children removed, child become null
        }
        addlistNode(tempchild, min);  // add this child to root list
        tempchild->parent = NULL;   // cancel its parent
    }
    rmlistNode(p);  // delete minnode from list
    if (p->right == p){
        min = NULL;    // if p is the last node, just become null
    }else{
        min = p->right;
        consolidate();  // consolidate for the heap
    }
    numnodes--;  
    delete p;    //deduct numnodes and delete temporary p
    return rtval ; // return the most porioriest patient's pointer
}




template <class T> Fb_node<T>* Fb_heap<T>::search(Fb_node<T> *root, person<int>* spatient){ //search the patient starting from the root
    Fb_node<T> *temp = root;    
    Fb_node<T> *ptr = NULL;    // store the ptr of the search node
    int tempkey = spatient->return_key() ; // read the keyvalue
    string tempid= spatient->show_id();  // to find the patient ,we need compare id
    if (root==NULL){
        return root;  //not find
    }
    do{ // use while and recursion to loop through all the node in the tree
        if ((temp->key == tempkey) &&(temp->patient->show_id()==tempid)){ //both key and id are the same, we can know we find the patient
            ptr = temp;
            break;
        }
        else{
            if ((ptr = search(temp->child, spatient)) != NULL)   // use recursion to search all the nodes in the heap tree
                break;
        }
        temp = temp->right; // to loop though all the node in the list
    } while (temp != root);
    return ptr;
}

template <class T> Fb_node<T>* Fb_heap<T>::search(person<int>* patient){
    if (min==NULL){
        return NULL;
    }   // if min is null, we need not search
    Fb_node<T>* rtnode= search(min, patient);  // search all the node from root min
    return rtnode ;
}

template <class T> void Fb_heap<T>::remove(Fb_node<T> *ptr){
    T smallval = min->key-2;
    decrease(ptr, smallval);  // if we want to remove one node, we just need decrease the value to a smallest one and deletemin
    delete_min();
}

template <class T> void Fb_heap<T>::remove(person<int>* oldpatient){
    if (min==NULL){
        return ;
    }                   // if min is null, need not search
    Fb_node<T> *ptr;
    ptr = search(oldpatient);
    if (ptr==NULL){ return ;}   // if not find this node, just return    
    remove(ptr);   // overloade to remove this node
}


template <class T> void Fb_heap<T>::decrease(Fb_node<T> *ptr, T newkey){
    Fb_node<T> *tempparent;
    if (min==NULL ||ptr==NULL||(newkey>=ptr->key)){ // ensure some initial condition is not wrong
        return ;
    }
    ptr->key = newkey;
    tempparent = ptr->parent;
    if (tempparent!=NULL && ptr->key < tempparent->key){ // if key is smaller than his parent, cut this node into root list
        cut(ptr, tempparent);
        markcut(tempparent);  // cut the node and check its parent's mark
    }
    if (ptr->key < min->key){ // check the minnode whether need update
        min = ptr;
    }
}

// this increase operation is followed the idea of the blog of csdn
template <class T> void Fb_heap<T>::increase(Fb_node<T> *ptr, T newkey){ // increase the value into the new key
    if (min==NULL ||ptr==NULL || (ptr->key >= newkey)){ return;} // avoid some initial wrong
    Fb_node<T> *tempchild, *tempparent;
    while (ptr->child != NULL){  // similar as decrease, move all the children into root list
        tempchild = ptr->child;
        rmlistNode(tempchild);              
        if (tempchild->right != tempchild){
            ptr->child = tempchild->right;  //let child become the next child in the list
        }else{                // or it has only one child in the list
            ptr->child = NULL;
        }
        addlistNode(tempchild, min);       // add this temporary child into the root list
        tempchild->parent = NULL;          // cancel its parent
    }
    ptr->key = newkey;
    ptr->degree = 0;       // update newkey and degree
    Fb_node<T> *tempright;
    tempparent = ptr->parent;
    if(tempparent != NULL){   // if this node is not in the root list
        cut(ptr, tempparent); // cut this node into the root list and check the mark of his parent
        markcut(tempparent);
    }
    else if(min == ptr){  // if this node is the min node , compare all the node in the root list to find the new minnode
        tempright = ptr->right;
        while(tempright != ptr){  // loop through all the node in the root list
            if(tempright->key<ptr->key ){
                min = tempright;  //update the minnode
            }
            tempright = tempright->right;
        }
    }
}


template <class T> void Fb_heap<T>::update(Fb_node<T> *ptr, T newkey){  // update the key into newkey
    if(ptr->key > newkey){    // select we should increase or decrease
        decrease(ptr, newkey);
    }
    else if(ptr->key < newkey){
        increase(ptr, newkey);
    }else{return ;}
}

template <class T> void Fb_heap<T>::update(person<int>*newpatient, person<int>* oldpatient){ // update the oldpatient in the heap into the new patient
    int newkey = newpatient->return_key() ;
    Fb_node<T> *ptr;
    ptr = search(oldpatient);  //first find the patient and then update the patient
    if (ptr!=NULL){
        update(ptr, newkey);
        ptr->patient=newpatient ;
    }
}



#endif