
#include<iostream>
#include"Bptree.h"
using namespace std;

template<class T, class G, class H>Bpnode<T,G,H>::Bpnode(int node_order)
{
    leaf_status = false;
    order = node_order;
    num_key = 0;
    max_key_num = order - 1;
    min_key_num = (order + 1) / 2 - 1;
    int max_children_num = order;

    // +1 是因为最后一位暂存一个overflow的key
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

//It's better to test this function more
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

template<class T, class G, class H>block<T,G,H>* Bptree<T,G,H>::retrieve(G pri_key)
{   
    return _retrieve(root, pri_key);
}

template<class T, class G, class H>block<T,G,H>* Bptree<T,G,H>::_retrieve(Bpnode<T,G,H>* cur_node, G pri_key)
{
    int index = cur_node->retrieve_binary_search(pri_key);
    //如果不是leaf，继续找
    if(cur_node->is_leaf() == false)
    {
        Bpnode<T,G,H>* child = cur_node->getchild(index);
        return _retrieve(child,pri_key);
    }
    //如果是leaf，找到对应的blockptr
    if(cur_node->is_leaf() == true)
    {
        block<T,G,H>* blockptr = cur_node->getblock(index);
        return blockptr;
    }
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
    //如果还是nonleaf，继续找
    if(cur_node->is_leaf() == false)
    {
        int child_index = cur_node->child_binary_search(pri_key);
        Bpnode<T,G,H>* child = cur_node->getchild(child_index);
        _insert(child,pri_key,blockptr);
    }
    if(cur_node->is_leaf() == true)
    {
        //找到要插入的点
        //key 往后移一位,插入 pri_key
        int key_index = cur_node->child_binary_search(pri_key);
        cur_node->num_key++;
        for(int i = cur_node->num_key - 1; i > key_index; i--)
        {
            cur_node->setkey(cur_node->getkey(i - 1), i);  
        }
        cur_node->setkey(pri_key, key_index);

        //block 往后移一位,插入blockptr
        int block_index = cur_node->child_binary_search(pri_key);

        for(int i = cur_node->num_key; i > block_index; i--)
        {
            cur_node->setblock(cur_node->getblock(i - 1) , i);
        }
        cur_node->setblock(blockptr,block_index);
        //如果当前叶结点满了，需要split
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
    // new_root_mark means a new root is created
    bool new_root_mark = false;    
    if(parent == NULL)
    {
        parent = new Bpnode<T,G,H>(order);
        //New parent is not leaf (defined by constructor)
        parent->setparent(NULL);    
        new_root_mark = true;
    }

    G mid_key = node->getkey(node->num_key / 2);
    //新建两个结点,拥有原结点的信息
    Bpnode<T,G,H>* l_node = new Bpnode<T,G,H>(order);
    Bpnode<T,G,H>* r_node = new Bpnode<T,G,H>(order);

    //leaf status and siblings
    if(node->is_leaf() == true)
    {
        l_node->setleaf(true);
        r_node->setleaf(true);
    }

    //设立l_node的相关信息
    for(int i = 0; i < node->num_key / 2; i++)
    {
        l_node->setkey(node->getkey(i), i);
        l_node->num_key++;
    }
    //node->num_key = 5
    //node->num_key / 2 = 2
    //i = 0,1,2
    for(int i = 0; i < node->num_key / 2 + 1; i++)
    {   
        if(node->is_leaf() == true)
            l_node->setblock(node->getblock(i),i);
        if(node->is_leaf() == false)
        {
            l_node->setchild(node->getchild(i),i);
            //若要使所有leaf相连，请删去一下语句
            if(i == node->num_key / 2)
                node->getchild(i)->setnext(NULL);
        }

    }
    l_node->setparent(parent);

    //设立r_node的相关信息
    for(int i = node->num_key / 2 + 1; i < node->num_key; i++)
    {
        r_node->setkey(node->getkey(i), i - (node->num_key / 2 + 1));
        r_node->num_key++;
    }

    for(int i = node->num_key / 2 + 1; i < node->num_key + 1; i++)
    {
        if(node->is_leaf() == true)
            r_node->setblock(node->getblock(i), i - (node->num_key / 2 + 1));
        if(node->is_leaf() == false)
        {
            r_node->setchild(node->getchild(i), i - (node->num_key / 2 + 1));
            //若要使所有leaf相连，请删去一下语句    
            if(i == node->num_key / 2 + 1)
                node->getchild(i)->setprev(NULL);
        }
    }
    r_node->setparent(parent);

    //如果l_node, r_node不是leaf，他们的children的parent也应修改
    if(node->is_leaf() == false)
    {   
        //i = 0,1,2
        for(int i = 0; i < l_node->num_key + 1; i++)
        {
            l_node->getchild(i)->setparent(l_node);
        }
        for(int i = 0; i < r_node->num_key + 1; i++)
        {
            r_node->getchild(i)->setparent(r_node);
        }
    }

    //设立parent的相关信息
    //把mid_key插入到parent node里,并设置parent node的children 类似于_insert但不同
    int key_index = parent->child_binary_search(mid_key);

    //parent key 后移一位
    parent->num_key++;
    for(int i = parent->num_key - 1; i > key_index; i--)
    {
        parent->setkey(parent->getkey(i - 1), i);  
    }
    parent->setkey(mid_key, key_index);

    int child_index = parent->child_binary_search(mid_key);
    //parent的child 后移一位
    for(int i = parent->num_key; i > child_index; i--)
    {
        parent->setchild(parent->getchild(i - 1) ,i);
    }

    parent->setchild(l_node, child_index - 1);
    parent->setchild(r_node,child_index);

    //设置l_node，r_node的sibling
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
    if(new_root_mark == true)
    {
        this->root = parent;
    }
    //如果parent也满了，就再继续split
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
    //如果当前不是leaf，继续寻找
    int index = cur_node->child_binary_search(oldpri_key);
    if(cur_node->is_leaf() == false)
    {
        Bpnode<T,G,H>* child = cur_node->getchild(index);
        _delete1(child, oldpri_key, newpri_key);
    }
    //如果是leaf，二分找到对应的oldpri_key,更新为newpri_key
    if(cur_node->is_leaf() == true)
    {
        cur_node->printkey();
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
    //如果还是nonleaf，继续找
    if(cur_node->is_leaf() == false)
    {
        int child_index = cur_node->child_binary_search(pri_key);
        Bpnode<T,G,H>* child = cur_node->getchild(child_index);
        _delete2(child, pri_key, blockptr);
    }
    if(cur_node->is_leaf() == true)
    {
        //如果是root，且只有一个block剩余，无法删除
        if(cur_node->getparent() == NULL && cur_node->num_key == 0)
        {
            cout << "There is only one block in root. Delete fail!";
            exit(EXIT_FAILURE);
        }
        //delete the key and blockptr
        int key_index = cur_node->key_binary_search(pri_key);
        int block_index = cur_node->block_search(blockptr);
        if(key_index - block_index > 1 || block_index - key_index > 1)
        {
            //Not very certain, just for test
            cout << "Primary Key and Block pointer doesn't match!" << endl;
            exit(EXIT_FAILURE);
        }
        //key 左移一位
        cur_node->num_key--;
        for(int i = key_index; i < cur_node->num_key; i++)
        {
            cur_node->setkey( cur_node->getkey(i + 1), i);
        }
        cur_node->setkey(0,cur_node->num_key);
        //block左移一位
        for(int i = block_index; i < cur_node->num_key + 1; i++)
        {
            cur_node->setblock( cur_node->getblock(i + 1), i);
        }
        cur_node->setblock(NULL,cur_node->num_key + 1);

        //如果少于min key num， 且不是root，merge
        if(cur_node->getparent() != NULL && cur_node->num_key < cur_node->min_key_num)
        {
            merge(cur_node);
        }
    }
}

template<class T, class G, class H>void Bptree<T,G,H>::merge(Bpnode<T,G,H>* node)
{
    Bpnode<T,G,H>* parent = node->getparent();

    //case 1如果有多于min_key_num的sibling，优先右
    if(node->getnext() != NULL)
    {
        if(node->getnext()->num_key > node->min_key_num)
        {
            merge1(node,node->getnext());
            return;
        }
    }
    if(node->getprev() != NULL)
    {
        if(node->getprev()->num_key > node->min_key_num)
        {
            merge1(node->getprev(),node);
            return;
        }
    }
    //case 2如果没有多于min_key_num的sibling,同时parent的separating key不是the only key（优先右）
    if(node->getnext() != NULL)
    {
        merge2(node,node->getnext());
        //如果merge后parent也需要merge，继续（同时parent不能是root)
        if(parent->getparent() != NULL && parent->num_key < parent->min_key_num)
            merge(parent);
        return;
    }
    if(node->getprev() != NULL)
    {
        merge2(node->getprev(), node);
        //如果merge后parent也需要merge，继续（同时parent不能是root)
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

    //保存原来两个node key的数目，在更新block时使用
    int leftkey_num = l_node->num_key;
    int rightkey_num = r_node->num_key;
    //Find separating key. key的index和左node的index相同
    int key_index = parent->child_search(l_node);   
    //Set up key array
    int array_size = l_node->num_key + 1 + r_node->num_key;
    G* key_array = new G[array_size];
    for(int i = 0; i < l_node->num_key; i++)
        key_array[i] = l_node->getkey(i);
    key_array[l_node->num_key] = parent->getkey(key_index);
    for(int i = l_node->num_key + 1; i < array_size; i++)
        key_array[i] = r_node->getkey(i - (l_node->num_key + 1));

    //左node设置key，首先清零
    for(int i = 0; i < l_node->num_key; i++)
        l_node->setkey(0, i);
    for(int i = 0; i < (array_size - 1) / 2; i++)
        l_node->setkey(key_array[i],i);
    l_node->num_key = (array_size - 1) / 2;

    //设置parent
    parent->setkey(key_array[(array_size - 1) / 2], key_index);    
    //右node设置key，首先清零
    for(int i = 0; i < r_node->num_key; i++)
        r_node->setkey(0,i);
    for(int i = (array_size + 1) / 2; i < array_size; i++)
        r_node->setkey(key_array[i], i - (array_size + 1) / 2);
    r_node->num_key = (array_size - (array_size + 1) / 2);

    //设置block array和child array，分别用于leaf和nonleaf
    int block_array_size = leftkey_num + 1 + rightkey_num + 1;
    //child_array_size和block array size一样，只不过为了方便区分不困惑
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
    //设置左node block/child,首先清零
    for(int i = 0; i < leftkey_num + 1; i++)
    {
        if(l_node->is_leaf() == true)
            l_node->setblock(NULL, i);
        if(l_node->is_leaf() == false)
            l_node->setchild(NULL, i);
    }
    //再重新赋予
    for(int i = 0; i < block_array_size / 2; i++)
    {
        if(l_node->is_leaf() == true)
            l_node->setblock(block_array[i], i);
        if(l_node->is_leaf() == false)
        {
            //children的信息最好重新设置一下,否则可能有点乱
            Bpnode<T,G,H>* child = child_array[i];
            l_node->setchild(child, i);
            child->setparent(l_node);
            //第一个child的prev应设为NULL
            if(i == 0)
                child->setprev(NULL);
            if(i != 0)
                child->setprev(child_array[i - 1]);
            //最后一个child的next应设为NULL
            if(i == block_array_size / 2 - 1)
                child->setnext(NULL);
            if(i != block_array_size / 2 - 1)
                child->setnext(child_array[i + 1]);
        }
    }

    //设置右node block/child，首先清零
    for(int i = 0; i < rightkey_num + 1; i++)
    {
        if(l_node->is_leaf() == true)
            r_node->setblock(NULL,i);
        if(l_node->is_leaf() == false)
            r_node->setchild(NULL, i);
    }
    //再重新赋予
    for(int i = block_array_size / 2; i < block_array_size; i++)
    {
        if(l_node->is_leaf() == true)
            r_node->setblock(block_array[i], i - block_array_size / 2);
        if(l_node->is_leaf() == false)
        {
            //同l_node, children的信息最好重新设置一下,否则可能有点乱
            Bpnode<T,G,H>* child = child_array[i];
            r_node->setchild(child, i - child_array_size / 2);
            child->setparent(r_node);
            //第一个child的prev应设为NULL
            if(i == child_array_size / 2)
                child->setprev(NULL);
            if(i != child_array_size / 2)
                child->setprev(child_array[i - 1]);
            //最后一个child的next应设为NULL
            if(i == child_array_size - 1)
                child->setnext(NULL);
            if(i != child_array_size - 1)
                child->setnext(child_array[i + 1]);            
        }
    }
}

//把r_node merge到l_node
template<class T, class G, class H>void Bptree<T,G,H>::merge2(Bpnode<T,G,H>* l_node, Bpnode<T,G,H>* r_node)
{
    Bpnode<T,G,H>* parent = l_node->getparent();
    //key的index和左node的index相同
    int key_index = parent->child_search(l_node);   

    //设置l_node 的key
    l_node->setkey(parent->getkey(key_index), l_node->num_key);
    for(int i = l_node->num_key + 1; i < l_node->num_key + 1 + r_node->num_key; i++)
    {
        l_node->setkey(r_node->getkey(i - (l_node->num_key + 1)) , i);
    }

    //设置l_node 的block/child
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

    //对nonleaf merge后的child来说，r_node的children改为了l_node的children，应加上sibling标记
    if(l_node->is_leaf() == false)
    {
        l_node->getchild(l_node->num_key)->setnext(l_node->getchild(l_node->num_key + 1));
        l_node->getchild(l_node->num_key + 1)->setprev(l_node->getchild(l_node->num_key));
    }

    //l_node 的sibling
    l_node->setnext( r_node->getnext() );
    //最后在更新num_key
    l_node->num_key = l_node->num_key + 1 + r_node->num_key;

    //如果时的parent为root，且只有一个key
    //此时的变为新的root
    if(parent->getparent() == NULL && parent->num_key == 1)
    {
        l_node->setparent(NULL);
        root = l_node;
        return;
    }

    //设置parent的key和child,左移一位
    for(int i = key_index; i < parent->num_key; i++)
        parent->setkey(parent->getkey(i + 1), i);
    parent->setkey(0 , parent->num_key);
    for(int i = key_index + 1; i < parent->num_key + 1; i++)
        parent->setchild(parent->getchild(i + 1), i);
    parent->setchild(NULL , parent->num_key + 1);
    parent->num_key--;

}

template<class T, class G, class H>void Bptree<T,G,H>::prettyprint()
{
    _prettyprint(root);
}

template<class T, class G, class H>void Bptree<T,G,H>::_prettyprint(Bpnode<T,G,H>* cur_node)
{
    //for leaf, directly print it
    if(cur_node->is_leaf() == true)
    {
        cout << "   ";
        cur_node->printkey();
        cout << " is leaf";
        //若需要print sibling，请取消以下注释
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
        //若需要print block，请取消一下注释
        cout << " with blocks ";
        cur_node->printblock();
        //------------------------------------
        cout << endl;
        return;
    }
    //print current node information first
    cur_node->printkey();
    if(cur_node->is_leaf() == false)
    {
        cout << " with children ";
        for(int i = 0; i < cur_node->num_key + 1; i++)
        {
            cur_node->getchild(i)->printkey();
            cout << " ";
        }
        //若需要print sibling，请取消以下注释
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
            cout << "   ";
            _prettyprint(cur_node->getchild(i));
        }
    }

}

