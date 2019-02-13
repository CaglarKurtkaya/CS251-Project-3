#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <vector>
#include <stdlib.h> //abs
using namespace std;

template <typename T>
class bst {

private:
    struct bst_node {
        T      val;
        bst_node *left;
        bst_node *right;
        int numLeft;   // bookkeeping num of nodes in left subtree
        int numRight;	 // bookkeeping num of nodes in right subtree
        int height;	// book keeping for height

        bst_node ( const T & _val = T{}, bst_node * l = nullptr, bst_node *r = nullptr, int m = 0 , int k = 0, int h = 1)
            : val { _val },  left { l }, right {r} , numLeft { m }, numRight { k }, height { h }
        { }
    };
    
    int num_node;   // total num of nodes in the class
    int max_node;	// stores the node with max val in it
    int min_node;	// stores the node with min val in it
    
    


public:
    // constructor:  initializes an empty tree
    bst(){
        root = nullptr;
        num_node = 0;
        max_node = -999999;
        min_node = 999999;
    }

private:
    // helper function which recursively deallocates nodes
    //   in a tree.
    static void delete_nodes(bst_node *r){
        if(r==nullptr) return;
        delete_nodes(r->left);
        delete_nodes(r->right);
        delete r;
    }

public:
    // destructor
    ~bst() {
        delete_nodes(root);
    }

private:

    /**
 * function:  insert()
 * desc:      recursive helper function inserting x into
 *            binary search tree rooted  at r.
 *
 * returns:   pointer to root of tree after insertion.
 *
 * notes:     if x is already in tree, no modifications are made.
 */
    // A utility function to get the height of the tree
    static int height1(bst_node *N)
    {
        if (N == nullptr)
            return 0;
        return N->height;
    }

    // A utility function to get maximum of two integers
    static int max1(int a, int b)
    {
        return (a > b)? a : b;
    }

    static bst_node * rightRotate(bst_node *y) {
        struct bst_node *x = y->left;
        struct bst_node *T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = max1( height1(y->left), height1(y->right) )+1;
        x->height = max1( height1(x->left), height1(x->right) )+1;

        // Return new root
        return x;
    }

    // A utility function to left rotate subtree rooted with x
    static bst_node * leftRotate( bst_node  *x) {
        struct bst_node *y = x->right;
        struct bst_node *T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        //  Update heights
        x->height = max1( height1(x->left), height1(x->right) )+1;
        y->height = max1( height1(y->left), height1(y->right) )+1;

        // Return new root
        return y;
    }

    // Get Balance factor of node
    static int getBalance(bst_node *r)
    {
        if (r == nullptr)
            return 0;
        return height1(r->left) - height1(r->right);
    }


    // helper recursive for insert()
    static bst_node * _insert(bst_node *r, T & x, bool &success){

        if(r == nullptr){
            success = true;
            return new bst_node(x, nullptr, nullptr);
        }

        if(r->val == x){
            success = false;
            return r;
        }

        if(x < r->val){
            r->left = _insert(r->left, x, success);
            r->numLeft++;    // increase the nodes on the right subtree
        }
        else {
            r->right = _insert(r->right, x, success);
            r->numRight++;  // increase the nodes on the right subtree
        }

        /* 2. Update height of this ancestor node */
        r->height = 1 + max1( height1(r->left), height1(r->right) );

        /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
        int balance = getBalance(r);

        // If this node becomes unbalanced, then
        // there are 4 cases

        // Left Left Case
        if (balance > 1 && x < r->left->val )
            return rightRotate(r);

        // Right Right Case
        if (balance < -1 && x > r->right->val )
            return leftRotate(r);

        // Left Right Case
        if (balance > 1 && x > r->left->val)
        {
            r->left =  leftRotate(r->left);
            return rightRotate(r);
        }

        // Right Left Case
        if (balance < -1 && x < r->right->val)
        {
            r->right = rightRotate(r->right);
            return leftRotate(r);
        }

        /* return the (unchanged) node pointer */
        return r;
    }
    


public:
    /**
   * function:  insert
   * desc:      inserts x into BST given by t.  Note that
   *            a BST stores a SET -- no duplicates.  Thus,
   *            if x is already in t when call made, no
   *            modifications to tree result.
   *
   * note:      helper function does most of the work.
   *
   */
    bool insert(T & x){
        bool success;
        root = _insert(root, x, success);
        this->num_node++;
        if(x > this->max_node)
            this->max_node = x;

        if(x < this->min_node)
            this->min_node = x;

        return success;
    }

    /**
 * function:  contains()
 * desc:      returns true or false depending on whether x is an
 *            element of BST (calling object)
 *
 */
    bool contains(const T & x){
        bst_node *p = root;

        while(p != nullptr){
            if(p->val == x)
                return true;
            if(x < p->val){
                p = p->left;
            }
            else
                p = p->right;
        }
        return false;
    }

private:
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _min_node(bst_node *r ){
        if(r==nullptr)
            return nullptr; // should never happen!
        while(r->left != nullptr)
            r = r->left;
        return r;
    }

    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _max_node(bst_node *r ){
        if(r==nullptr)
            return nullptr; // should never happen!
        while(r->right != nullptr)
            r = r->right;
        return r;
    }

    // recursive helper function for node removal
    //   returns root of resulting tree after removal.
    static bst_node * _remove(bst_node *r, T & x, bool &success){
        bst_node *tmp;
        bool sanity;

        if(r==nullptr){
            success = false;
            return nullptr;
        }
        if(r->val == x){
            success = true;

            if(r->left == nullptr){
                tmp = r->right;
                delete r;
                return tmp;
            }

            if(r->right == nullptr){
                tmp = r->left;
                delete r;
                return tmp;
            }
            // if we get here, r has two children
            r->val = _min_node(r->right)->val;
            r->right = _remove(r->right, r->val, sanity);
            // decrement the node on the right
            r->numRight--;
            if(!sanity)
                std::cerr << "ERROR:  remove() failed to delete promoted value?\n";
            return r;
        }
        if(x < r->val){
            r->left = _remove(r->left, x, success);
            // decrement the node on the left
            r->numLeft--;
        }
        else {
            r->right = _remove(r->right, x, success);
            // decrement the node on the right
            r->numRight--;
        }
        return r;

    }

public:

    bool remove(T & x){
        bool success;
        root = _remove(root, x, success);
        if(success != 0){
            this->num_node--;
            if(x == this->max_node)
                this->max_node = _min_node(root)->val;

            if(x == this->min_node)
                this->max_node = _max_node(root)->val;
        }

        return success;
    }


private:
    // recursive helper function to compute size of
    //   tree rooted at r
    static int _size(bst_node *r){
        if(r==nullptr) return 0;
        return _size(r->left) + _size(r->right) + 1;
    }

public:
    int size() {
        //return _size(root);
        return this->num_node;
    }

private:

    static int _height(bst_node *r){
        int l_h, r_h;

        if(r==nullptr) return -1;
        l_h = _height(r->left);
        r_h = _height(r->right);
        return 1 + (l_h > r_h ? l_h : r_h);
    }

public:

    int height() {
        return _height(root);
    }
    
    static int _height1(bst_node *r){
        int l_h, r_h;

        if(r==nullptr) return -1;
        l_h = _height(r->left);
        r_h = _height(r->right);
        return 1 + (l_h > r_h ? l_h : r_h);
    }
    
    

    bool min(T & answer) {
        if(root == nullptr){
            return false;
        }
        answer = this->min_node;
        return true;
    }

    T max() {
        //return _max_node(root)->val;
        return this->max_node;
    }

    /******************************************
     *
     * "stubs" for assigned TODO functions below
     *
     *****************************************/

    // TODO
    // helper recursive func for to_vector
    void returnInOrder(bst_node *r,vector<T> *copy_vector){
        if(r == nullptr){
            return;
        }
        returnInOrder(r->left, copy_vector);
        copy_vector->push_back(r->val);
        returnInOrder(r->right, copy_vector);

    }
    
    vector<T> * to_vector() {
        vector<T> *copy_vector = new vector <T>;
        returnInOrder(root, copy_vector);

        return copy_vector;
    }


    /* TODO
     * Function:  get_ith
     * Description:  determines the ith smallest element in t and
     *    "passes it back" to the caller via the reference parameter x.
     *    i ranges from 1..n where n is the number of elements in the
     *    tree.
     *
     *    If i is outside this range, false is returned.
     *    Otherwise, true is returned (indicating "success").
     *
     * Runtime:  O(h) where h is the tree height
     */
private:

    static void help_to_get_ith(bst_node *t, int i, T &x) {
        if(t==nullptr)
            return;
        // if the current node is the ith smallest
        if(t->numLeft + 1 == i){
            x = t->val;
            return;
        }
        // check right side
        if(t->numLeft < i) { //
            return help_to_get_ith(t->right, i - t->numLeft - 1, x);
        }
        else {
            // check left side
            return help_to_get_ith(t->left, i, x);
        }
    }

public:

    bool get_ith(int i, T &x) {
        // check if i is valid
        int s = this->num_node; // s is total number of nodes in tree
        if( i < 1 || i > s ){
            return false;
        }
        // call recursive helper func
        help_to_get_ith(root, i , x);
        return true;
    }


    bool get_ith_SLOW(int i, T &x) {
        int n = size();
        int sofar=0;

        if(i < 1 || i > n)
            return false;

        _get_ith_SLOW(root, i, x, sofar);
        return true;
    }


    /* TODO
     * Function:  num_geq
     * Description:  returns the number of elements in tree which are
     *       greater than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     */

    int help_num_geq(bst_node * t, const T &x) {

        if(t == nullptr) {
            return 0;
        }

        if(t->val == x){
            return  1 + t->numRight;
        }
        //count all right nodes
        //and send it to left
        if(t->val < x){
            // go right
            return help_num_geq(t->right, x);
        }
        else{
            //get count of right and go left
            return 1 + t->numRight + help_num_geq(t->left, x);
        }
    }


    static void help_num_geq_xxx1(bst_node * t, const T &x, int &count) {
        if(t == nullptr)
            return 0;

        if(t->val == x){
            count++;
            return help_num_geq(t->right, x, count);
        }
        //count all right nodes
        //and send it to left
        if(t->val > x){
            count += t->numRight + 1;
            return help_num_geq(t->left, x, count);
        }

        return help_num_geq(t->right, x, count);
    }

    //greater or equal >=
    int num_geq(const T & x) {
        // call helper recursive function
        int count = help_num_geq(root, x);
        return count;
    }

    /*
     * function:     num_geq_SLOW
     * description:  same functionality as num_geq but sloooow (linear time)
     *
     */
    int num_geq_SLOW(const T & x) {
        return _num_geq_SLOW(root, x);
    }
    

    /* TODO
     * Function:  num_leq
     * Description:  returns the number of elements in tree which are less
     *      than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    static void help_num_leq(bst_node *t, const T &x, int &count) {

        if(t == nullptr)
            return;
        if(t->val == x){
            count++;
            return help_num_leq(t->left, x, count);
        }
        if(t->val < x){
            count += t->numLeft + 1;
            return help_num_leq(t->right, x, count);
        }

        return help_num_leq(t->left, x, count);

    }

    int num_leq(const T &x) {
        int count = 0;
        if(x < this->min_node)
            return 0;
        if(x >= this->max_node)
            return this->num_node;

        help_num_leq(root, x, count);

        return count;
    }

    /*
     * function:     num_leq_SLOW
     * description:  same functionality as num_leq but sloooow (linear time)
     *
     */
    int num_leq_SLOW(const T & x) {
        return _num_leq_SLOW(root, x);
    }

    /* TODO
     * Function:  num_range
     * Description:  returns the number of elements in tree which are
     *       between min and max (inclusive).
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/

    
    int num_range(const T & min, const T & max) {

        return abs(this->num_node - num_geq(min) - num_leq(max));
    }

    /*
     * function:     num_range_SLOW
     * description:  same functionality as num_range but sloooow (linear time)
     *
     */
    int num_range_SLOW(const T & min, const T & max) {
        return _num_range_SLOW(root, min, max);
    }

private:


    static void _get_ith_SLOW(bst_node *t, int i, T &x, int &sofar) {
        if(t==nullptr)
            return;
        _get_ith_SLOW(t->left, i, x, sofar);

        if(sofar==i)
            return;
        sofar++;
        if(sofar==i) {
            x = t->val;
            return;
        }
        _get_ith_SLOW(t->right, i, x, sofar);
    }

    static int _num_geq_SLOW(bst_node * t, const T & x) {
        int total;

        if(t==nullptr) return 0;
        total =_num_geq_SLOW(t->left, x) + _num_geq_SLOW(t->right, x);

        if(t->val >= x)
            total++;
        return total;
    }

    static int _num_leq_SLOW(bst_node *t, const T &x) {
        int total;

        if(t==nullptr) return 0;
        total =_num_leq_SLOW(t->left, x) + _num_leq_SLOW(t->right, x);

        if(t->val <= x)
            total++;
        return total;
    }

    static int _num_range_SLOW(bst_node *t, const T &min, const T &max) {
        int total;

        if(t==nullptr) return 0;
        total =_num_range_SLOW(t->left, min, max) +
                _num_range_SLOW(t->right, min, max);

        if(t->val >= min && t->val <= max)
            total++;
        return total;
    }





private:
    static void indent(int m){
        int i;
        for(i=0; i<m; i++)
            std::cout << "-";
    }

    static void _inorder(bst_node *r){
        if(r==nullptr) return;
        _inorder(r->left);
        std::cout << "[ " << r->val << " ]\n";
        _inorder(r->right);
    }

    static void _preorder(bst_node *r, int margin){
        if(r==nullptr) {
            indent(margin);
            std::cout << " nullptr \n";
        }
        else {
            indent(margin);
            std::cout << "[ " << r->val << " ]\n";
            _preorder(r->left, margin+3);
            _preorder(r->right, margin+3);
        }
    }

    /*
     * TODO:
     * Complete the (recursive) helper function for the post-order traversal.
     * Remember: the indentation needs to be proportional to the height of the node!
     */
    static void _postorder(bst_node *r, int margin){
        if(r==nullptr) {
            indent(margin);
            std::cout << " nullptr \n";
        }

        else{
            indent(margin);
            // first recur on left subtree
            _postorder(r->left, margin+3);
            // then recur on right subtree
            _postorder(r->right, margin+3);
            // now deal with the node
            std::cout << "[ " << r->val << " ]\n";
        }

    }

public:
    void inorder() {
        std::cout << "\n======== BEGIN INORDER ============\n";
        _inorder(root);
        std::cout << "\n========  END INORDER  ============\n";
    }


    void preorder() {

        std::cout << "\n======== BEGIN PREORDER ============\n";
        _preorder(root, 0);
        std::cout << "\n========  END PREORDER  ============\n";

    }

    // indentation is proportional to depth of node being printed
    //   depth is #hops from root.
    void postorder() {

        std::cout << "\n======== BEGIN POSTORDER ============\n";
        _postorder(root, 0);
        std::cout << "\n========  END POSTORDER  ============\n";

    }

private:
    /*
     * Recursive  helper function _from_vec, used by
     * bst_from_sorted_arr(...). The function must return a sub-tree that is
     * perfectly balanced, given a sorted array of elements a.
     */
    static bst_node * _from_vec(const std::vector<T> &a, int low, int hi){
        int m;
        bst_node *root;

        if(hi < low) return nullptr;
        m = (low+hi)/2;
        root = new bst_node(a[m]);
        root->left  = _from_vec(a, low, m-1);
        root->right = _from_vec(a, m+1, hi);
        return root;

    }

public:
    static bst * from_sorted_vec(const std::vector<T> &a, int n){

        bst * t = new bst();
        t->root = _from_vec(a, 0, n-1);
        return t;
    }

    // TODO:  num_leaves
    //   Hint:  feel free to write a helper function!!
    static int help_num_leaves(bst_node *t){
        if(t == nullptr)
            return 0;
        if(t->left == nullptr && t->right== nullptr)
            return 1;
        else
            return help_num_leaves(t->left)+  help_num_leaves(t->right);
    }
    
    int num_leaves() {
        //std::cout << "\n     WARNING:  bst::num_leaves UNIMPLEMENTED...\n";
        return help_num_leaves(root);
    }

    // TODO:  num_at_level
    // description:  returns the number of nodes at specified level
    //   in tree.
    // note:  the root is at level 0.
    // Hint:  recursive helper function?
    
    static int _num_at_level(bst_node *t, int level) {
        if(t==nullptr)
            return 0;
        if(level == 0)
            return 1;
        return _num_at_level(t->left, level-1) + _num_at_level(t->right, level-1);

    }


    int num_at_level(int level) {
        return _num_at_level(root, level);
    }


private:
    bst_node *root;
    


}; // end class bst

#endif
