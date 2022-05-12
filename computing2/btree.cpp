//#include "btree.h"
#include "head.h"

template<class T,class G,class H> btree<T,G,H>::btree(){	
	root = new bnode();
	root->setLeaf(true);	//start with leaf node
}




template<class T,class G,class H>  bool btree<T,G,H>::search(H se_key){	 //search secondary key from the tree root
	bnode* ptr = root;                     // start from root
	while (ptr!= NULL) {
		int i = 0;
		while (i < ptr->getnumk() && se_key > ptr->getkey(i)){ //find the proper place in the spiderlist
			i++;
		}
		if (i < ptr->getnumk() && se_key == ptr->getkey(i)){   //if find this secondary key, return true
			return true;
		}
		else if(ptr->getleaf()){                                // if it is leaf, it has no child so that not find
			return false;
		}
		ptr = ptr->getchild(i);                                 // go to its child and loop again
	}
	return false;
}

template<class T,class G,class H> void btree<T,G,H>::split(bnode* x, int i){	// split x->child[i]'s spider list
	bnode* z = new bnode();
	bnode* y = x->getchild(i);	//y is left child of x 
	z->setLeaf(y->getleaf());  // z is right child of x
	z->setNumk(mindegree - 1); 
	for (int j = 0; j < mindegree - 1; j++){	
		z->setspider(j, y->getspider(j + mindegree));  // copy mindegree-1 spiders in right child 
	}
	if (y->isleaf == false){
		for (int j = 0; j < mindegree ; j++){
			z->setChild(j, y->getchild(mindegree+j));  // if it is not leaf,copy z's child for right child
		}
	}
	y->setNumk(mindegree - 1);                         //now y's numkey is mindegree-1 
	for (int j = x->getnumk(); j > i; j--){            //in node x,leave the i+1th place for right child node z
		x->setChild(j + 1, x->getchild(j));
	}
	x->setChild(i + 1, z);                            
	for (int j = x->getnumk() - 1; j >= i; j--){
		x->setspider(j + 1, x->getspider(j));        // insert new spider in ith place 
	}
	x->setspider(i, y->getspider(mindegree - 1));    // insert the new spider 
	x->setNumk(x->getnumk() + 1);                    //increment the numkey in this node
}

template<class T,class G,class H> void btree<T,G,H>::insert_root(bnode* r, H k){	    // insert secondary key from given root r
	int i = r->getnumk() - 1;
	if (r->isleaf == true){	                        //we only insert when reach leaf node
		while (i >= 0 && k < r->getkey(i)) {        //find the proper place to insert new spider with secondary key
			r->setspider(i + 1, r->getspider(i));   //move spider backward one place
			i--;                                    //decrement
		}
		spider<T,G,H>* newsp = new spider<T,G,H>;
		newsp->second_key= k ;                      //create a new spider with desired secondary key
		r->setspider(i + 1, newsp);					
		r->setNumk(r->getnumk() + 1);               //increment numkey
	}
	else {
		while (i >= 0 && k < r->getkey(i)){ 		// if it is not leafnode, we should find proper child to continue to find
			i--;
		}                              
		i=i+1;
		if (r->getchild(i)->getnumk()==mindegree*2 - 1) {//if the desired child is full,split it
			split(r, i);
			if (k > r->getkey(i)){
				i++;
			}
		}
		insert_root(r->getchild(i), k);               //use recursion to insert_root from root child[i]            
	}
}

template<class T,class G,class H> void btree<T,G,H>::insert(H se_key){	              //insert the secondary k 
	bnode* r = root;
	if (mindegree*2-1 == r->getnumk()) {		//for the root, the tree grows only with the root 
		bnode* newchild = new bnode();          // if the root is full, create the new child and insert se_key into the new child
		root = newchild;
		newchild->setNumk(0);
		newchild->setLeaf(false);            
		newchild->setChild(0, r);               // create a new child and initialize it
		split(newchild, 0);
		insert_root(newchild, se_key);          //insert k from root new child 
	}else{
		insert_root(r, se_key);                 //insert from root
	}

}

template<class T,class G,class H> void btree<T,G,H>::merge(bnode* x, int i, bnode* y, bnode* z) {		 
	int j = mindegree;   //merge bnode y and z and spider key[i], and x is y z's parent,y is the left child node, z is the right child node
	y->setNumk(mindegree*2 - 1);     // merge the key into y until y has 2mindegree-1
	for (; j <mindegree*2 - 1; j++){
		y->setspider(j, z->getspider(j - mindegree)); // set z's spider into y 
	}
	y->setspider(mindegree-1, x->getspider(i));   // merge key[i] into y
	if (!y->getleaf()){                             // if y is not leaf node, set z's child into proper place
		for (j =mindegree; j <mindegree*2;j++){
			y->setChild(j, z->getchild(j - mindegree));
		}    
	}                                 
	for (j = i + 1; j < x->getnumk(); j++) { //move keylist and childlist towards one place from index i+1;
		x->setspider(j - 1, x->getspider(j));
		x->setChild(j, x->getchild(j + 1));       
	}
	x->setNumk(x->getnumk() - 1); //update x's keynum
	delete z;
}

template<class T,class G,class H> void btree<T,G,H>::deletek(H k){ //search and delete the spider with secondary key k in tree 
	if (search(k)){
		bnode* r = root;
		if (r->getnumk() == 1 && !r->getleaf())  // if root only has only one spider and is not leaf node
		{
			bnode* y = root->getchild(0);        //left child  y
			bnode* z = root->getchild(1);        //right child z
			if (y->getnumk() == z->getnumk() && z->getnumk() == mindegree - 1) {  // if both y and z has mindegree-1 spiders, then merge them
				merge(r, 0, y, z);               
				root = y;
				delete r ;
				delete_root(y, k);                    // delete from new root y
			}else{
				 delete_root(r, k);                   //just delete from root r
			}                   
		}
		else delete_root(r, k);					   	//just delete from root r
	}
}

template<class T,class G,class H> void btree<T,G,H>::delete_root(bnode* r, H k) //delete spider with k starting from r
{
	int i = 0;
	while (i < r->getnumk() && k > r->getkey(i)){  //find proper place
		i++;
	}
	if (r->getleaf()){// if it reaches the leaf node
		if (i<r->getnumk() && k == r->getkey(i))  // if find the delete place
		{
			for (int j = i + 1; j < r->getnumk(); j++)   
				r->setspider(j - 1, r->getspider(j));
			r->setNumk(r->getnumk() - 1);                //move toward to delete it
		}
		else cout << "The key does not exist!" << endl;
		return;
	}
// if this node is not leaf node				
	spider<T,G,H>* replace;
	bnode* z = NULL;
	bnode* y = r->getchild(i);
	if (i < r->getnumk()){
		z = r->getchild(i + 1);        //z right child
	}
	if ( i<r->getnumk() &&  k == r->getkey(i))  // if find the spider
	{
		if (y->getnumk() >= mindegree) {       //get spider with from the left node
			replace = searchahead(y);
			delete_root(y, replace->second_key);
			r->setspider(i, replace);
		}
		else if (z->getnumk() >= mindegree) {   //get spider with key from the right node
			replace = searchbehind(z);
			delete_root(z, replace->second_key);
			r->setspider(i, replace);
		}
		else {                                //merge the y, k, z
			merge(r, i, y, z);
			delete_root(y, k);
		}
	}else {
		bnode* p = NULL;
		if (i > 0) p = r->getchild(i - 1);
		if (y->getnumk() == mindegree - 1) {
			if (i > 0 && p->getnumk() >= mindegree){//Get: try the left side
				borrowrl(r, i - 1, p, y);
			}
			else if (i < r->getnumk() && z->getnumk() >= mindegree){//Get: try the right side
				borrowlr(r, i, y, z);
			}
			else if (i > 0) {              //merge the left side 
				merge(r, i - 1, p, y);
				y = p;
			}else merge(r, i, y, z);        //Merge the right side
		}
		delete_root(y, k);
	}
}


template<class T,class G,class H> spider<T,G,H>* btree<T,G,H>::searchahead(bnode* y){//get the spider with smallest ahead of y
	bnode* x = y;
	while (!x->getleaf()){
		x = x->getchild(x->getnumk());
	}
	return x->getspider(x->getnumk() - 1);
}

template<class T,class G,class H> spider<T,G,H>* btree<T,G,H>::searchbehind(bnode* z){//get the spider with biggest behind z
	bnode* x = z;
	while (!x->getleaf()){
		 x = x->getchild(0);
	}
	return x->getspider(0);
}

template<class T,class G,class H> void btree<T,G,H>::borrowrl(bnode* x, int i, bnode* y, bnode* z){//x's right child z borrows a spider and a child from x's left child of y
	int j = z->getnumk();
	for (; j > 0; j--){
		z->setspider(j, z->getspider(j - 1));          //borrows a spider and a child from x's left child of y
	}                                      
	z->setspider(0, x->getspider(i));
	x->setspider(i, y->getspider(y->getnumk() - 1));
	if (!z->getleaf()) {                              //if z is not leafnode, handle its child
		for (j = z->getnumk(); j >= 0; j--){
			z->setChild(j + 1, z->getchild(j));
		}
		z->setChild(0, y->getchild(y->getnumk()));
	}
	z->setNumk(z->getnumk() + 1);   // update z and y's numkey
	y->setNumk(y->getnumk() - 1);
}

template<class T,class G,class H> void btree<T,G,H>::borrowlr(bnode* x, int i, bnode* y, bnode* z) {//x's left child y borrows a spider and a child from x's right child of z
	int n = y->getnumk();
	y->setspider(n, x->getspider(i));
	x->setspider(i, z->getspider(0));
	for (int t = 1; t < z->getnumk(); t++){
		z->setspider(t - 1, z->getspider(t));     // borrows a spider and a child from x's right child of z
	}
	y->setChild(n + 1, z->getchild(0));
	if (!z->getleaf()){
		for (int t = 1; t <= z->getnumk(); t++){     // if z is not leaf,handle its child
			z->setChild(t - 1, z->getchild(t));
		}
	}
	y->setNumk(n + 1);
	z->setNumk(z->getnumk() - 1);  //update numkey
}

template<class T,class G,class H> void btree<T,G,H>::show() {// show the structure of b_tree from root 	
	doShow(root, 0);
}
template<class T,class G,class H> void btree<T,G,H>::doShow(bnode* root, int d){ // show the structure of b_tree from root 	
	bnode* tmp = root;
	cout << endl;
	for (int i = 0; i < d; i++)
	 	cout << "   ";
	if (d)
	 	cout << "->";
	cout << "Node (" << tmp->getnumk() << ": ";
	for (int i = 0; i < tmp->getnumk(); i++)
		cout << tmp->getkey(i) << " ";
	cout << ")";

	cout << " with \n ";
	for (int i = 0; i < tmp->getnumk(); i++)
	    {
			cout << "Tuples with Sec Key " << tmp->getkey(i) << ":\n ";
			b_retrieve_B(tmp->getkey(i))->dp_show();
		}
	if (!tmp->getleaf())
		for (int i = 0; i <= tmp->getnumk(); i++)
			doShow(tmp->getchild(i), d + 1);
}



template<class T,class G,class H> spider<T,G,H>* btree<T,G,H>::b_retrieve_B(H k){
	bnode* tmp = root;                                        //use btree property to find spider and return spider with this key
	while (tmp) {                                             
		int i = 0;
		while (i < tmp->getnumk() && k > tmp->getkey(i)){  
			i++;
		}                                                      //find proper place
		if (i < tmp->getnumk() && k == tmp->getkey(i)){
			return tmp->getspider(i);
		}
		else if(tmp->getleaf()){                               //if it is leaf, so that not find
			return NULL;
		}
		tmp = tmp->getchild(i);
	}
	return NULL;
}

template<class T,class G,class H> pair<block<T,G,H>*, int> btree<T,G,H>::b_delete_B(H k){
	spider<T,G,H>* temp=b_retrieve_B(k);    //find this spider
	if(temp !=NULL){
		if (temp->datapointer.size()>1)
		{
			pair<block<T,G,H>*, int> rtval=temp->datapointer[temp->datapointer.size()-1] ;
			temp->datapointer.pop_back();
			return rtval ;                       //if size is larger than  1,pop and return one dataptr
		}
		else if(temp->datapointer.size()==1){
			pair<block<T,G,H>*, int> rtval=temp->datapointer[temp->datapointer.size()-1] ;
			temp->datapointer.pop_back();
			deletek(k) ;
			return rtval ;                         //if it has only one ,pop it and delete this spider
		}
		
	}
	else{
		pair<block<T,G,H>*, int> rtnull ;
		rtnull.first = NULL ;
		rtnull.second=0 ;
		return rtnull ;                      //if not find , return paticular relation
	}
	exit(EXIT_FAILURE);
}

template<class T,class G,class H> void btree<T,G,H>::bp_insert_B(pair<block<T,G,H>*, int> dataptr, H k){
	spider<T,G,H>* tempspider=b_retrieve_B(k);     // find if it has exist spider
	if(tempspider !=NULL){
		tempspider->datapointer.push_back(dataptr);  // if it has, just push back this dataptr into vector
	}
	else{
		insert(k);
		tempspider=b_retrieve_B(k);
		tempspider->datapointer.push_back(dataptr); // if not, first insert spider and insert it
	}
}

template<class T,class G,class H> void  btree<T,G,H>::bp_delete_B(pair<block<T,G,H>*, int> dataptr,H k)
{
	spider<T,G,H>* tempspider = b_retrieve_B(k);
	if(tempspider ==NULL){
		cout<<"no this secondary key"<<endl ;
		return;
	}                    // if not find spider
	
	int num = tempspider->datapointer.size();
	 for(int i=0; i<num; i++){                           // delete particular dataptr in the vector
    	if(tempspider->datapointer[i]==dataptr){
        	tempspider->datapointer.erase(tempspider->datapointer.begin() + i);
        	break;
    	}
	}
	if(tempspider->datapointer.size() == 0){    //if spider is empty, delete this spider
		deletek(k) ;
	}
}

template<class T,class G,class H> void btree<T,G,H>::b_update(H k, pair<block<T,G,H>*, int> oldptr, pair<block<T,G,H>*, int> newptr)
{
	spider<T,G,H>* tempspider=b_retrieve_B(k);
	if(tempspider ==NULL)
	{
		cout<<"no this secondary key"<<endl ;
		return;                                  // if not find
	}
	for(int i=0; i<tempspider->datapointer.size();i++){
		if(tempspider->datapointer[i]==oldptr){
			tempspider->datapointer[i]=newptr ;
			break ;
		}                                         // if find this oldptr,just update it
		if(i==tempspider->datapointer.size()-1){
			cout<<"there is no this oldpointer "<<endl ;
		}
	}
}


