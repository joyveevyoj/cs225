
#include "head.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template <class T, class G, class H>  block<T, G, H>::block(int mainsize, int oversize)
{
    mainb_size=mainsize;
    overb_size=oversize;
    tombptr=new T(-100,-100);//测试用的构造函数
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

template <class T, class G, class H>  T* block< T,  G,  H>::bp_retrieve(G pri_key){

//mainblock[0]=tombptr;//测试用
//linear search in the overflow block

    for (int i=0; i<overflowblock.size(); i++){
        if(overflowblock[i]->pri_key==pri_key)
        {
            return overflowblock[i];
        }//relation里面需要写get_prikey函数
    }

    //binary search in the main block
    int low=0, high=mainblock.size()-1, mid;
    int m=0;//m is the counter that makes sure the while loop do not go forever
    if(mainblock.size()>0)
    {
        while(low<=high&&m<=mainblock.size())
        {
            mid=(low+high)/2;
            while (mainblock[mid] == tombptr&& mid<=high)
            {
                mid++;
            }//if the mid is tombptr, search backward, until find one non-tomb element完了死循环了

            if(mid==high+1){
                mid=(low+high)/2;
                while (mainblock[mid]==tombptr&& mid>=low)
                {
                    mid--;
                }

            }//if cannot find non-tomb element in right half, try find one on left side, as the block will be reorganized when too much tomb num, we must be able to find such one
            if(mid!=low-1&&mainblock[mid]!=tombptr && mainblock[mid]->pri_key==pri_key)
            {
                return mainblock[mid];
            }
            else if (mid!=low-1&&mainblock[mid]!=tombptr && mainblock[mid]->pri_key>pri_key)
            {
                high=mid-1;
            }
            else if (mid!=low-1&&mainblock[mid]!=tombptr && mainblock[mid]->pri_key<pri_key)
            {
                low=mid+1;
            }
            m++;
        }
    }


    return NULL;//if not in block pair, return NULL
}//测试tombptr,应该好了


template <class T, class G, class H>  pair<block<T,G,H>*, int> block< T,  G,  H>::insert( G pri_key, T* tuple)
{

    pair<block<T,G,H>*, int> dataptr;
    dataptr.first = this;
    overflowblock.push_back(tuple);
    dataptr.second = overflowblock.size() - 1 + mainb_size;
    tuple->dataptr = dataptr;
    if(overflowblock.size() >= overb_size)
    {
        sort();/*dataptr=bp_retrieve(pri_key)->dataptr;*/
    }
    if(mainblock.size() >= mainb_size * h_fillfactor)
    {
        split();
    }
    if(bp_retrieve(pri_key) != NULL)
    {
        dataptr=bp_retrieve(pri_key)->dataptr;
    }
    else
    {
        dataptr=r_sibling->bp_retrieve(pri_key)->dataptr;
    }
    tuple->dataptr=dataptr;
    tuple->dataptr_old=dataptr;
    return dataptr;
}


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
    if(mainblock.size()+overflowblock.size()-tomb_num<(mainb_size)*l_fillfactor)
    {
        sort();
        merge();
    }
    return dataptr;//old position in the b tree
}//merge以外的部分好了

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
}//测试正确


template <class T, class G, class H>  void block< T,  G,  H>::b_delete(pair<block<T,G,H>*, int> dataptr)
{
    //if the tuple is in mainblock, mark it with tombptr
    int index;
    if(dataptr.second <mainb_size)
    {
        index=dataptr.second;
        mainblock[index]=tombptr;
        tomb_num++;
    }
    //if the tuple is in overflow block, directly delete it
    else{
    index=dataptr.second-mainb_size;
    overflowblock.erase(overflowblock.begin()+index);
    }
    if(mainblock.size()+overflowblock.size()-tomb_num<(mainb_size)*l_fillfactor){
        sort();
        merge();
    //b 树里相应的应该已经删掉了
    //b+ 树里的操作在merge里进行了
    }

    return;
}//测试基本正确


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
    //换个写法，把所有非tombptr放到临时的，再全部放入新的
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
    //update all the datapointer
    for(int i = 0; i < mainblock.size(); i++)
    {
        (mainblock[i]->dataptr).first=this;
        (mainblock[i]->dataptr).second = i;//relation类里面需要加dataptr成员
        if(mainblock[i]->dataptr_old.first != NULL)
        {
            a_btree->b_update(mainblock[i]->secondary_key, mainblock[i]->dataptr_old, mainblock[i]->dataptr);//新加，未测试
            mainblock[i]->dataptr_old = mainblock[i]->dataptr;
        }
    }

    //到b树里去更新 b.update();

}
//sort函数测试基本没问题


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
        (mainblock[i]->dataptr).second=i;//relation类里面需要加dataptr成员
        if(mainblock[i]->dataptr_old.first!=NULL)
        {
            a_btree->b_update(mainblock[i]->secondary_key, mainblock[i]->dataptr_old, mainblock[i]->dataptr);//新加，未测试
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
    //到b树里去更新 b.update();
    //update the seperate key in b+ tree
    G new_seperate=mainblock[mainblock.size()-1]->pri_key;//relation 新加pri_key 成员
    //把 new_seperatekey放到b+树里面 b+->new_sepkey(new_seperate);*/
    a_Bptree->insert(new_seperate,new_block);
    new_block->r_seperate=r_seperate;
    r_seperate=new_seperate;
    new_block->l_seperate=new_seperate;

}//split()测试暂时没什么问题？




template <class T, class G, class H>  void block< T,  G,  H>::merge()
{
    //此block所在的leaf
    Bpnode<T,G,H>* leaf = a_Bptree->block_retrieve(this);
    //此block在此leaf的index
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
        l_validnum=l_sibling->mainblock.size()+l_sibling->overflowblock.size()-l_sibling->tomb_num;
    }
    int validnum=mainblock.size()+overflowblock.size()-tomb_num;
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
            (mainblock[i]->dataptr).second=i;//relation类里面需要加dataptr成员
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
        //到b树里去更新 b.update();
        //update the seperate key in b+ tree
        G new_seperate=mainblock[mainblock.size()-1]->pri_key;//relation 新加pri_key 成员变量
        //把 new_seperatekey放到b+树里面 b+->new_sepkey(new_seperate);
        a_Bptree->delete1(r_seperate, new_seperate);//old seperate 需要在block里存一下
        r_seperate=new_seperate;
        r_sibling->l_seperate=new_seperate;//新加未测试
        return;

    }

    //subcase, if redistribution with the left sibling
    if(l_validnum != -1 && l_validnum + validnum > 2*l_fillfactor*(mainb_size) && block_index != 0)
    {
        l_sibling->sort();
        int total=validnum+l_validnum;
        int mid=total/2;
        for(int i=l_validnum-1; i>=mid; i--)
        {
            mainblock.insert(mainblock.begin(), l_sibling->mainblock[i]);
        }
        l_sibling->mainblock.erase(l_sibling->mainblock.begin()+mid, l_sibling->mainblock.begin()+l_validnum);
        //change in b
        //change all dataptr
        for(int i=0; i<mainblock.size(); i++)
        {
            (mainblock[i]->dataptr).first=this;
            (mainblock[i]->dataptr).second=i;//relation类里面需要加dataptr成员
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
        //到b树里去更新 b.update();
        //update the seperate key in b+ tree
        G new_seperate=l_sibling->mainblock[mainblock.size()-1]->pri_key;//relation 新加pri_key 成员变量
        //把 new_seperatekey放到b+树里面 b+->new_sepkey(new_seperate);
        a_Bptree->delete1(l_seperate, new_seperate);//新写未测试
        l_seperate=new_seperate;//新写未测试
        l_sibling->r_seperate=new_seperate;//新写未测试
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
        //删掉右边的sibling block a_maindata->blocklist.删除block_index+1
        block<T,G,H>* old_rsib=r_sibling;
        r_sibling=r_sibling->r_sibling;
        if(r_sibling != NULL)
        {   
            r_sibling->l_sibling=this;
        }
        //b树去更新
        for(int i=0; i<mainblock.size(); i++)
        {
            (mainblock[i]->dataptr).first=this;
            (mainblock[i]->dataptr).second=i;//relation类里面需要加dataptr成员
            if(mainblock[i]->dataptr_old.first!=NULL)
            {
                a_btree->b_update(mainblock[i]->secondary_key, mainblock[i]->dataptr_old, mainblock[i]->dataptr);
                mainblock[i]->dataptr_old= mainblock[i]->dataptr;
            }
        }
        //到b+树里去删掉一个seperate key



        // cout<<"before delete2, r_sibling\n";
        // a_Bptree->prettyprint();
        // cout<<"the seperate key to be deleted is: "<<r_seperate<<"\n";


        a_Bptree->delete2(r_seperate,old_rsib);//新写未测试
        if(r_sibling!=NULL)
        {
            r_seperate=old_rsib->r_seperate;//新写未测试
        }
        else
        {
            r_seperate=-999999;
        }
        // cout<<"after delete2, r_sibling\n";
        // a_Bptree->prettyprint();//测试用
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
        //删掉this block 
        l_sibling->r_sibling=this->r_sibling;
        if(r_sibling!=NULL)
        {
            r_sibling->l_sibling=l_sibling;
        }

        //b树去更新
        for(int i=0; i<l_sibling->mainblock.size(); i++)
        {
            (l_sibling->mainblock[i]->dataptr).first=l_sibling;
            (l_sibling->mainblock[i]->dataptr).second=i;//relation类里面需要加dataptr成员
            if(l_sibling->mainblock[i]->dataptr_old.first!=NULL)
            {
                a_btree->b_update(l_sibling->mainblock[i]->secondary_key, l_sibling->mainblock[i]->dataptr_old, l_sibling->mainblock[i]->dataptr);
                l_sibling->mainblock[i]->dataptr_old= l_sibling->mainblock[i]->dataptr;
            }
        }
        //到b树里去更新 b.update();
        //到b+树里去删掉一个seperate key, 和1个blockptr

        a_Bptree->delete2(l_seperate, this);//新写未测试
        if(l_sibling!=NULL)
        {
            l_sibling->r_seperate=r_seperate;
        }//新写未测试
        else
        {
            l_sibling->r_seperate=-999999;
        }
        return;
    }
    else
    {
        return;
    }//测试正确

}//还差删除一个block的情况没有测试


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
                cout<<"A tombstone\n";
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
/*
int main(){
    relation* tuple_1 = new relation(5);
    relation* tuple_2 = new relation(4);
    relation* tuple_3= new relation(2);
    relation* tuple_4=new relation(3);
    relation* tuple_5=new relation(8);
    relation* tuple_6=new relation(9);
    
    block<relation,int,int> a_block(5,1);//测试构造函数
    a_block.insert(5,tuple_1);
    //a_block.prettyprint();
    cout<<"in the blocklist\n";
    for(block<relation,int,int>* ptr=&a_block; ptr!=NULL; ptr=ptr->r_sibling){
        ptr->prettyprint();
    }
    a_block.insert(4,tuple_2);
    //a_block.prettyprint();
     cout<<"in the blocklist\n";
    for(block<relation,int,int>* ptr=&a_block; ptr!=NULL; ptr=ptr->r_sibling){
        ptr->prettyprint();
    }
    a_block.insert(3,tuple_4);
     cout<<"in the blocklist\n";
    for(block<relation,int,int>* ptr=&a_block; ptr!=NULL; ptr=ptr->r_sibling){
        ptr->prettyprint();
    }
    a_block.insert(8,tuple_5);
     cout<<"in the blocklist\n";
    for(block<relation,int,int>* ptr=&a_block; ptr!=NULL; ptr=ptr->r_sibling){
        ptr->prettyprint();
    }
    //测试merge了
   a_block.b_delete(tuple_4->dataptr);
   cout<<"in the blocklist\n";
   for(block<relation,int,int>* ptr=&a_block; ptr!=NULL; ptr=ptr->r_sibling){
        ptr->prettyprint();
    }
   //此处测试无法和前面同时进行
    //测试sort()push到main的部分
    //测试retrieve
    /*
   a_block.bp_retrieve(4)->prettyprint();
     a_block.bp_retrieve(5)->prettyprint();
     a_block.bp_retrieve(2)->prettyprint();
    cout<<"from b_retrieve\n";
    a_block.b_retrieve(tuple_1->dataptr)->prettyprint();
    a_block.b_retrieve(tuple_2->dataptr)->prettyprint();
    a_block.b_retrieve(tuple_3->dataptr)->prettyprint();*/
    
    //测试delete
    /*
    a_block.bp_retrieve(5);
    a_block.b_delete(tuple_1->dataptr);
    a_block.prettyprint();
     a_block.b_delete(tuple_3->dataptr);
      a_block.prettyprint();*/
//测试split()
//测试一组大一点的数据

    
/*}*/










