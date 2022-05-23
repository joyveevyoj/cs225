
#include<iostream>
#include"head.h"
using namespace std;
template<class T, class G, class H>Bpnode<T,G,H>::Bpnode(int node_order)
{
    leaf_status = false;
    order = node_order;
    num_key = 0;
    max_key_num = order - 1;
    min_key_num = (order + 1) / 2 - 1;
    int max_children_num = order;

    //Add 1 so that the last position stores overflow item temporarily
    keys = new G[max_key_num + 1];
    children = new Bpnode<T,G,H>*[max_children_num + 1];
    blocks = new block<T,G,H>*[max_children_num + 1];
    //initialized as 0 and NULL
    for(int i = 0; i < max_key_num + 1; i++)
    {
        keys[i] = 0;
    }
    for(int i = 0; i < max_children_num + 1; i++)
    {
        children[i] = NULL;
        blocks[i] = NULL;
    }
    parent = NULL;
    prev = NULL;
    next = NULL;
}

template<class T, class G, class H>bool Bpnode<T,G,H>::is_leaf()
{   
   return leaf_status;
}

template<class T, class G, class H>int Bpnode<T,G,H>::getkeynum()
{
    return num_key;
}
template<class T, class G, class H>G Bpnode<T,G,H>::getkey(int index)
{   
    return keys[index];
}

template<class T,class G, class H>Bpnode<T,G,H>* Bpnode<T,G,H>::getchild(int index)
{
    if(this->leaf_status == true)
    {
        cout << "Get child from leaf node. Wrong attempt!" << endl;
        exit(EXIT_FAILURE);
    }
    return children[index];
}

template<class T,class G, class H>block<T,G,H>* Bpnode<T,G,H>::getblock(int index)
{
    if(this->leaf_status == false)
    {
        cout << "Get block from nonleaf node. Wrong attempt!" << endl;
        exit(EXIT_FAILURE);
    }
    return blocks[index];
}

template<class T,class G, class H>Bpnode<T,G,H>* Bpnode<T,G,H>::getparent()
{
    return this->parent;
}

template<class T,class G, class H>Bpnode<T,G,H>* Bpnode<T,G,H>::getprev()
{
    return this->prev;
}

template<class T,class G, class H>Bpnode<T,G,H>* Bpnode<T,G,H>::getnext()
{
    return this->next;
}

template<class T,class G, class H>void Bpnode<T,G,H>::setleaf(bool status)
{
    leaf_status = status;
}

template<class T,class G, class H>void Bpnode<T,G,H>::setkey(G key, int index)
{
    keys[index] = key;
}

template<class T,class G, class H>void Bpnode<T,G,H>::setchild(Bpnode<T,G,H>* child, int index)
{
    if(this->leaf_status == true)
    {
        cout << "Set child in leaf node. Wrong attempt!" << endl;
        exit(EXIT_FAILURE);
    }
    children[index] = child;
}

template<class T,class G, class H>void Bpnode<T,G,H>::setblock(block<T,G,H>* block, int index)
{
    if(this->leaf_status == false)
    {
        cout << "Set block in nonleaf node. Wrong attempt!" << endl;
        exit(EXIT_FAILURE);
    }
    blocks[index] = block;
}

template<class T,class G, class H>void Bpnode<T,G,H>::setparent(Bpnode<T,G,H>* node)
{
    parent = node;
}

template<class T,class G, class H>void Bpnode<T,G,H>::setprev(Bpnode<T,G,H>* node)
{
    prev = node;
}

template<class T,class G, class H>void Bpnode<T,G,H>::setnext(Bpnode<T,G,H>* node)
{
    next = node;
}


template<class T,class G,class H>int Bpnode<T,G,H>::child_binary_search(G pri_key)
{
    int mid;
    int front = 0;
    int back = num_key - 1;
    //If there is no key, return the first index
    if(num_key == 0)
    {
        return 0;
    }
    if(pri_key < keys[front])
    {
        return 0;
    }
    if(pri_key >= keys[back])
    {
        return num_key;
    }
    while(front <= back)
    {
        mid = front + (back - front) / 2;
        if(pri_key < keys[mid])
        {
            //If there is no element in between, we find the position
            if(mid - front == 1)
            {
                return front + 1;
            }
            back = mid;
        }
        if(pri_key >= keys[mid])
        {
            //If there is no element in between, we find the position
            if(back - mid == 1)
            {
                return back;
            }
            front = mid;
        }
    }
    cout << "Something Wrong happened" << endl;
    return -1;
}

template<class T,class G,class H>int Bpnode<T,G,H>::retrieve_binary_search(G pri_key)
{
    int mid;
    int front = 0;
    int back = num_key - 1;
    //If there is no key, return the first index
    if(num_key == 0)
    {
        return 0;
    }
    if(pri_key <= keys[front])
    {
        return 0;
    }
    if(pri_key > keys[back])
    {
        return num_key;
    }
    while(front <= back)
    {
        mid = front + (back - front) / 2;
        if(pri_key <= keys[mid])
        {
            //If there is no element in between, we find the position
            if(mid - front == 1)
            {
                return front + 1;
            }
            back = mid;
        }
        if(pri_key > keys[mid])
        {
            //If there is no element in between, we find the position
            if(back - mid == 1)
            {
                return back;
            }
            front = mid;
        }
    }
    cout << "Something Wrong happened" << endl;
    return -1;
}

template<class T,class G, class H> int Bpnode<T,G,H>::key_binary_search(G pri_key)
{
    if(this->is_leaf() == false)
    {
        cout << "Please use key binary search on leaf nodes" << endl;
        exit(EXIT_FAILURE);
    }
    int mid;
    int front = 0;
    int back = this->num_key - 1;
    while(front <= back)
    {
        mid = front + (back - front) / 2;
        if(keys[mid] == pri_key)
            return mid;
        if(keys[mid] < pri_key)
            front = mid + 1;
        if(keys[mid] > pri_key)
            back = mid - 1;
    }
    cout <<"Primary key not in the node" << endl;
    return -1;
}

template<class T, class G, class H> int Bpnode<T,G,H>::block_search(block<T,G,H>* blockptr)
{
    if(this->is_leaf() == false)
    {
        cout << "Please use block search on leaf nodes" << endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < this->num_key + 1; i++)
    {
        if(this->blocks[i] == blockptr)
            return i;
    }
    cout <<"Block Pointer not in the node" << endl;
    return -1;
}

template<class T, class G, class H> int Bpnode<T,G,H>::child_search(Bpnode<T,G,H>* node)
{
    if(this->is_leaf() == true)
    {
        cout << "Please use child search on nonleaf nodes" << endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < this->num_key + 1; i++)
    {
        if(this->children[i] == node)
            return i;
    }
    cout <<"Child not in the node" << endl;
    return -1;
}

template<class T,class G,class H>void Bpnode<T,G,H>::printkey()
{
    cout << "(";
    for(int i = 0; i < this->num_key; i++)
    {
        if(i == this->num_key - 1)
        {
            cout << this->keys[i];
            continue;
        }
        cout << this->keys[i] << ", ";
    }
    cout << ")";
}

//This function should be used together with class block
template<class T,class G,class H>void Bpnode<T,G,H>::printblock()
{
    for(int i = 0; i < this->num_key + 1; i++)
    {
        cout << "[";
        if(this->blocks[i]->l_seperate == -999999)
            cout << "None, ";
        else
            cout << this->blocks[i]->l_seperate << ", ";
        if(this->blocks[i]->r_seperate == -999999)
            cout << "None";
        else
            cout << this->blocks[i]->r_seperate;
        cout << "] ";
    }
}



template<class T, class G,class H>Bptree<T,G,H>::Bptree(int tree_order)
{
    order = tree_order;
    root = new Bpnode<T,G,H>(tree_order);
    root->setleaf(true);
}

template<class T, class G, class H>Bpnode<T,G,H>* Bptree<T,G,H>::block_retrieve(block<T,G,H>* blockptr)
{
    return _block_retrieve(root, blockptr);
}

template<class T, class G, class H>Bpnode<T,G,H>* Bptree<T,G,H>::_block_retrieve(Bpnode<T,G,H>* cur_node, block<T,G,H>* blockptr)
{
    //Search based on the first key in block
    int index = cur_node->retrieve_binary_search(blockptr->mainblock[0]->pri_key);
    //If it's not leaf, keep searching
    if(cur_node->is_leaf() == false)
    {
        Bpnode<T,G,H>* child = cur_node->getchild(index);
        return _block_retrieve(child, blockptr);
    }
    //If it's leaf, node found  
    if(cur_node->is_leaf() == true)
        return cur_node;
    //Exception Case
    cout << "Something Wrong with block retrieve function" << endl;
    exit(EXIT_FAILURE);
    

}

template<class T, class G, class H>block<T,G,H>* Bptree<T,G,H>::retrieve(G pri_key)
{   
    return _retrieve(root, pri_key);
}

template<class T, class G, class H>block<T,G,H>* Bptree<T,G,H>::_retrieve(Bpnode<T,G,H>* cur_node, G pri_key)
{
    //Search based on the position of the given pri_key
    int index = cur_node->retrieve_binary_search(pri_key);
    //If it's not leaf, keep searching
    if(cur_node->is_leaf() == false)
    {
        Bpnode<T,G,H>* child = cur_node->getchild(index);
        return _retrieve(child,pri_key);
    }
    //If it's leaf, find the according block
    if(cur_node->is_leaf() == true)
    {
        block<T,G,H>* blockptr = cur_node->getblock(index);
        return blockptr;
    }
    //Exception Case
    cout << "Something wrong with retrieve " << endl;
    exit(EXIT_FAILURE);
}

template<class T, class G, class H>void Bptree<T,G,H>::init_insert(block<T,G,H>* blockptr)
{
    root->setblock(blockptr,0);
}

template<class T, class G, class H>void Bptree<T,G,H>::insert(G pri_key, block<T,G,H>* blockptr)
{
    _insert(root, pri_key,blockptr);
}

template<class T, class G, class H>void Bptree<T,G,H>::_insert(Bpnode<T,G,H>* cur_node, G pri_key, block<T,G,H>* blockptr)
{
    //If it's not leaf, keep searching
    if(cur_node->is_leaf() == false)
    {
        int child_index = cur_node->child_binary_search(pri_key);
        Bpnode<T,G,H>* child = cur_node->getchild(child_index);
        _insert(child,pri_key,blockptr);
    }
    //If it's leaf
    if(cur_node->is_leaf() == true)
    {
        //Key Operation
        //Find the position to insert
        int key_index = cur_node->child_binary_search(pri_key);
        cur_node->num_key++;
        //Key shift right by 1
        for(int i = cur_node->num_key - 1; i > key_index; i--)
        {
            cur_node->setkey(cur_node->getkey(i - 1), i);  
        }
        //Insert pri_key
        cur_node->setkey(pri_key, key_index);

        //Block operation
        //Find the position to insert
        int block_index = cur_node->child_binary_search(pri_key);
        //Block shift right by 1
        for(int i = cur_node->num_key; i > block_index; i--)
        {
            cur_node->setblock(cur_node->getblock(i - 1) , i);
        }
        //Insert blockptr
        cur_node->setblock(blockptr,block_index);

        //If current leaf node is full, split it
        if(cur_node->num_key - 1 == cur_node->max_key_num)
        {
            split(cur_node);
        }
    }
}

template<class T, class G, class H>void Bptree<T,G,H>::split(Bpnode<T,G,H>* node)
{
    Bpnode<T,G,H>* parent = node->getparent();
    //if this node is root, create a new root(parent)
    //new_root_mark means a new root is created
    bool new_root_mark = false;    
    if(parent == NULL)
    {
        parent = new Bpnode<T,G,H>(order);
        //New parent is not leaf (defined by constructor)
        parent->setparent(NULL);    
        new_root_mark = true;
    }

    G mid_key = node->getkey(node->num_key / 2);
    //Set up two new nodes
    Bpnode<T,G,H>* l_node = new Bpnode<T,G,H>(order);
    Bpnode<T,G,H>* r_node = new Bpnode<T,G,H>(order);

    //leaf status
    if(node->is_leaf() == true)
    {
        l_node->setleaf(true);
        r_node->setleaf(true);
    }

    //l_node information
    //Key set up
    for(int i = 0; i < node->num_key / 2; i++)
    {
        l_node->setkey(node->getkey(i), i);
        l_node->num_key++;
    }
    //Block/Children set up
    for(int i = 0; i < node->num_key / 2 + 1; i++)
    {   
        if(node->is_leaf() == true)
            l_node->setblock(node->getblock(i),i);
        if(node->is_leaf() == false)
        {
            l_node->setchild(node->getchild(i),i);
            //If all leaves are to be connected, delete the following code
            if(i == node->num_key / 2)
                node->getchild(i)->setnext(NULL);
        }

    }
    l_node->setparent(parent);

    //r_node information
    //Key set up
    for(int i = node->num_key / 2 + 1; i < node->num_key; i++)
    {
        r_node->setkey(node->getkey(i), i - (node->num_key / 2 + 1));
        r_node->num_key++;
    }
    //Block/Children set up
    for(int i = node->num_key / 2 + 1; i < node->num_key + 1; i++)
    {
        if(node->is_leaf() == true)
            r_node->setblock(node->getblock(i), i - (node->num_key / 2 + 1));
        if(node->is_leaf() == false)
        {
            r_node->setchild(node->getchild(i), i - (node->num_key / 2 + 1));
            //If all leaves are to be connected, delete the following code     
            if(i == node->num_key / 2 + 1)
                node->getchild(i)->setprev(NULL);
        }
    }
    r_node->setparent(parent);

    //If l_node and r_node are not leaves, their children's parent should change as well
    if(node->is_leaf() == false)
    {   
        for(int i = 0; i < l_node->num_key + 1; i++)
        {
            l_node->getchild(i)->setparent(l_node);
        }
        for(int i = 0; i < r_node->num_key + 1; i++)
        {
            r_node->getchild(i)->setparent(r_node);
        }
    }

    //parent information
    //Find the position to insert mid key
    int key_index = parent->child_binary_search(mid_key);
    //Parent's key shift right by 1
    parent->num_key++;
    for(int i = parent->num_key - 1; i > key_index; i--)
    {
        parent->setkey(parent->getkey(i - 1), i);  
    }
    //Insert mid_key
    parent->setkey(mid_key, key_index);

    //Find the position for two new children
    int child_index = parent->child_binary_search(mid_key);
    //Parent's children shift right by 1
    for(int i = parent->num_key; i > child_index; i--)
    {
        parent->setchild(parent->getchild(i - 1) ,i);
    }
    //Set up parent's new children
    parent->setchild(l_node, child_index - 1);
    parent->setchild(r_node,child_index);

    //l_node's and r_node's sibling 
    if(node->getprev() != NULL)
    {
        node->getprev()->setnext(l_node);
    }
    l_node->setprev(node->getprev());
    l_node->setnext(r_node);
    r_node->setprev(l_node);
    r_node->setnext(node->getnext());
    if(node->getnext() != NULL)
    {
        node->getnext()->setprev(r_node);
    }
    //If new root is created, parent becomes the root
    if(new_root_mark == true)
    {
        this->root = parent;
    }
    //If parent overflows as well, keep splitting
    if(parent->num_key - 1 == parent->max_key_num)
    {
        split(parent);
    }
}

template<class T, class G, class H>void Bptree<T,G,H>::delete1(G oldpri_key, G newpri_key)
{
    _delete1(root, oldpri_key, newpri_key);
}

template<class T, class G, class H>void Bptree<T,G,H>::_delete1(Bpnode<T,G,H>* cur_node, G oldpri_key, G newpri_key)
{
    //If it's not leaf, keep searching
    int index = cur_node->child_binary_search(oldpri_key);
    if(cur_node->is_leaf() == false)
    {
        Bpnode<T,G,H>* child = cur_node->getchild(index);
        _delete1(child, oldpri_key, newpri_key);
    }
    //If it's leaf, find the index of old primary key, update it
    if(cur_node->is_leaf() == true)
    {
        int key_index = cur_node->key_binary_search(oldpri_key);
        cur_node->setkey(newpri_key, key_index);
    }
}

template<class T, class G, class H>void Bptree<T,G,H>::last_delete()
{
    root->setblock(NULL,0);
}

template<class T, class G, class H>void Bptree<T,G,H>::delete2(G pri_key, block<T,G,H>* blockptr)
{
    _delete2(root, pri_key, blockptr);
}

template<class T, class G, class H>void Bptree<T,G,H>::_delete2(Bpnode<T,G,H>* cur_node, G pri_key, block<T,G,H>* blockptr)
{
    //If it's not leaf, keep searching
    if(cur_node->is_leaf() == false)
    {
        int child_index = cur_node->child_binary_search(pri_key);
        Bpnode<T,G,H>* child = cur_node->getchild(child_index);
        _delete2(child, pri_key, blockptr);
    }
    //If it's leaf
    if(cur_node->is_leaf() == true)
    {
        //If current node is root and only one block is leaf, we can't delete it
        if(cur_node->getparent() == NULL && cur_node->num_key == 0)
        {
            cout << "There is only one block in root. Delete fail!";
            exit(EXIT_FAILURE);
        }
        //Find key's and block's index to be deleted
        int key_index = cur_node->key_binary_search(pri_key);
        int block_index = cur_node->block_search(blockptr);
        //Make sure the key and blockptr match
        if(key_index - block_index > 1 || block_index - key_index > 1)
        {
            cout << "Primary Key and Block pointer doesn't match!" << endl;
            exit(EXIT_FAILURE);
        }
        //Key left shift by one
        cur_node->num_key--;
        for(int i = key_index; i < cur_node->num_key; i++)
        {
            cur_node->setkey( cur_node->getkey(i + 1), i);
        }
        //Original last position reset as 0
        cur_node->setkey(0,cur_node->num_key);
        //Block left shift by one
        for(int i = block_index; i < cur_node->num_key + 1; i++)
        {
            cur_node->setblock( cur_node->getblock(i + 1), i);
        }
        //Original last postion resest as NULL
        cur_node->setblock(NULL,cur_node->num_key + 1);

        //If current key number is less than min_key_num and it's not root, merge this node
        if(cur_node->getparent() != NULL && cur_node->num_key < cur_node->min_key_num)
        {
            merge(cur_node);
        }
    }
}

template<class T, class G, class H>void Bptree<T,G,H>::merge(Bpnode<T,G,H>* node)
{
    Bpnode<T,G,H>* parent = node->getparent();
    //Case 1: if there is a sibling with more than min_key_num keys. Only redistribute
    //Choose to merge with right sibling first
    //Merge with right
    if(node->getnext() != NULL)
    {
        if(node->getnext()->num_key > node->min_key_num)
        {
            merge1(node,node->getnext());
            return;
        }
    }
    //Merge with left
    if(node->getprev() != NULL)
    {
        if(node->getprev()->num_key > node->min_key_num)
        {
            merge1(node->getprev(),node);
            return;
        }
    }
    //Case 2: if there is no sibling with more than min_key_num keys.
    //Choose toe merge with right sibling first
    //Merge with right
    if(node->getnext() != NULL)
    {
        merge2(node,node->getnext());
        //After merge, if parent needs to merge as well, continue (parent can't be root)
        if(parent->getparent() != NULL && parent->num_key < parent->min_key_num)
            merge(parent);
        return;
    }
    //Merge with left
    if(node->getprev() != NULL)
    {
        merge2(node->getprev(), node);
        //After merge, if parent needs to merge as well, continue (parent can't be root)
        if(parent->getparent() != NULL && parent->num_key < parent->min_key_num)
        {
            merge(parent);
        }
        return;
    }
}

template<class T, class G, class H>void Bptree<T,G,H>::merge1(Bpnode<T,G,H>* l_node, Bpnode<T,G,H>* r_node)
{
    Bpnode<T,G,H>* parent = l_node->getparent();
    //Store the original key number of two nodes. They are used when blocks are updated.
    int leftkey_num = l_node->num_key;
    int rightkey_num = r_node->num_key;
    //Find separating key. Key's index is the same is left node's index
    int key_index = parent->child_search(l_node);   
    //Set up key array which stores keys in two nodes and separate key in parent
    int array_size = l_node->num_key + 1 + r_node->num_key;
    G* key_array = new G[array_size];
    for(int i = 0; i < l_node->num_key; i++)
        key_array[i] = l_node->getkey(i);
    key_array[l_node->num_key] = parent->getkey(key_index);
    for(int i = l_node->num_key + 1; i < array_size; i++)
        key_array[i] = r_node->getkey(i - (l_node->num_key + 1));

    //Set key for left nodes
    //Initialized as zero first
    for(int i = 0; i < l_node->num_key; i++)
        l_node->setkey(0, i);
    for(int i = 0; i < (array_size - 1) / 2; i++)
        l_node->setkey(key_array[i],i);
    l_node->num_key = (array_size - 1) / 2;

    //Set key for parent node
    parent->setkey(key_array[(array_size - 1) / 2], key_index);   

    //Set key for right nodes
    //Initialized as zero first
    for(int i = 0; i < r_node->num_key; i++)
        r_node->setkey(0,i);
    for(int i = (array_size + 1) / 2; i < array_size; i++)
        r_node->setkey(key_array[i], i - (array_size + 1) / 2);
    r_node->num_key = (array_size - (array_size + 1) / 2);


    //Same precedure, but for blocks and children
    //Set up block array for leaf and child array for nonleaf
    int block_array_size = leftkey_num + 1 + rightkey_num + 1;
    //child_array_size is the same as block array size. Just a little convenient to distinguish
    int child_array_size = block_array_size;

    block<T,G,H>** block_array = new block<T,G,H>*[block_array_size];
    Bpnode<T,G,H>** child_array = new Bpnode<T,G,H>*[child_array_size];
    for(int i = 0; i < leftkey_num + 1; i++)
    {
        if(l_node->is_leaf() == true)
            block_array[i] = l_node->getblock(i);
        if(l_node->is_leaf() == false)
            child_array[i] = l_node->getchild(i);
    }
    for(int i = leftkey_num + 1; i < block_array_size; i ++)
    {
        if(l_node->is_leaf() == true)
            block_array[i] = r_node->getblock( i - (leftkey_num + 1));
        if(l_node->is_leaf() == false)
            child_array[i] = r_node->getchild( i - (leftkey_num + 1));
    }

    //Set blocks/children for left node
    //Initialized as NULL at first
    for(int i = 0; i < leftkey_num + 1; i++)
    {
        if(l_node->is_leaf() == true)
            l_node->setblock(NULL, i);
        if(l_node->is_leaf() == false)
            l_node->setchild(NULL, i);
    }
    //Give it blocks/children again
    for(int i = 0; i < block_array_size / 2; i++)
    {
        if(l_node->is_leaf() == true)
            l_node->setblock(block_array[i], i);
        if(l_node->is_leaf() == false)
        {
            //It's better to reset children's information, or it might be a little messy
            Bpnode<T,G,H>* child = child_array[i];
            l_node->setchild(child, i);
            child->setparent(l_node);
            //First child's prev should be NULL
            if(i == 0)
                child->setprev(NULL);
            if(i != 0)
                child->setprev(child_array[i - 1]);
            //Last child's next should be NULL
            if(i == block_array_size / 2 - 1)
                child->setnext(NULL);
            if(i != block_array_size / 2 - 1)
                child->setnext(child_array[i + 1]);
        }
    }

    //Set blocks/children for right node
    //Initialized as NULL at first
    for(int i = 0; i < rightkey_num + 1; i++)
    {
        if(l_node->is_leaf() == true)
            r_node->setblock(NULL,i);
        if(l_node->is_leaf() == false)
            r_node->setchild(NULL, i);
    }
    //Give it blocks/children again
    for(int i = block_array_size / 2; i < block_array_size; i++)
    {
        if(l_node->is_leaf() == true)
            r_node->setblock(block_array[i], i - block_array_size / 2);
        if(l_node->is_leaf() == false)
        {
            //It's better to reset children's information, or it might be a little messy
            Bpnode<T,G,H>* child = child_array[i];
            r_node->setchild(child, i - child_array_size / 2);
            child->setparent(r_node);
            //First child's prev should be NULL
            if(i == child_array_size / 2)
                child->setprev(NULL);
            if(i != child_array_size / 2)
                child->setprev(child_array[i - 1]);
            //Last child's next should be NULL
            if(i == child_array_size - 1)
                child->setnext(NULL);
            if(i != child_array_size - 1)
                child->setnext(child_array[i + 1]);            
        }
    }
}

//This function merge r_node to l_node by default
template<class T, class G, class H>void Bptree<T,G,H>::merge2(Bpnode<T,G,H>* l_node, Bpnode<T,G,H>* r_node)
{
    Bpnode<T,G,H>* parent = l_node->getparent();
    //Key's index is the same as l_node's index
    int key_index = parent->child_search(l_node);   

    //left node set key
    l_node->setkey(parent->getkey(key_index), l_node->num_key);
    for(int i = l_node->num_key + 1; i < l_node->num_key + 1 + r_node->num_key; i++)
    {
        l_node->setkey(r_node->getkey(i - (l_node->num_key + 1)) , i);
    }

    //left node set blocks/children
    for(int i = l_node->num_key + 1; i < l_node->num_key + 1 + r_node->num_key + 1; i++)
    {
        if(l_node->is_leaf() == true)
            l_node->setblock(r_node->getblock(i - l_node->num_key - 1), i);
        if(l_node->is_leaf() == false)
        {
            l_node->setchild(r_node->getchild(i - l_node->num_key - 1), i);
            r_node->getchild(i - l_node->num_key - 1)->setparent(l_node);
        }
    }
    //If nonleaf is merged, for its children, r_node's children becomes l_node's children. They should be sibling
    if(l_node->is_leaf() == false)
    {
        l_node->getchild(l_node->num_key)->setnext(l_node->getchild(l_node->num_key + 1));
        l_node->getchild(l_node->num_key + 1)->setprev(l_node->getchild(l_node->num_key));
    }

    //l_node's right sibling update
    l_node->setnext( r_node->getnext() );
    //At last, update num of key
    l_node->num_key = l_node->num_key + 1 + r_node->num_key;

    //Case 3: the sepcial case
    //If parent node is root and there is only one key in root
    //Create a new root(Set l_node as root)
    if(parent->getparent() == NULL && parent->num_key == 1)
    {
        l_node->setparent(NULL);
        root = l_node;
        return;
    }

    //Set parent's key and children
    //left shift by 1
    for(int i = key_index; i < parent->num_key; i++)
        parent->setkey(parent->getkey(i + 1), i);
    parent->setkey(0 , parent->num_key);
    //left shift by 1
    for(int i = key_index + 1; i < parent->num_key + 1; i++)
        parent->setchild(parent->getchild(i + 1), i);
    parent->setchild(NULL , parent->num_key + 1);
    parent->num_key--;

}

template<class T, class G, class H>void Bptree<T,G,H>::prettyprint()
{
    _prettyprint(root, 0);
}

template<class T, class G, class H>void Bptree<T,G,H>::_prettyprint(Bpnode<T,G,H>* cur_node, int level)
{
    //for leaf, directly print it
    if(cur_node->is_leaf() == true)
    {
        //Indentation of current level
        for(int i = 0; i < level; i++)
        {
            cout << "   ";
        }
        
        cur_node->printkey();
        cout << " is leaf";
        // To print sibling of the leaf, uncomment the following
        //---------------------------------
        // if(cur_node->getprev() == NULL)
        // {
        //     cout << " with left sibling (NULL)";
        // }
        // else
        // {
        //     cout << " with left sibling ";
        //     cur_node->getprev()->printkey();
        // }
        // cout << " and ";
        // if(cur_node->getnext() == NULL)
        // {
        //     cout << " right sibling (NULL)";
        // }
        // else
        // {
        //     cout << "right sibling ";
        //     cur_node->getnext()->printkey();
        // }
        //------------------------------------
        //------------------------------------
        // To print the blocks of the leaf, uncomment the following
        cout << " with blocks ";
        cur_node->printblock();
        //------------------------------------
        cout << endl;
        return;
    }

    //Indentation of curretn level
    for(int i = 0; i < level; i++)
    {
        cout << "   ";
    }
    //Print current node information first
    cur_node->printkey();
    if(cur_node->is_leaf() == false)
    {
        cout << " with children ";
        for(int i = 0; i < cur_node->num_key + 1; i++)
        {
            cur_node->getchild(i)->printkey();
            cout << " ";
        }
        // To print nonleaf's sibling, uncomment the following
        //---------------------------------
        // cout << " and left sibling ";
        // if(cur_node->getprev() == NULL)
        // {
        //     cout << "(NULL) ";
        // }
        // else
        // {
        //     cur_node->getprev()->printkey();
        // }
        // cout << ", right sibling ";
        // if(cur_node->getnext() == NULL)
        // {
        //     cout << "(NULL) ";  
        // }
        // else
        // {
        //     cur_node->getnext()->printkey();
        // }
        //-----------------------------------
        cout << endl;
        //Then go to child of this node
        for(int i = 0; i < cur_node->num_key + 1; i++)
        {
            _prettyprint(cur_node->getchild(i), level + 1);
        }
    }

}
