//#include "btree.h"
#include "head.h"

//initial
template<class T,class G,class H>
btree<T,G,H>::btree() {	
	root = new bnode();
	root->setLeaf(true);	//start with leaf node
}

//initial
template<class T,class G,class H>
btree<T,G,H>::~btree() {	
	clear();			//free the sources
	delete root;		//free the root of btree
}

//search k in root
template<class T,class G,class H>
bool btree<T,G,H>::search(H k) {	
	bnode* tmp = root;
	while (tmp) {
		int i = 0;
		while (i < tmp->getnumk() && k > tmp->getkey(i)) i++;
		if (i < tmp->getnumk() && k == tmp->getkey(i)) return true;
		else if (tmp->getleaf()) return false;
		tmp = tmp->getchild(i);
	}
	return false;
}

//split the child whose index is i of node x
template<class T,class G,class H>
void btree<T,G,H>::split(bnode* x, int i) {	
	//x - the node, i - the index of node which will be splited
	bnode* z = new bnode(), *y = x->getchild(i);	//y - left child of x, z - right child of x
	z->setLeaf(y->getleaf());
	z->setNumk(mindegree - 1);
	for (int j = 0; j < mindegree - 1; j++)	//handle with the right child
		z->setspider(j, y->getspider(j + mindegree));  //!!
	if (y->isleaf == false)
		for (int j = 0; j < mindegree; j++)
			z->setChild(j, y->getchild(j + mindegree));
	y->setNumk(mindegree - 1);
	//y->show();//test
	//z->show();//test
	for (int j = x->getnumk(); j > i; j--)		//handle node x
		x->setChild(j + 1, x->getchild(j));
	x->setChild(i + 1, z);
	for (int j = x->getnumk() - 1; j >= i; j--)
		x->setspider(j + 1, x->getspider(j)); //!!
	x->setspider(i, y->getspider(mindegree - 1));
	x->setNumk(x->getnumk() + 1);
	//x->show(0);//test
	//cout << endl;//test
}

//insert the k into the subtree whose root is node x
template<class T,class G,class H>
void btree<T,G,H>::insertNon(bnode* x, H k) {	
	int i = x->getnumk() - 1;
	if (x->isleaf) {	//for leaf node
		while (i >= 0 && k < x->getkey(i)) {
			x->setspider(i + 1, x->getspider(i));
			i--;
		}
		spider<T,G,H>* newsp = new spider<T,G,H>;
		newsp->second_key= k ;
		x->setspider(i + 1, newsp);
		x->setNumk(x->getnumk() + 1);
	}
	else {
		while (i >= 0 && k < x->getkey(i)) i--;
		i++;
		if (x->getchild(i)->getnumk() == 2 * mindegree - 1) {		//we split the node for recursion
			split(x, i);
			if (k > x->getkey(i)) i++;
		}
		insertNon(x->getchild(i), k);
	}
}

//insert the k into root
template<class T,class G,class H>
void btree<T,G,H>::insert(H k) {	
	bnode* r = root;
	if (2 * mindegree - 1 == r->getnumk()) {		//for the root, the tree grows only with the root 
		bnode* s = new bnode();
		root = s;
		s->setNumk(0);
		s->setLeaf(false);
		s->setChild(0, r);
		split(s, 0);
		insertNon(s, k);
	}
	else insertNon(r, k);
}

//merge node y, key i and node z, x is the parent of y and z
template<class T,class G,class H>
void btree<T,G,H>::merge(bnode* x, int i, bnode* y, bnode* z) {		
	//i: the index of key in x, y: left child of x, z: right child of x
	int j = mindegree;
	y->setNumk(2 * mindegree - 1);
	for (; j < 2 * mindegree - 1; j++)
		y->setspider(j, z->getspider(j - mindegree));
	y->setspider(mindegree - 1, x->getspider(i));
	if (!y->getleaf())
		for (j = mindegree; j < 2 * mindegree; j++)
			y->setChild(j, z->getchild(j - mindegree));
	for (j = i + 1; j < x->getnumk(); j++) {
		x->setspider(j - 1, x->getspider(j));
		x->setChild(j, x->getchild(j + 1));
	}
	x->setNumk(x->getnumk() - 1);
	delete z;
}

//delete the k from root
template<class T,class G,class H>
void btree<T,G,H>::deletek(H k)
{		
	if (search(k))
	{
		bnode* r = root;
		if (r->getnumk() == 1 && !r->getleaf())
		{
			bnode* y = root->getchild(0);
			bnode* z = root->getchild(1);
			if (y->getnumk() == z->getnumk() && z->getnumk() == mindegree - 1) {
				merge(r, 0, y, z);
				root = y;
				delete r;
				delNon(y, k);
			}
			else delNon(r, k);
		}
		else delNon(r, k);
	}
}

template<class T,class G,class H>
void btree<T,G,H>::delNon(bnode* x, H k)
{
	int i = 0;
	while (i < x->getnumk() && k > x->getkey(i))  i++;

	//Reach the leaf node
	if (x->getleaf())
	{

		if (x->getspider(i) != NULL && k == x->getkey(i))
		{
			//delete x->getspider(i);     ///////
			for (int j = i + 1; j < x->getnumk(); j++)
				x->setspider(j - 1, x->getspider(j));
			x->setNumk(x->getnumk() - 1);
		}
		else cout << "The key does not exist!" << endl;
		return;
	}

	// the iner node
	spider<T,G,H>* ans;
	bnode* z = NULL, *y = x->getchild(i);
	if (i < x->getnumk()) z = x->getchild(i + 1);
	if ( x->getspider(i) != NULL &&  k == x->getkey(i))
	{
		if (y->getnumk() >= mindegree) {//get the key from the left node
			ans = searchPre(y);
			delNon(y, ans->second_key);
			x->setspider(i, ans);
		}
		else if (z->getnumk() >= mindegree) {//get the key from the right node
			ans = searchSuc(z);
			delNon(z, ans->second_key);
			x->setspider(i, ans);
		}
		else {//merge the y, k, z
			merge(x, i, y, z);
			delNon(y, k);
		}
	}
	else {
		bnode* p = NULL;
		if (i > 0) p = x->getchild(i - 1);
		if (y->getnumk() == mindegree - 1) {
			if (i > 0 && p->getnumk() >= mindegree)//Get: try the left side
				shiftRTL(x, i - 1, p, y);
			else if (i < x->getnumk() && z->getnumk() >= mindegree)//Get: try the right side
				shiftLTR(x, i, y, z);
			else if (i > 0) {//Merge: try the left side
				merge(x, i - 1, p, y);
				y = p;
			}
			else merge(x, i, y, z);//Merge: try the right side
		}
		delNon(y, k);
	}
}

//get the pre of node y
template<class T,class G,class H>
spider<T,G,H>* btree<T,G,H>::searchPre(bnode* y) {
	bnode* x = y;
	while (!x->getleaf())
		x = x->getchild(x->getnumk());
	return x->getspider(x->getnumk() - 1);
}

//get the suc of node z
template<class T,class G,class H>
spider<T,G,H>* btree<T,G,H>::searchSuc(bnode* z) {
	bnode* x = z;
	while (!x->getleaf()) x = x->getchild(0);
	return x->getspider(0);
}

//x's right child y borrows a key and a child from x's left child of z
//i: the index of key in x, y: left child of x, z: right child of x
template<class T,class G,class H>
void btree<T,G,H>::shiftRTL(bnode* x, int i, bnode* y, bnode* z) {

	int j = z->getnumk();
	for (; j > 0; j--)
		z->setspider(j, z->getspider(j - 1));
	z->setspider(0, x->getspider(i));
	x->setspider(i, y->getspider(y->getnumk() - 1));
	if (!z->getleaf()) {
		for (j = z->getnumk(); j >= 0; j--)
			z->setChild(j + 1, z->getchild(j));
		z->setChild(0, y->getchild(y->getnumk()));
	}
	z->setNumk(z->getnumk() + 1);
	y->setNumk(y->getnumk() - 1);
}

template<class T,class G,class H>
void btree<T,G,H>::shiftLTR(bnode* x, int i, bnode* y, bnode* z) {//...
	//i: the index of key in x, y: left child of x, z: right child of x
	int n = y->getnumk();
	y->setspider(n, x->getspider(i));
	x->setspider(i, z->getspider(0));
	for (int k = 1; k < z->getnumk(); k++)
		z->setspider(k - 1, z->getspider(k));
	y->setChild(n + 1, z->getchild(0));
	if (!z->getleaf())
		for (int k = 1; k <= z->getnumk(); k++)
			z->setChild(k - 1, z->getchild(k));
	y->setNumk(n + 1);
	z->setNumk(z->getnumk() - 1);
}

//show the nodes in the order of floor
template<class T,class G,class H>
void btree<T,G,H>::doShow(bnode* root, int d) {	
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

template<class T,class G,class H>
void btree<T,G,H>::show() {
	//API for showing the btrees
	doShow(root, 0);
}

//show the nodes in the order of floor
template<class T,class G,class H>
void btree<T,G,H>::doClear(bnode* root) {	
	if (!root->getleaf())
		for (int i = 0; i <= root->getnumk(); i++) {
			doClear(root->getchild(i));
			root->setLeaf(true);
			delete root->getchild(i);
		}
	for (int i = 0; i < root->getnumk(); i++){
		delete root->getspider(i);
	}
	
}

template<class T,class G,class H>
void btree<T,G,H>::clear() {
	//API for free the sources we apply
	doClear(root);
}

template<class T,class G,class H> spider<T,G,H>* btree<T,G,H>::b_retrieve_B(H k){
	bnode* tmp = root;
	while (tmp) {
		int i = 0;
		while (i < tmp->getnumk() && k > tmp->getkey(i)){
			i++;
		}
		if (i < tmp->getnumk() && k == tmp->getkey(i)){
			return tmp->getspider(i);
		}
		else if(tmp->getleaf()){
			return NULL;
		}
		tmp = tmp->getchild(i);
	}
	return NULL;
}

template<class T,class G,class H> spider<T,G,H>* btree<T,G,H>::b_delete_B(H k){
	spider<T,G,H>* rtval=b_retrieve_B(k);
	if(rtval !=NULL){
		deletek(k);
	}
	return rtval ;
}

template<class T,class G,class H> void btree<T,G,H>::bp_insert_B(pair<block<T,G,H>*, int> dataptr, H k){
	spider<T,G,H>* tempspider=b_retrieve_B(k);
	if(tempspider !=NULL){
		tempspider->datapointer.push_back(dataptr);
	}
	else{
		insert(k);
		tempspider=b_retrieve_B(k);
		tempspider->datapointer.push_back(dataptr);
	}
}

template<class T,class G,class H> void  btree<T,G,H>::bp_delete_B(pair<block<T,G,H>*, int> dataptr,H k)
{
	spider<T,G,H>* tempspider = b_retrieve_B(k);
	if(tempspider ==NULL){
		cout<<"no this secondary key"<<endl ;
		return;
	}
	//for(vector<pair<block<T,G,H>*, int>>::iterator iter=tempspider->datapointer.begin();iter!=tempspider->datapointer.end();iter++){        //从vector中删除指定的某一个元素 
	int num = tempspider->datapointer.size();
	 for(int i=0; i<num; i++){
    	if(tempspider->datapointer[i]==dataptr){
			//需不需要delete dataptr？
        	tempspider->datapointer.erase(tempspider->datapointer.begin() + i);
        	break;
    	}
	}
	if(tempspider->datapointer.size() == 0){
		deletek(k) ;
	}
}

template<class T,class G,class H> void btree<T,G,H>::b_update(H k, pair<block<T,G,H>*, int> oldptr, pair<block<T,G,H>*, int> newptr)
{
	spider<T,G,H>* tempspider=b_retrieve_B(k);
	if(tempspider ==NULL)
	{
		cout<<"no this secondary key"<<endl ;
		return;
	}
	for(int i=0; i<tempspider->datapointer.size();i++){
		if(tempspider->datapointer[i]==oldptr){
			tempspider->datapointer[i]=newptr ;
			break ;
		}
		if(i==tempspider->datapointer.size()-1){
			cout<<"there is no this oldpointer "<<endl ;
		}
	}
}


