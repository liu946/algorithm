//
//  avltree.h
//  al2
//
//  Created by liu on 15/4/28.
//  Copyright (c) 2015年 liu. All rights reserved.
//

/***********
 *AvlTree.h*
 ***********/

#include <iostream>
using namespace std;

template<typename Comparable>
class AvlTree {
public:

    AvlTree(){
        root = NULL;
    } // Constructor
    
    AvlTree(const AvlTree & rhs) {
        root = NULL;
        *this = rhs; // 新建一颗AVL树，这里使用新内存，本句等号是本类重载的=
    } // Copy Constructor
    
    const AvlTree & operator=(const AvlTree & rhs) {
        if (this != &rhs) { // 此处检查赋值变量是否相等，如果不相等清空、拷贝一颗新树，相等则
            makeEmpty();
            root = clone(rhs.root);
        }//此处使用一种技巧在特殊情况下渐少操作。
        return *this;
    } // Overload "operator="
    
    ~AvlTree() {
        makeEmpty(); // 调用式析构，可以用在多处
    } // Destructor
    
    const Comparable & findMin() const {
        if (isEmpty()) throw string("can't find with the empty tree!") ; // 检查空树
        return findMin(root)->element; // 公有引出，私有回调模式。可以给方法添加默认字段参数
    } // Find Minimum Element
    
    const Comparable & findMax() const {
        if (isEmpty()) throw string("can't find with the empty tree!") ; // 检查空树
        return findMax(root)->element;// 公有引出，私有回调模式。
    } // Find Maximum Element
    
    bool contains(const Comparable & x) const {
        return contains(x, root);// 公有引出，私有回调模式。
    } // Test if an item is in the tree.
    
    bool isEmpty() const {
        return root == NULL;
    } // Test if the tree is empty.
    
    void makeEmpty() {
        makeEmpty(root);
    } // Make the tree empty.
    
    void insert(const Comparable & x) {
        insert(x, root);
    } // Insert an item into the tree.
    
    void remove(const Comparable & x) {
        remove(x, root);
    } // Remove an item from the tree.
    
    void printTree(std::ostream & out = std::cout) const {
        if (isEmpty())
            out << "Empty Tree" << std::endl;
        else
            printTree(root, out);
    } // Print the tree in sorted order.
    
    int treeHeight() const {
        return treeHeight(root);
    } // Compute the height of the tree.
    
private:
    struct AvlNode {
        Comparable element;
        AvlNode *left;
        AvlNode *right;
        int height;
        
        AvlNode(const Comparable & theElement, AvlNode *lt, AvlNode *rt, int h = 0) : element(theElement), left(lt), right(rt), height(h) {}
    };
    
    AvlNode *root;
    
    void insert(const Comparable & x, AvlNode * & t);
    void remove(const Comparable & x, AvlNode * & t);
    AvlNode * findMin(AvlNode *t) const;
    AvlNode * findMax(AvlNode *t) const;
    bool contains(const Comparable & x, AvlNode *t) const;
    void makeEmpty(AvlNode * & t);
    void printTree(AvlNode *t, std::ostream & out) const;
    int treeHeight(AvlNode *t) const;
    AvlNode * clone(AvlNode *t) const;
    int height(AvlNode *t) const;
    void rotateWithLeftChild(AvlNode * & k2);
    void rotateWithRightChild(AvlNode * & k2);
    void doubleWithLeftChild(AvlNode * & k3);
    void doubleWithRightChild(AvlNode * & k3);
}
;


template<typename Comparable>
typename AvlTree<Comparable>::AvlNode * AvlTree<Comparable>::findMin(
                                                                     AvlNode *t) const {
    if (t == NULL)
        return NULL;
    if (t->left == NULL)
        return t;
    return findMin(t->left);
}

template<typename Comparable>
typename AvlTree<Comparable>::AvlNode * AvlTree<Comparable>::findMax(
                                                                     AvlNode *t) const {
    if (t == NULL)
        return NULL;
    if (t->right == NULL)
        return t;
    return findMax(t->left);
}

/**
 * Internal method to test if an item is in a subtree.
 * x is item to search for.
 * t is the node that roots the subtree.
 */
template<typename Comparable>
bool AvlTree<Comparable>::contains(const Comparable & x, AvlNode *t) const {
    if (t == NULL)
        return false;
    else if (t->element > x)
        return contains(x, t->left);
    else if (x > t->element)
        return contains(x, t->right);
    else
        return true; // Match
}
/**
 * Internal method to insert into a subtree.
 * x is the item to insert.
 * t is the node that roots the subtree.
 * Set the new root of the subtree.
 */
template<typename Comparable>
void AvlTree<Comparable>::insert(const Comparable & x, AvlNode * & t) {
    if (t == NULL) //新建叶节点
        t = new AvlNode(x, NULL, NULL);
    else if (t->element > x) {
        insert(x, t->left);
        if (height(t->left) - height(t->right) == 2) {
            if (t->left->element > x) // x在左边的左边，左儿子往上转
                rotateWithLeftChild(t);
            else
                doubleWithLeftChild(t);// x在左边的右边，左儿子的右儿子往上转，再左儿子转上来
        }
    } else if (x > t->element) {
        insert(x, t->right);
        if (height(t->right) - height(t->left) == 2) {
            if (x > t->right->element) // x 在右边的右边
                rotateWithRightChild(t); // 右儿子向上转
            else
                doubleWithRightChild(t); // 右儿子的左儿子向上转，右儿子向上转
        }
    } else
        ; // Duplicate; do nothing
    t->height = std::max(height(t->left), height(t->right)) + 1; //调整高度，注意在height中检查-1
}

/**
 * Internal method to remove from a subtree.
 * x is the item to remove.
 * t is the node that roots the subtree.
 * Set the new root of the subtree.
 */
template<typename Comparable>
void AvlTree<Comparable>::remove(const Comparable & x, AvlNode * & t) {
    if (t == NULL)
        return;
    // Item not found; do nothing
    if (t->element > x) {
        remove(x, t->left);
        if (height(t->right) - height(t->left) == 2) {
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t);
            else
                doubleWithRightChild(t);
        }
    } else if (x > t->element) {
        remove(x, t->right);
        if (height(t->left) - height(t->right) == 2) {
            if (height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t);
            else
                doubleWithLeftChild(t);
        }
    } else if (t->left != NULL && t->right != NULL) { // 左右都有节点
        t->element = findMin(t->right)->element; // 删掉右子树中最大元素，并把它弄到这个该删除的节点
        remove(t->element, t->right);
        t->height = std::max(height(t->left), height(t->right)) + 1;
    } else { // 只有一边有节点
        AvlNode *oldNode = t;
        t = (t->left != NULL) ? t->left : t->right; // 接上不空的子树
        delete oldNode; // 删除本节点
    }
    
    if (t != NULL)
        t->height = std::max(height(t->left), height(t->right)) + 1; // 调整高度
}

/**
 * Internal method to make subtree empty.
 */
template<typename Comparable>
void AvlTree<Comparable>::makeEmpty(AvlNode * & t) {
    if (t != NULL) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

/**
 * Internal method to clone subtree.
 */
template<typename Comparable>
typename AvlTree<Comparable>::AvlNode * AvlTree<Comparable>::clone(
                                                                   AvlNode *t) const {
    if (t == NULL)
        return NULL;
    return new AvlNode(t->element, clone(t->left), clone(t->right));
}

/**
 * Internal method to print a subtree rooted at t in sorted order.
 */
template<typename Comparable>
void AvlTree<Comparable>::printTree(AvlNode *t, std::ostream & out) const {
    if (t != NULL) {
        printTree(t->left, out);
        out << t->element << ' ';
        printTree(t->right, out);
    }
}

/**
 * Return the height of node t or -1 if NULL.
 * 区别于treeHeight函数，那个函数是真的去递归求深度。
 */
template<typename Comparable>
int AvlTree<Comparable>::height(AvlNode *t) const {
    return t == NULL ? -1 : t->height;
}

/**
 * Rotate binary tree node with left child.
 * For AVL trees, this is a single rotation for case 1.
 * Update heights, then set new root.
 * 关键函数，旋转
 */
template<typename Comparable>
void AvlTree<Comparable>::rotateWithLeftChild(AvlNode * & k2) {
    AvlNode *k1 = k2->left;
    k2->left = k1->right; // 左子树的右子树放到左子树上
    k1->right = k2; // 自己变成左子树的右儿子
    
    // 调整高度
    k2->height = std::max(height(k2->left), height(k2->right)) + 1;
    k1->height = std::max(height(k1->left), k2->height) + 1;
    
    k2 = k1; // 左子树接到父
}

/**
 * Rotate binary tree node with right child.
 * For AVL trees, this is a single rotation for case 1.
 * Update heights, then set new root.
 */
template<typename Comparable>
void AvlTree<Comparable>::rotateWithRightChild(AvlNode * & k2) {
    AvlNode *k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;
    k2->height = std::max(height(k2->right), height(k2->left)) + 1;
    k1->height = std::max(height(k1->right), k2->height) + 1;
    k2 = k1;
}

/**
 * Double rotate binary tree node: first left child
 * with its right child; then node k3 with new left child.
 * For AVL trees, this is a double rotation for case 2.
 * Update heights, then set new root.
 */
template<typename Comparable>
void AvlTree<Comparable>::doubleWithLeftChild(AvlNode * & k3) {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

/**
 * Double rotate binary tree node: first right child
 * with its left child; then node k3 with new right child.
 * For AVL trees, this is a double rotation for case 2.
 * Update heights, then set new root.
 */
template<typename Comparable>
void AvlTree<Comparable>::doubleWithRightChild(AvlNode * & k3) {
    rotateWithLeftChild(k3->right);
    rotateWithRightChild(k3);
}

/**
 * Internal method to compute the height of a subtree rooted at t.
 */
template<typename Comparable>
int AvlTree<Comparable>::treeHeight(AvlNode *t) const {
    if (t == NULL)
        return -1;
    else
        return 1 + std::max(treeHeight(t->left), treeHeight(t->right));
}

// example:
//
//
//int main() {
//    AvlTree<int> t1;
//    for (int i = 0; i < 130; i++) {
//        t1.insert(rand()%10000);
//    }
//    
//    std::cout << "t1's height=" << t1.treeHeight() << std::endl;
//    std::cout << "t1 is { ";
//    t1.printTree();
//    std::cout << "}" << std::endl;
//    
//    AvlTree<int> t2(t1);
//    int n = 0;
//    while (n < 120) {
//        int j = rand()%10000;
//        if (t2.contains(j)) {
//            t2.remove(j);
//            n++;
//        }
//    }
//    t1 = t2;
//    
//    std::cout << "t1's height=" << t1.treeHeight() << std::endl;
//    std::cout << "t1 is { ";
//    t1.printTree();
//    std::cout << "}" << std::endl;
//    return 0;
//}