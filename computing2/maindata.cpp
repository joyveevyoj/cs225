
#include "head.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
//constructor for block initialization
template <class T, class G, class H>  block<T, G, H>::block(int mainsize, int oversize)
{
    mainb_size=mainsize;
    overb_size=oversize;
    tombptr=new T;
    tomb_num=0;
    l_seperate=-999999;
    r_seperate=-999999;
    h_fillfactor=0.7;
    l_fillfactor=0.3;
    l_sibling=NULL;
    r_sibling=NULL;
    a_Bptree = new Bptree<T,G,H>;
    a_btree = new btree<T,G,H>;
}
//retrieve from bp tree,input a pri_key, return the tuple pointer
template <class T, class G, class H>  T* block< T,  G,  H>::bp_retrieve(G pri_key){

//linear search in the overflow block

    for (int i=0; i<overflowblock.size(); i++){
        if(overflowblock[i]->pri_key == pri_key)
        {
            return overflowblock[i];
        }
    }

    //binary search in the main block
    int low = 0, high = mainblock.size()-1, mid;
    int m = 0;//m is the counter that makes sure the while loop do not go forever
    if(mainblock.size() > 0)
    {
        while(low <= high && m <= mainblock.size() )
        {

            mid = (low + high) / 2;

            //if the mid is tombptr, search backward, until find one non-tomb element
            while (mainblock[mid] == tombptr && mid <= high)
            {
                mid++;
            }

            //if cannot find non-tomb element in right half, try find one on left side, as the block will be reorganized when too much tomb num, we must be able to find such one
            if(mid == high + 1)
            {
                mid = (low + high) / 2;
                while (mainblock[mid]==tombptr&& mid>=low)
                {
                    mid--;
                }
            }

            if(mid != low-1 && mainblock[mid]!=tombptr && mainblock[mid]->pri_key == pri_key)
            {
                return mainblock[mid];
            }
            else if (mid != low-1 && mainblock[mid] != tombptr && mainblock[mid]->pri_key > pri_key)
            {
                high = mid - 1;
            }
            else if (mid != low-1 && mainblock[mid] != tombptr && mainblock[mid]->pri_key < pri_key)
            {
                low = mid + 1;
            }
            m++;
        }
    }


    return NULL;//if not in block pair, return NULL
}

//insert a new tuple into the block
template <class T, class G, class H>  pair<block<T,G,H>*, int> block< T,  G,  H>::insert( G pri_key, T* tuple)
{
    //first insert into overflow block
    pair<block<T,G,H>*, int> dataptr;
    dataptr.first = this;
    overflowblock.push_back(tuple);
    dataptr.second = overflowblock.size() - 1 + mainb_size;
    tuple->dataptr = dataptr;
    if(overflowblock.size() >= overb_size)
    {
        sort();
    }//call sort fuction if the overflow block is full
    if(mainblock.size() >= mainb_size * h_fillfactor)
    {   sort();
        split();
    }//call split function if the mainblock size is bigger than the high factor
    if(bp_retrieve(pri_key) != NULL)
    {
        dataptr=bp_retrieve(pri_key)->dataptr;
    }
    else
    {
        dataptr=r_sibling->bp_retrieve(pri_key)->dataptr;
    }//update the dataptr after split or sort
    tuple->dataptr=dataptr;
    tuple->dataptr_old=dataptr;
    return dataptr;//return the dataptr for further b tree insert
}

//input a pri_key, delete the corresponding tuple, and return the dataptr 
template <class T, class G, class H>  pair<block<T,G,H>*, int> block< T,  G,  H>::bp_delete(G pri_key)
{
    pair<block<T,G,H>*, int> dataptr;
    dataptr.first=NULL;
    T* find = bp_retrieve(pri_key);
    //if cannot find, the datapointer returned will have a null pointer in first.
    if(find==NULL){
        cout<<"cannot delete\n";
        return dataptr;
    }
    dataptr=find->dataptr;
    int index;
    //if the tuple is in mainblock, mark it with tombptr
    if(find->dataptr.second <mainb_size)
    {
        index=find->dataptr.second;
        mainblock[index]=tombptr;

        tomb_num++;
    }
    //if the tuple is in overflow block, directly delete it
    else
    {
        index=find->dataptr.second-mainb_size;
        overflowblock.erase(overflowblock.begin()+index);
    }
    if(mainblock.size() + overflowblock.size()-tomb_num < (mainb_size)*l_fillfactor)
    {
        sort();
        merge();
    }
    return dataptr;//return old position in the b tree
}
//input a dataptr found from b tree, return the tuple pointer
template <class T, class G, class H>  T* block< T,  G,  H>::b_retrieve(pair<block<T,G,H>*, int> dataptr)
{
    if(dataptr.second<mainb_size)
    {
        return mainblock[dataptr.second];
    }
    else
    {
        return overflowblock[dataptr.second-mainb_size];
    }
}

//input a dataptr found from b tree, delete the corresponding tuple
template <class T, class G, class H>  void block< T,  G,  H>::b_delete(pair<block<T,G,H>*, int> dataptr)
{
    this->prettyprint();
    //if the tuple is in mainblock, mark it with tombptr
    int index;
    if(dataptr.second < mainb_size)
    {
        index = dataptr.second;
        mainblock[index] = tombptr;
        tomb_num++;
    }
    //if the tuple is in overflow block, directly delete it
    else
    {
        index = dataptr.second - mainb_size;
        overflowblock.erase( overflowblock.begin() + index);
        //dataptrs in overblock should update as well
        for(int i = 0; i < overflowblock.size(); i++)
        {
            (overflowblock[i]->dataptr).first = this;
            (overflowblock[i]->dataptr).second = i + mainb_size;
            if(overflowblock[i]->dataptr_old.first != NULL)
            {
                a_btree->b_update(overflowblock[i]->secondary_key, overflowblock[i]->dataptr_old, overflowblock[i]->dataptr);
                overflowblock[i]->dataptr_old = overflowblock[i]->dataptr;
            }
            
        }

    }
    if(mainblock.size()+overflowblock.size()-tomb_num<(mainb_size)*l_fillfactor)
    {
        sort();
        merge();
    }

    

    return;
}


template <class T, class G, class H>  void block< T,  G,  H>::sort()
{
    //push all the members from overflow block into mainblock
    int over_num = overflowblock.size();
    for(int i = 0; i < over_num; i++)
    {
        mainblock.push_back(overflowblock.back());
        overflowblock.pop_back();
    }
    //sort mainblock
    //remove all the tombptr first by putting them into a temperary vector
    vector<T*> temp_main;
    for (int i=0; i<mainblock.size(); i++)
    {
        if(mainblock[i]!=tombptr)
        {
            temp_main.push_back(mainblock[i]);
        }
    }
    mainblock=temp_main;
    tomb_num=0;
    T* k;
    int current;
    for(int i=1; i<mainblock.size(); i++)
    {
        k=mainblock[i];
        current=i-1;
        for(; current>=0&&mainblock[current]->pri_key > k->pri_key;current--)
        {
            mainblock[current+1]=mainblock[current];
        }
        mainblock[current+1]=k;

    }//use inserson sort to sort
    //update all the datapointer and update them in b tree
    for(int i = 0; i < mainblock.size(); i++)
    {
        (mainblock[i]->dataptr).first=this;
        (mainblock[i]->dataptr).second = i;
        if(mainblock[i]->dataptr_old.first != NULL)
        {
            a_btree->b_update(mainblock[i]->secondary_key, mainblock[i]->dataptr_old, mainblock[i]->dataptr);//新加，未测试
            mainblock[i]->dataptr_old = mainblock[i]->dataptr;
        }
    }

   

}



template <class T, class G, class H>  void block< T,  G,  H>::split()
{
    //build a new block
    block<T,G,H>* new_block = new block<T,G,H>(mainb_size, overb_size);
    new_block->a_btree=a_btree;
    new_block->a_Bptree=a_Bptree;
    new_block->l_sibling=this;
    new_block->r_sibling=r_sibling;
    r_sibling=new_block;
    if(new_block->r_sibling!=NULL)
        new_block->r_sibling->l_sibling=new_block;//insert the new block into the doubly linked block list

    //reditribution
    int mid=mainblock.size()/2;
    int mainsize=mainblock.size();
    for(int i=mid; i<mainsize; i++)
    {
        new_block->mainblock.push_back(mainblock[i]); 
    }
    mainblock.erase(mainblock.begin()+mid, mainblock.end());
    //update all the datapointer
    for(int i=0; i<mainblock.size(); i++)
    {
        (mainblock[i]->dataptr).first=this;
        (mainblock[i]->dataptr).second=i;
        if(mainblock[i]->dataptr_old.first!=NULL)
        {
            a_btree->b_update(mainblock[i]->secondary_key, mainblock[i]->dataptr_old, mainblock[i]->dataptr);
            mainblock[i]->dataptr_old= mainblock[i]->dataptr;
        }
    }
    for(int i=0; i<new_block->mainblock.size();i++)
    {
        new_block->mainblock[i]->dataptr.first=new_block;
        new_block->mainblock[i]->dataptr.second=i;
        if(new_block->mainblock[i]->dataptr_old.first!=NULL)
        {
            a_btree->b_update(new_block->mainblock[i]->secondary_key, new_block->mainblock[i]->dataptr_old, new_block->mainblock[i]->dataptr);//新加，未测试
            new_block->mainblock[i]->dataptr_old= new_block->mainblock[i]->dataptr;
        }
    }
    //update in b tree b.update();
    //update the seperate key in b+ tree
    G new_seperate=mainblock[mainblock.size()-1]->pri_key;
    a_Bptree->insert(new_seperate,new_block);
    new_block->r_seperate=r_seperate;
    r_seperate=new_seperate;
    new_block->l_seperate=new_seperate;

}




template <class T, class G, class H>  void block< T,  G,  H>::merge()
{
    //the leaf this block is in
    Bpnode<T,G,H>* leaf = a_Bptree->block_retrieve(this);
    //the index of this block in he leaf
    int block_index = leaf->block_search(this);
    //case1, redistribution
    int r_validnum=-1;
    int l_validnum=-1;
    if(r_sibling!= NULL)
    {
        r_validnum=r_sibling->mainblock.size()+r_sibling->overflowblock.size()-r_sibling->tomb_num;
    }
    if(l_sibling!= NULL)
    {
        l_validnum = l_sibling->mainblock.size() + l_sibling->overflowblock.size() - l_sibling->tomb_num;
    }
    int validnum = mainblock.size() + overflowblock.size() - tomb_num;
    //subcase, if redistribution with the right sibling
    if(r_validnum != -1 && r_validnum + validnum > 2*l_fillfactor*(mainb_size) && block_index != leaf->getkeynum())
    {

        r_sibling->sort();
        int total=validnum+r_validnum;
        int mid=total/2;
        for(int i=0; i<mid-validnum; i++)
        {
            mainblock.push_back(r_sibling->mainblock[i]);
        }
        r_sibling->mainblock.erase(r_sibling->mainblock.begin(), r_sibling->mainblock.begin()+mid-validnum);
        //change in b
        //change all dataptr
        for(int i=0; i<mainblock.size(); i++)
        {
            (mainblock[i]->dataptr).first=this;
            (mainblock[i]->dataptr).second=i;
            if(mainblock[i]->dataptr_old.first!=NULL)
            {
                a_btree->b_update(mainblock[i]->secondary_key, mainblock[i]->dataptr_old, mainblock[i]->dataptr);
                mainblock[i]->dataptr_old= mainblock[i]->dataptr;
            }
        }
        for(int i=0; i<r_sibling->mainblock.size();i++){
            r_sibling->mainblock[i]->dataptr.first=r_sibling;
            r_sibling->mainblock[i]->dataptr.second=i;
            if( r_sibling->mainblock[i]->dataptr_old.first!=NULL)
            {
                a_btree->b_update(r_sibling->mainblock[i]->secondary_key, r_sibling->mainblock[i]->dataptr_old, r_sibling->mainblock[i]->dataptr);
                r_sibling->mainblock[i]->dataptr_old= r_sibling->mainblock[i]->dataptr;
            }
        }
        //update b tree's dataptr
        //update the seperate key in b+ tree
        G new_seperate=mainblock[this->mainblock.size()-1]->pri_key;
        a_Bptree->delete1(r_seperate, new_seperate);
        r_seperate=new_seperate;
        r_sibling->l_seperate=new_seperate;
        return;

    }

    //subcase, if redistribution with the left sibling
    if(l_validnum != -1 && l_validnum + validnum > 2*l_fillfactor*(mainb_size) && block_index != 0)
    {
        l_sibling->sort();
        int total = validnum + l_validnum;
        int mid = total / 2;
        for(int i = l_validnum - 1; i >= mid; i--)
        {
            mainblock.insert(mainblock.begin(), l_sibling->mainblock[i]);
        }
        l_sibling -> mainblock.erase(l_sibling->mainblock.begin()+mid, l_sibling->mainblock.begin()+l_validnum);
        //change in b
        //change all dataptr
        for(int i=0; i<mainblock.size(); i++)
        {
            (mainblock[i]->dataptr).first=this;
            (mainblock[i]->dataptr).second=i;
            if(mainblock[i]->dataptr_old.first!=NULL)
            {
                a_btree->b_update(mainblock[i]->secondary_key, mainblock[i]->dataptr_old, mainblock[i]->dataptr);
                mainblock[i]->dataptr_old= mainblock[i]->dataptr;
            }
        }
        for(int i=0; i<l_sibling->mainblock.size();i++)
        {
            l_sibling->mainblock[i]->dataptr.first=l_sibling;
            l_sibling->mainblock[i]->dataptr.second=i;
            if(l_sibling-> mainblock[i]->dataptr_old.first!=NULL)
            {
                a_btree->b_update( l_sibling->mainblock[i]->secondary_key, l_sibling-> mainblock[i]->dataptr_old,  l_sibling->mainblock[i]->dataptr);
                l_sibling-> mainblock[i]->dataptr_old= l_sibling->mainblock[i]->dataptr;
            }
        }
        //update the dataptr in b tree
        //update the seperate key in b+ tree
        G new_seperate = l_sibling->mainblock[this->l_sibling->mainblock.size()-1]->pri_key;
        a_Bptree->delete1(l_seperate, new_seperate);
        l_seperate=new_seperate;
        l_sibling->r_seperate=new_seperate;
        return;
    }

    //delete one block case
    //if has right sibling, push all the elements in right sibling into this block

    if(r_sibling != NULL && block_index != leaf->getkeynum())
    {  
        //if valid right sibling
        r_sibling->sort();
        for(int i=0; i<r_sibling->mainblock.size(); i++)
        {
            mainblock.push_back(r_sibling->mainblock[i]);//push all in one mainblock    
        }    
        //delete right sibling block 
        block<T,G,H>* old_rsib=r_sibling;
        r_sibling=r_sibling->r_sibling;
        if(r_sibling != NULL)
        {   
            r_sibling->l_sibling=this;
        }
        //update dataptr in b tree
        for(int i=0; i<mainblock.size(); i++)
        {
            (mainblock[i]->dataptr).first=this;
            (mainblock[i]->dataptr).second=i;
            if(mainblock[i]->dataptr_old.first!=NULL)
            {
                a_btree->b_update(mainblock[i]->secondary_key, mainblock[i]->dataptr_old, mainblock[i]->dataptr);
                mainblock[i]->dataptr_old= mainblock[i]->dataptr;
            }
        }
        //delete seperate key in b+ tree

        a_Bptree->delete2(r_seperate,old_rsib);
        if(r_sibling!=NULL)
        {
            r_seperate=old_rsib->r_seperate;
        }
        else
        {
            r_seperate=-999999;
        }
        return;

    }
    else if(l_sibling != NULL && block_index != 0) 
    {
        //if valid left sibling
        l_sibling->sort();
        for(int i=0; i<mainblock.size(); i++)
        {
            l_sibling->mainblock.push_back(mainblock[i]);//else, if only has left sibling push all in lefgtsibling's mainblock    
        }    
        // delete this block 
        l_sibling->r_sibling=this->r_sibling;
        if(r_sibling!=NULL)
        {
            r_sibling->l_sibling=l_sibling;
        }

        //update dataptr in b tree
        for(int i=0; i<l_sibling->mainblock.size(); i++)
        {
            (l_sibling->mainblock[i]->dataptr).first=l_sibling;
            (l_sibling->mainblock[i]->dataptr).second=i;
            if(l_sibling->mainblock[i]->dataptr_old.first!=NULL)
            {
                a_btree->b_update(l_sibling->mainblock[i]->secondary_key, l_sibling->mainblock[i]->dataptr_old, l_sibling->mainblock[i]->dataptr);
                l_sibling->mainblock[i]->dataptr_old= l_sibling->mainblock[i]->dataptr;
            }
        }
       // update b tree's dataptr
        //delete seperate key in b+ key and one block ptr
        a_Bptree->delete2(l_seperate, this);
        if(l_sibling!=NULL)
        {
            l_sibling->r_seperate=r_seperate;
        }
        else
        {
            l_sibling->r_seperate=-999999;
        }
        return;
    }
    else
    {
        return;
    }

}

//pretty print for testing use
template <class T, class G, class H>  void block< T,  G,  H>::prettyprint()
{
    cout<<"Mainblock: ";
    if(mainblock.size() == 0)
        cout << "Empty";
    else
    {
        for(int i=0; i<mainblock.size(); i++)
        {
            if(mainblock[i]!=tombptr)
                mainblock[i]->prettyprint(); 
            else
            {
                cout << "(Loc:" << i << ", Tombstone)";
            }
    }
    }

    cout<<"\nOverflowblock: ";
    if(overflowblock.size() == 0)
        cout << "Empty";
    else
    {
        for(int i=0; i<overflowblock.size(); i++)
            overflowblock[i]->prettyprint();  
    }

    cout<<"\nSize of Mainblock: ";
    cout<<mainblock.size()<<"\n";
    cout<<"Size of Overflowblock: ";
    cout<<overflowblock.size()<<"\n";
    cout<<"Tombstone Num: ";
    cout<<tomb_num<<"\n";
    cout<<"Valid Num in the Block: ";
    cout<<mainblock.size()+overflowblock.size()-tomb_num<<"\n";
    if(l_seperate==-999999)
    {
        cout<<"Left Seperate Key: None\n";
    }
    else{cout<<"Left Seperate Key: "<<(l_seperate)<<"\n";}
    if(r_seperate==-999999){cout<<"Right Seperate Key: None";}
    else{cout<<"Right Seperate Key: "<<(r_seperate);}
    cout<<"\n\n";
}









