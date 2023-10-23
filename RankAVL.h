#ifndef RANKAVL_H
#define RANKAVL_H

#include <iostream>
#include <ostream>
#include <cassert>
#include <memory>

using namespace std;
template <typename Key>
class RankAVL {
    struct node_t {
        Key key;
        int height;
        int num_players; //consider it the data for each level

        int sub_p; //total number of players in the subtree (rank)
        int sub_l; //sum of levels multiplied by their num of players

        bool isRightSon, isFirstNode;
        shared_ptr<node_t> left, right, parent;
        explicit node_t(Key key, int num_players = 0, bool isDummy = false) : key(key), height(1), num_players(num_players), sub_p(0), sub_l(0),
                                                               isRightSon(false), isFirstNode(isDummy), left(nullptr), right(nullptr), parent(nullptr) {}
        ~node_t() = default;
    };

    typedef shared_ptr<node_t> Node;
    Node root, it, first, last;
    int num_nodes;

    int getHeight(Node r) {
        if (!r)
            return 0;
        return r->height;
    }

    int getNumPlayers(Node r) {
        if (!r)
            return 0;
        return r->num_players;
    }

    int getSubP(Node r) {
        if (!r)
            return 0;
        return r->sub_p;
    }

    int getSubL(Node r) {
        if (!r)
            return 0;
        return r->sub_l;
    }

    int getBF(Node r) {
        return getHeight(r->left) - getHeight(r->right);
    }

    int max(int right, int left) {
        if (right > left)
            return right;
        return left;
    }

    void updateSubs(Node r){
        r->sub_p = getNumPlayers(r) + getSubP(r->right) + getSubP(r->left);
        r->sub_l = (getNumPlayers(r) * r->key) + getSubL(r->right) + getSubL(r->left);
    }

    void Rotate_LR(Node r) {
        assert(r && r->left && r->left->right);
        Node A = r, B = r->left, C = B->right, y = C->left, w = C->right, p = A->parent;
        bool A_was_right = A->isRightSon;

        //do the rotation.
        B->right = y;
        if (y) {
            y->parent = B;
            y->isRightSon = true;
        }
        A->left = w;
        if (w) {
            w->parent = A;
            w->isRightSon = false;
        }
        C->right = A;
        A->parent = C;
        A->isRightSon = true;
        C->left = B;
        B->parent = C;
        B->isRightSon = false;

        //update the parent of A to be the parent of C after C took A's place.
        if (p->isFirstNode) {
            p->right = C;
            p->left = C;
            C->parent = p;
        }
        else if (A_was_right) {
            p->right = C;
            C->parent = p;
            C->isRightSon = true;
        }
        else {
            p->left = C;
            C->parent = p;
            C->isRightSon = false;
        }

        //update the sub tree length of the rest of the affected nodes of the tree after the rotation.
        A->height = 1 + max(getHeight(A->right), getHeight(A->left));
        updateSubs(A);

        B->height = 1 + max(getHeight(B->right), getHeight(B->left));
        updateSubs(B);

        C->height = 1 + max(getHeight(C->right), getHeight(C->left));
        updateSubs(C);
    }

    void Rotate_LL(Node r) {
        assert(r && r->left);
        Node A = r, B = r->left, y = B->right, p = A->parent;
        bool A_was_right = A->isRightSon;


        //do the rotation.
        B->right = A;
        A->parent = B;
        A->isRightSon = true;
        A->left = y;
        if (y) {
            y->parent = A;
            y->isRightSon = false;
        }
        //update the parent of A to be the parent of B after B took A's place.
        if (p->isFirstNode) {
            p->right = B;
            p->left = B;
            B->parent = p;
        }
        else if (A_was_right) {
            p->right = B;
            B->parent = p;
            B->isRightSon = true;
        }
        else {
            p->left = B;
            B->parent = p;
            B->isRightSon = false;
        }
        A->height = 1 + max(getHeight(A->right), getHeight(A->left));
        updateSubs(A);

        B->height = 1 + max(getHeight(B->right), getHeight(B->left));
        updateSubs(B);
    }

    void Rotate_RL(Node r) {
        assert(r && r->right && r->right->left);
        Node A = r, B = r->right, C = B->left, y = C->left, w = C->right, p = A->parent;
        bool A_was_right = A->isRightSon;

        //do the rotation.
        B->left = w;
        if (w) {
            w->parent = B;
            w->isRightSon = false;
        }
        A->right = y;
        if (y) {
            y->parent = A;
            y->isRightSon = true;
        }
        C->left = A;
        A->parent = C;
        A->isRightSon = false;
        C->right = B;
        B->parent = C;
        B->isRightSon = true;

        //update the parent of A to be the parent of C after C took A's place.
        if (p->isFirstNode) {
            p->right = C;
            p->left = C;
            C->parent = p;
        }
        else if (A_was_right) {
            p->right = C;
            C->parent = p;
            C->isRightSon = true;
        }
        else {
            p->left = C;
            C->parent = p;
            C->isRightSon = false;
        }

        //update the sub tree length of the rest of the affected nodes of the tree after the rotation.
        A->height = 1 + max(getHeight(A->right), getHeight(A->left));
        updateSubs(A);

        B->height = 1 + max(getHeight(B->right), getHeight(B->left));
        updateSubs(B);

        C->height = 1 + max(getHeight(C->right), getHeight(C->left));
        updateSubs(C);
    }

    void Rotate_RR(Node r) {
        assert(r && r->right);
        Node A = r, B = A->right, x = B->left, p = A->parent;
        bool A_was_right = A->isRightSon;

        //do the rotation.
        B->left = A;
        A->parent = B;
        A->isRightSon = false;
        A->right = x;
        if (x) {
            x->parent = A;
            x->isRightSon = true;
        }

        //update the parent of A to be the parent of B after B took A's place.
        if (p->isFirstNode) {
            p->right = B;
            p->left = B;
            B->parent = p;
        }
        else if (A_was_right) {
            p->right = B;
            B->parent = p;
            B->isRightSon = true;
        }
        else {
            p->left = B;
            B->parent = p;
            B->isRightSon = false;
        }
        //update the sub tree length of the rest of the affected nodes of the tree after the rotation.
        A->height = 1 + max(getHeight(A->right), getHeight(A->left));
        updateSubs(A);

        B->height = 1 + max(getHeight(B->right), getHeight(B->left));
        updateSubs(B);
    }

    void Rotate(Node r) {
        if (!r)
            return;
        if (getBF(r) == 2 && getBF(r->left) == -1) {
            Rotate_LR(r);
            return;
        }
        if (getBF(r) == 2 && getBF(r->left) >= 0) {
            Rotate_LL(r);
            return;
        }
        if (getBF(r) == -2 && getBF(r->right) == 1) {
            Rotate_RL(r);
            return;
        }
        if (getBF(r) == -2 && getBF(r->right) <= 0) {
            Rotate_RR(r);
            return;
        }
    }

    void update_and_rotate(Node r) {
        if (!r)
            return;
        Node iterator = r, p = iterator->parent;
        while (!p->isFirstNode) {
            iterator->height = 1 + max(getHeight(iterator->left), getHeight(iterator->right));
            updateSubs(iterator);
            Rotate(iterator);
            iterator = p;
            p = iterator->parent;
        }
        iterator->height = 1 + max(getHeight(iterator->left), getHeight(iterator->right));
        updateSubs(iterator);
        Rotate(iterator);
    }

    bool isLeaf(Node r) {
        return !r->left && !r->right;
    }

    Node toFind(Node r, Key key_) {
        Node iterator = r;
        if(!r)
            return nullptr;
        while (!isLeaf(iterator)) {
            if (key_ == iterator->key)
                return iterator;

            if (key_ > iterator->key) {
                if (!iterator->right)
                    break;
                iterator = iterator->right;
                continue;
            }

            if (key_ < iterator->key) {
                if (!iterator->left)
                    break;
                iterator = iterator->left;
                continue;
            }
        }
        return iterator;
    }

    Node getMinNode(Node r) {
        if (!r)
            return NULL;
        Node iterator = r;
        while (iterator->left) {
            iterator = iterator->left;
        }
        return iterator;
    }

    Node copyTree(Node& r, Node& old_r, Node& p) {
        if (!old_r)
            return nullptr;
        r = Node(new node_t(Key(old_r->key), old_r->num_players));
        r->isRightSon = old_r->isRightSon;
        r->height = old_r->height;
        r->sub_p = old_r->sub_p;
        r->sub_l = old_r->sub_l;
        //r->num_players = old_r->num_players;
        r->parent = p;
        r->left = copyTree(r->left, old_r->left, r);
        r->right = copyTree(r->right, old_r->right, r);
        return r;
    }

    void ino(Node r, Key** keys, int** num_players, int* index){
        if(r == nullptr)
            return;
        ino(r->left,keys,num_players,index);

        (*keys)[*index] = Key(r->key);
        (*num_players)[*index] = getNumPlayers(r);
        (*index)++;

        ino(r->right,keys,num_players,index);
    }

    //Array to AVL.
    Node arr2avl(Key* arr1, int* arr2, int n, Node parent_, bool right_son) {
        if (n == 1) {
            Node to_ret = Node(new node_t(*arr1, *arr2));
            to_ret->isRightSon = right_son;
            to_ret->parent = parent_;
            updateSubs(to_ret);
            return to_ret;
        }
        if (n == 2) {
            Node p = Node(new node_t(*arr1, *arr2));
            Node s = Node(new node_t(arr1[1], arr2[1]));
            p->isRightSon = right_son;
            p->parent = parent_;
            p->height = 2;

            p->right = s;
            s->parent = p;
            s->isRightSon = true;

            updateSubs(s);
            updateSubs(p);
            return p;
        }
        int mid = n / 2;
        if (mid % 2 == 0) {
            if (n % 2 == 1)
                mid++;
            else mid--;
        }

        Node r = Node(new node_t(arr1[mid], arr2[mid]));
        r->parent = parent_;
        r->isRightSon = right_son;
        r->left = arr2avl(arr1, arr2, mid, r, false);
        r->right = arr2avl(arr1 + mid + 1,arr2 + mid + 1, n - mid - 1, r, true);
        r->height = 1 + max(getHeight(r->left), getHeight(r->right));
        updateSubs(r);
        return r;
    }

    void update_sub_size(Node r){
        if(!r)
            return;
        update_sub_size(r->left);
        update_sub_size(r->right);
        updateSubs(r);
    }



public:
    //create root dummy node, and set height to zero.
    RankAVL() {
        Node n = Node(new node_t(Key(), 0, true));
        root = n;
        num_nodes = 0;
        it = nullptr;
        first = nullptr;
        last = nullptr;
    }

    RankAVL(const RankAVL& old) : RankAVL() {
        copyTree(root->right, old.root->right, root);
        root->left = root->right;
        first = toFind(root->right, old.first->key);
        last = toFind(root->right, old.last->key);
        this->num_nodes = old.num_nodes;
    }

    RankAVL& operator=(const RankAVL& old) {
        if (this == &old)
            return *this;
        this->clear();
        if(old.isEmpty())
            return *this;
        copyTree(root->right, old.root->right, root);
        root->left = root->right;
        this->num_nodes = old.num_nodes;
        this->first = toFind(root->right, old.first->key);
        this->last = toFind(root->right, old.last->key);
        return *this;
    }

    ~RankAVL() {
        this->it = nullptr;
        this->first = nullptr;
        this->last = nullptr;
        this->clear();
        root = nullptr;
    }

    bool isEmpty() const{
        return  num_nodes == 0;
    }

    bool insert(Key key_, int num_players) {
        Node to_add = Node(new node_t(Key(key_), num_players));
        if (!to_add)
            return false;
        Node iterator = root->right;

        //Adds the new node as the root.
        if (isEmpty()) {
            root->left = to_add;
            root->right = to_add;
            to_add->parent = root;
            num_nodes++;
            first = to_add;
            last = to_add;
            return true;
        }

        //Add the new node in the right place.
        iterator = toFind(iterator, key_);
        if (iterator->key == to_add->key)
            return false;

        if (iterator->key < key_) {
            iterator->right = to_add;
            to_add->parent = iterator;
            to_add->isRightSon = true;
        }

        else if (iterator->key > key_) {
            iterator->left = to_add;
            to_add->parent = iterator;
            to_add->isRightSon = false;
        }

        update_and_rotate(to_add);
        num_nodes++;
        if (!first || first->key > key_) {
            first = to_add;
        }
        if (!last || last->key < key_) {
            last = to_add;
        }
        return true;
    }

    int find(Key key_) { //returns the num of players in the level
        if (isEmpty())
            return 0;
        Node n = toFind(root->right, key_);
        if (n && key_ == n->key){
            return n->num_players;
        }
        return 0;
    }

    int sumOfPlayersLessThan(Key key_){
        if (isEmpty()){
            return 0;
        }
        Node iterator = root->right;
        int counter = 0;
        while(iterator){
            if (key_ > iterator->key) {
                counter = counter + iterator->num_players + getSubP(iterator->left);
                iterator = iterator->right;
                continue;
            }
            if (key_ < iterator->key) {
                iterator = iterator->left;
                continue;
            }
            if (key_ == iterator->key) {
                //does not include num of players in the given level
                counter += getSubP(iterator->left);
                return counter;
            }
        }
        return counter;
    }

    bool findAndUpdate(Key key_, bool is_insert){ //called to do inc or dec to num of players
        Node n = toFind(root->right, key_);
        bool res = true;
        if((!n || key_ != n->key) && is_insert){
            res = insert(key_, 0);
            if(!res)
                return false;
        }
        Node r = toFind(root->right, key_);
        if(r) {
            if (is_insert) {
                increaseNumPlayers(r);
            } else {
                decreaseNumPlayers(r);
            }
            return true;
        }
        return false;
    }

    void increaseNumPlayers(Node r){
        r->num_players++;
        update_and_rotate(r);
    }

    void decreaseNumPlayers(Node r){
        r->num_players--;
        update_and_rotate(r);
        if(r->num_players == 0){
            remove(r->key);
        }
    }

    Node find_m(Node& r, int m, int zero_ps){ //finds the highest level node with the sub_p closest to m (sub_p>=m)
        Node iterator = r;
        if(!r)
            return nullptr;
        int total = getSubP(r) + zero_ps;
        if(total < m)
            return nullptr;
        if(total == m || getSubP(r) == m || (m - getSubP(r) < zero_ps && m - getSubP(r) > 0)) //because players with level 0 contribute 0 to the sum
            return r;
        bool stop = false;
        if(iterator->key >= m && !iterator->right)
            return iterator;
        while(!stop){
            if(getSubP(iterator) > m && iterator->right) {
                iterator = iterator->right;
            }
            else if(getSubP(iterator) == m){
                return iterator;
            }
            else{
                stop = true;
            }
        }
        if(getSubP(iterator) >= m)
            return iterator;
        return iterator->parent;
    }

    int select_m(int m, int zero_ps){
        int counter = 0, tmp = m;
        if(isEmpty()){
            if(m <= zero_ps) //only have players in level 0 or there are no players in the group at all
                return 0;
            return -1;
        }
        int total_players = getSubP(root->right) + zero_ps;
        if(total_players >= m && getSubP(root->right) <= m )
            return getSubL(root->right);
        if(total_players < m){
            return -1; //fail
        }
        Node it = find_m(root->right, tmp, zero_ps);
        if(!it){
            return -1; //fail
        }
        if(getSubP(it) == m)
            return getSubL(it);
//        if(getSubP(it) + zero_ps == m || getSubP(it) == m || m - getSubP(it) < zero_ps) //because players with level 0 contribute 0 to the sum
//            return getSubL(it);
        bool s = true;
        //if we are here then getSubP(it) > m
        while(tmp > 0){
            if(s) {
                if(tmp >= getSubP(it->right)){
                    counter += getSubL(it->right);
                    tmp -= getSubP(it->right);
                }
                else{
                    counter += (it->right->key * tmp);
                    tmp = 0;
                    continue;
                }
            }
            if(tmp > getNumPlayers(it)){
                counter = counter + (getNumPlayers(it) * (it->key));
                tmp -= getNumPlayers(it);
                if(it->left) {
                    Node t = it;
                    it = find_m(it->left, tmp, zero_ps);
                    if(t == it)
                        it = it->left;
                    s = true;
                }
                else {
                    it = it->parent;
                    s = false;
                }
            }
            else{
                counter += tmp * (it->key);
                tmp -= getNumPlayers(it);
            }
        }
        return counter;
    }

    void clear() {
        if(num_nodes == 0)
            return;
        Node parent = root;
        Node iterator = root->right;
        bool wasRight;
        while (!this->isEmpty()) {
            if (isLeaf(iterator)) {
                if (iterator == root->right) {
                    parent->right = nullptr;
                    parent->left = nullptr;
                    num_nodes--;
                    break;
                }
                wasRight = iterator->isRightSon;
                if (wasRight){
                    parent->right = nullptr;
                    num_nodes--;
                }
                else{
                    parent->left = nullptr;
                    num_nodes--;
                }
                iterator = parent;
                parent = iterator->parent;
                continue;
            }
            if (iterator->left) {
                parent = iterator;
                iterator = iterator->left;
                continue;
            }
            if (iterator->right) {
                parent = iterator;
                iterator = iterator->right;
                continue;
            }
        }
        this->it = nullptr;
        this->first = nullptr;
        this->last = nullptr;
        num_nodes = 0;
    }

    bool remove(Key key_) {
        if (isEmpty())
            return false;
        Node to_del = toFind(root->right, key_), p = to_del->parent;

        //the node to delete isn't found in the tree.
        if (to_del->key != key_)
            return false;

        //the node to delete is a leaf; has no sons.
        if (isLeaf(to_del)) {
            if (p == root) {
                p->right = nullptr;
                p->left = nullptr;
                to_del->parent = nullptr;
                first = nullptr;
                last = nullptr;
                num_nodes--;
                return true;
            }
            if (to_del == first) {
                first = p;
            }
            if (to_del == last) {
                last = p;
            }
            if (to_del->isRightSon) {
                p->right = nullptr;
            }
            if (!to_del->isRightSon) {
                p->left = nullptr;
            }
            num_nodes--;
            update_and_rotate(p);
            return true;
        }

        //the node to_delete has 2 sons (left & right)
        if (to_del->right && to_del->left) {
            Node to_swap = getMinNode(to_del->right), p_ = to_swap->parent, update_from = to_swap;
            bool swap_is_son = p_ == to_del, update_from_swap = swap_is_son;
            if (to_swap->isRightSon) {
                p_->right = nullptr;
            }
            else p_->left = nullptr;

            //attach the minimum key node in the right sub tree in place of the node to_delete.
            if (swap_is_son) {
                to_swap->left = to_del->left;
                to_swap->parent = to_del->parent;
                to_swap->isRightSon = to_del->isRightSon;
            }
            else {
                if (!to_swap->right) {
                    to_swap->right = to_del->right;
                }
                else {
                    Node tmp = to_swap->right;
                    p_->left = tmp;
                    tmp->isRightSon = false;
                    //tmp->right->parent = tmp;
                    tmp->parent = p_;
                    //to_swap->right = nullptr;
                    to_swap->right = to_del->right;
                    update_from_swap = true;
                    update_from = tmp;
                }
                to_swap->left = to_del->left;
                to_swap->parent = to_del->parent;
                to_swap->isRightSon = to_del->isRightSon;
            }
            //attach the parent to have the new node as son in place of the node to_delete.
            if (p == root) {
                p->right = to_swap;
                p->left = to_swap;
            }
            else if (to_del->isRightSon) {
                p->right = to_swap;
            }
            else if (!to_del->isRightSon) {
                p->left = to_swap;
            }

            //detach the right, left and parent of the node to_delete.
            to_del->right = nullptr;
            to_del->left = nullptr;
            to_del->parent = nullptr;

            //update the sons to point at new parent.
            if (to_swap->right)
                to_swap->right->parent = to_swap;
            if (to_swap->left)
                to_swap->left->parent = to_swap;

            //update the tree, starting from the parent of the minimum node before we swapped.
            if (!swap_is_son && !update_from_swap)
                update_and_rotate(p_);
            else update_and_rotate(update_from);
            num_nodes--;
            return true;
        }

            //the node to_delete has one son (left / right)
        else {
            if (to_del == first) {
                first = to_del->right;
                while (first->left) {
                    first = first->left;
                }
            }
            if (to_del == last) {
                last = to_del->left;
                while (last->right) {
                    last = last->right;
                }
            }
            Node sub;
            if (to_del->left)
                sub = to_del->left;
            else if (to_del->right)
                sub = to_del->right;
            sub->parent = p;
            if (p == root) {
                p->right = sub;
                p->left = sub;
            }
            else {
                if (to_del->isRightSon) {
                    p->right = sub;
                    sub->isRightSon = true;
                }
                else {
                    p->left = sub;
                    sub->isRightSon = false;
                }
            }
            to_del->right = nullptr;
            to_del->left = nullptr;
            to_del->parent = nullptr;
            update_and_rotate(sub);
            num_nodes--;
            return true;
        }
    }

    int nodeNum() {
        return num_nodes;
    }

//    T* getNext() {
//        assert(first && it); //TODO
//        if (num_nodes == 1 || it == last) {
//            return nullptr;
//        }
//        if (it->parent == root) {
//            if (it->right) {
//                it = it->right;
//                while (it->left) {
//                    it = it->left;
//                }
//                return &it->data;
//            }
//            else {
//                return nullptr;
//            }
//        }
//
//        if (it->right) {
//            it = it->right;
//            while (it->left) {
//                it = it->left;
//            }
//            return &it->data;
//        }
//
//        if (!it->isRightSon && it->parent != root && !it->right) {
//            it = it->parent;
//            return &it->data;
//        }
//        if (it->isRightSon) {
//            while (it->key > it->parent->key) {
//                it = it->parent;
//            }
//            it = it->parent;
//            return &it->data;
//        }
//        return nullptr;
//    }

    int* getFirstNumPlayers() {
        if (this->isEmpty())
            return nullptr;
        if (first) {
            it = first;
            return &first->num_players;
        }
        return nullptr;
    }

    int* getLastNumPlayers() {
        if (this->isEmpty())
            return nullptr;
        if (last)
            return &last->num_players;
        return nullptr;
    }

//    T* getCurrent() {
//        if (!it)
//            return nullptr;
//        return &it->data;
//    }

    Key* getFirstKey() {
        if (this->isEmpty())
            return nullptr;
        if (first)
            return &first->key;
        return nullptr;
    }

    Key* getLastKey() {
        if (this->isEmpty())
            return nullptr;
        if (last)
            return &last->key;
        return nullptr;
    }

//    Key* getCurrentKey() {
//        if (!it)
//            return nullptr;
//        return &it->key;
//    }

    int* getTotalNumPlayers() {
        if (this->isEmpty())
            return nullptr;
        if (root)
            return &root->sub_p;
        return nullptr;
    }

    void avl2arr(Key** keys, int** num_players){
        int index = 0;
        ino(root->right,keys,num_players,&index);
    }

    //takes in 4 arrays, their sizes, and 2 more for output
    void merge(Key** keys1, int** data1, int n1, Key** keys2, int** data2, int n2, Key** keys, int** data, int* n){
        int i1=0, i2=0, index=0, counter=0;
        while(i1 < n1 && i2 < n2){
            if((*keys1)[i1] < (*keys2)[i2]){
                (*keys)[index] = (*keys1)[i1];
                (*data)[index] = (*data1)[i1];
                i1++;
                index++;
            }
            else if((*keys1)[i1] > (*keys2)[i2]){
                (*keys)[index] = (*keys2)[i2];
                (*data)[index] = (*data2)[i2];
                i2++;
                index++;
            }
            else if((*keys1)[i1] == (*keys2)[i2]){ //if have same level
                (*keys)[index] = (*keys1)[i1];
                (*data)[index] = (*data1)[i1] + (*data2)[i2];
                i1++;
                i2++;
                index++;
                counter++;
            }
        }
        if(i1 < n1 && i2 == n2){
            while(i1 < n1){
                (*keys)[index] = (*keys1)[i1];
                (*data)[index] = (*data1)[i1];
                i1++;
                index++;
            }
        }
        else if(i1 == n1 && i2 < n2){
            while(i2 < n2){
                (*keys)[index] = (*keys2)[i2];
                (*data)[index] = (*data2)[i2];
                i2++;
                index++;
            }
        }
        *n = n1 + n2 - counter;
    }

    template <typename Key_>
    friend RankAVL<Key_> RankArrToAVL(Key_* keys, int* vals, int n);

    template <typename Key_>
    friend RankAVL<Key_> mergeRankTrees(RankAVL<Key_>* tree1, RankAVL<Key_>* tree2);
};

template <typename Key_>
RankAVL<Key_> RankArrToAVL(Key_* keys, int* vals, int n) {
    RankAVL<Key_> to_ret = RankAVL<Key_>();
    to_ret.root->right = to_ret.arr2avl(keys, vals, n, to_ret.root, true);
    to_ret.root->left = to_ret.root->right;
    to_ret.it = nullptr;
    to_ret.first = to_ret.toFind(to_ret.root->right, keys[0]);
    to_ret.last = to_ret.toFind(to_ret.root->right, keys[n - 1]);
    to_ret.num_nodes = n;
    to_ret.update_sub_size(to_ret.root->right);
    return to_ret;
}

template <typename Key_>
RankAVL<Key_> mergeRankTrees(RankAVL<Key_>* tree1, RankAVL<Key_>* tree2){
    //merge this and tree
    int size1 = tree1->nodeNum();
    Key_ *key_arr1 = new Key_[size1];
    int *data_arr1 = new int[size1];
    tree1->avl2arr(&key_arr1, &data_arr1);
    tree1->clear();

    int size2 = tree2->nodeNum();
    Key_ *key_arr2 = new Key_[size2];
    int *data_arr2 = new int[size2];
    tree2->avl2arr(&key_arr2, &data_arr2);
    tree2->clear();

    int size = size1 + size2;
    Key_ *key_arr = new Key_[size];
    int *data_arr = new int[size];
    int* new_size = new int;

    tree1->merge(&key_arr1, &data_arr1, size1, &key_arr2, &data_arr2, size2, &key_arr, &data_arr, new_size);

    //*new_size is less than or equal to size
    Key_ *final_keys = new Key_[*new_size];
    int *final_data = new int[*new_size];
    for(int i = 0; i < *new_size; i++){
        final_keys[i] = key_arr[i];
        final_data[i] = data_arr[i];
    }

    RankAVL<Key_> merged_tree = RankArrToAVL(final_keys, final_data, *new_size);

    delete[](key_arr1);
    delete[](data_arr1);
    delete[](key_arr2);
    delete[](data_arr2);
    delete[](key_arr);
    delete[](data_arr);
    delete[](final_keys);
    delete[](final_data);
    delete new_size;

    return merged_tree;
}

#endif //RANKAVL_H