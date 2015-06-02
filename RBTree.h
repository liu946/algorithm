//
//  RBTree.h
//  al2
//
//  Created by liu on 15/4/29.
//  Copyright (c) 2015年 liu. All rights reserved.
//
#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <iostream>
using namespace std;

template <class Comparable>
class RBTree {
    enum RB{RED,BLACK};
    struct RBNode{
        Comparable element;
        RBNode *left;
        RBNode *right;
        RBNode *parent;
        RB color;
        RBNode(const Comparable & theElement, RBNode *lt, RBNode *rt, RBNode *pt, RB ct) : element(theElement), left(lt), right(rt), parent(pt), color(ct) {}
    };
    RBNode * root;
    void makeEmpty(RBNode * & rt){
        if (rt != NULL) {
            makeEmpty(rt->left);
            makeEmpty(rt->right);
            delete rt;
        }
    }
    RBNode * clone(RBNode * & spt,RBNode * ppt) const {
        if (spt==NULL) {
            return NULL;
        }else{
            RBNode * thisnode =new RBNode(spt->element,NULL,NULL,ppt,spt->color);
            thisnode->left=clone(spt->left,thisnode);
            thisnode->right=clone(spt->right,thisnode);
            return thisnode;
        }
    }
    void insert(const Comparable & x, RBNode * & t, RBNode * p);
    void remove(const Comparable & x, RBNode * t);
    bool contains(const Comparable & x,RBNode * t){
        if (t==NULL) {
            return false;
        }else if(x<t->element){
            return contains(x, t->left);
        }else if (x>t->element){
            return contains(x,t->right);
        }else{
            return true;
        }
    }
    RBNode * findprenode(RBNode *);
    void rotateWithLeftChild(RBNode *  k2); //记录父节点不用引用改变值
    void rotateWithRightChild(RBNode *  k2);
    void maintainoncreat(RBNode * t ){
        if (t->parent==NULL) {
            t->color=RB::BLACK;
        }else if(t->parent->color==RED){
            if (uncle(t) && uncle(t)->color==RED) {//当父亲和叔叔都是红色时(不是null、红色)
                uncle(t)->color=t->parent->color=BLACK;//父、叔=黑色
                t->parent->parent->color=RED;
                maintainoncreat(t->parent->parent);
            }else{//当叔叔是黑色时
                if (t->parent->parent->left==t->parent) { // 父亲是爷爷的左儿子
                    if (t->parent->right==t) {
                        rotateWithRightChild(t->parent);
                        maintainoncreat(t->left);
                    }else{
                        t->parent->color=BLACK;//父亲变黑、爷爷变红
                        t->parent->parent->color=RED;
                        rotateWithLeftChild(t->parent->parent);
                        
                    }
                }else{                                      // 父亲是爷爷的右儿子
                    if (t->parent->left==t){
                        rotateWithLeftChild(t->parent);
                        maintainoncreat (t->right);
                    }else{
                        t->parent->color=BLACK;//父亲变黑、爷爷变红
                        t->parent->parent->color=RED;
                        rotateWithRightChild(t->parent->parent);
                    }
                }
            }
        }
    }
    /**
     * 调整短的节点
     * t 可能是短掉的子树，或是被删除过（未释放）的节点（为了防止取空节点的父节点的时候出现空指针错）
     * 间接调用平衡左子树或平衡右子树的方法，其判定后传入的是缺少左（右）黑节点的父节点。
     */
    void maintain(RBNode * t){
        
        if (t->parent) {
            if (t==t->parent->left) {
                maintainleft(t->parent);
            }else{
                maintainright(t->parent);
            }
        }
    }
    void maintainleft(RBNode *  t);
    void maintainright(RBNode *  t);
    static RBNode * uncle( RBNode * & t){
        if (t->parent->parent->left==t->parent) {
            return t->parent->parent->right;
        }else{
            return t->parent->parent->left;
        }
    }
    void printTree(RBNode *t, std::ostream & out) const;
    void changcolor(RBNode *a,RBNode *b){
        RB c=a->color;
        a->color=b->color;
        b->color=c;
    }
    RBNode * treesuccessor(RBNode * t){
        if (t->right!=NULL) {
            t=t->right;
            while (t->left!=NULL) {
                t=t->left;
            }
            return t;
        }else{
            RBNode * y=t->parent;
            while (y!=NULL && t==y->right) {
                t=y;
                y=y->parent;
            }
            return y;
        }
        
    }
    void delcase1left(RBNode *  t);
    void delcase2left(RBNode *  t);
    void delcase3left(RBNode *  t);
    void delcase4left(RBNode *  t);
    void delcase5left(RBNode *  t);
    void delcase1right(RBNode *  t);
    void delcase2right(RBNode *  t);
    void delcase3right(RBNode *  t);
    void delcase4right(RBNode *  t);
    void delcase5right(RBNode *  t);
public:
    RBTree(){
        root=NULL;
    }
    RBTree(const RBTree & rhs){
        root=NULL;
        (*this)=rhs;
    }
    const RBTree & operator=(const RBTree & rhs) {
        if (this != &rhs) { // 此处检查赋值变量是否相等，如果不相等清空、拷贝一颗新树，相等则
            makeEmpty();
            root = clone(rhs.root,NULL);
        }//此处使用一种技巧在特殊情况下渐少操作。
        return *this;
    }
    bool isempty() const{
        return root==NULL;
    }
    void makeEmpty(){
        makeEmpty(root);
    }
    void insert(const Comparable & x){
        insert(x,root,NULL);
    }
    void remove(const Comparable & x){
        remove(x,root);
    }
    bool contains(const Comparable & x){
        return contains(x,root);
    }
    void printTree(std::ostream & out = std::cout) const {
        if (isempty())
            out << "Empty Tree" << std::endl;
        else
            printTree(root, out);
    } // Print the tree in sorted order.
    
    RBNode* find(Comparable element)
    {
        RBNode* index = root;
        while(index != NULL)
        {
            if(element<index->element)
            {
                index  = index->left;  //比当前的小，往左
            }
            else if(element>index->element)
            {
                index = index->right;  //比当前的大，往右
            }
            else
            {
                break;
            }
        }
        return index;
    }
    
};
template <class Comparable>
void RBTree<Comparable>::insert(const Comparable & x, RBNode * & t, RBNode *  p) {
    if (NULL==t){
        t = new RBTree<Comparable>::RBNode(x,NULL,NULL,p,RED);
        maintainoncreat(t);
    }else if(x < t->element){
        insert(x,t->left,t);
    }else{   //(t->element <= x)
        insert(x,t->right,t);
    }
    
}

template <class Comparable>
void RBTree<Comparable>::maintainleft(RBNode *  t) {
    if (t==NULL) {
        return;
    }
    if(t->right->right && t->right->right->color==RED){
        delcase5left(t);
        
    }else if (t->right->left && t->right->left->color==RED) {
        delcase4left(t);
    }else{
        // 黑侄
        if (t->color==RED) {
            // 红父
            delcase1left(t);
        }else{
            if (t->right->color==RED) {
                delcase2left(t);
            }else{
                delcase3left(t);
            }
        }
    }
}
template <class Comparable>
void RBTree<Comparable>::delcase1left(RBNode *  t) {
    t->color=BLACK;
    t->right->color=RED;
}
template <class Comparable>
void RBTree<Comparable>::delcase2left(RBNode *  t) {
    t->color=RED;
    t->right->color=BLACK;
    rotateWithRightChild(t);
    maintainleft(t);
}
template <class Comparable>
void RBTree<Comparable>::delcase3left(RBNode *  t) {
    t->right->color =RED;
    maintain(t);
}
template <class Comparable>
void RBTree<Comparable>::delcase4left(RBNode *  t) {
    t->right->color = RED;
    t->right->left->color = BLACK;
    rotateWithLeftChild(t->right);
    delcase5left(t);
}
template <class Comparable>
void RBTree<Comparable>::delcase5left(RBNode *  t) {
    t->right->color = t->color;
    t->color=BLACK;
    t->right->right->color=BLACK;
    rotateWithRightChild(t);
}
template <class Comparable>
void RBTree<Comparable>::maintainright(RBNode *  t) {
    if (t==NULL) {
        return;
    }
    if(t->left->left && t->left->left->color==RED){
        delcase5right(t);
        
    }else if (t->left->right && t->left->right->color==RED) {
        delcase4right(t);
    }else{
        // 黑侄
        if (t->color==RED) {
            // 红父
            delcase1right(t);
        }else{
            if (t->left->color==RED) {
                delcase2right(t);
            }else{
                delcase3right(t);
            }
        }
    }
}
template <class Comparable>
void RBTree<Comparable>::delcase1right(RBNode *  t) {
    t->color=BLACK;
    t->left->color=RED;
}
template <class Comparable>
void RBTree<Comparable>::delcase2right(RBNode *  t) {
    t->color=RED;
    t->left->color=BLACK;
    rotateWithLeftChild(t);
    maintainright(t);
}
template <class Comparable>
void RBTree<Comparable>::delcase3right(RBNode *  t) {
    t->left->color =RED;
    maintain(t);
}
template <class Comparable>
void RBTree<Comparable>::delcase4right(RBNode *  t) {
    t->left->color = RED;
    t->left->right->color = BLACK;
    rotateWithRightChild(t->left);
    delcase5right(t);
}
template <class Comparable>
void RBTree<Comparable>::delcase5right(RBNode *  t) {
    t->left->color = t->color;
    t->color=BLACK;
    t->left->left->color=BLACK;
    rotateWithLeftChild(t);
}
template <class Comparable>
void RBTree<Comparable>::rotateWithLeftChild( RBNode *  t){
    RBNode * c=t->left;
    // 注意改变root
    if(t->parent==NULL)root=t->left;
    
    if (t->parent!=NULL) {
        if (t->parent->left==t) {
            t->parent->left=c;
        }else{
            t->parent->right=c;
        }
    }
    c->parent=t->parent;
    if (c->right!=NULL) {
        c->right->parent=t;
    }
    t->left=c->right;
    c->right=t;
    t->parent=c;
    
}
template <class Comparable>
void RBTree<Comparable>::rotateWithRightChild( RBTree<Comparable>::RBNode *  t){
    RBNode * c=t->right;
    // 注意改变root
    if(t->parent==NULL)root=t->right;
    
    if (t->parent!=NULL) {
        if (t->parent->left==t) {
            t->parent->left=c;
        }else{
            t->parent->right=c;
        }
    }
    c->parent=t->parent;
    if (c->left!=NULL) {
        c->left->parent=t;
    }
    t->right=c->left;
    c->left=t;
    t->parent=c;
}
/**
 * Internal method to print a subtree rooted at t in sorted order.
 */
template<typename Comparable>
void RBTree<Comparable>::printTree(RBNode *t, std::ostream & out) const {
    if (t != NULL) {
        printTree(t->left, out);
        out << t->element << ' ';
        printTree(t->right, out);
    }
}

template<typename Comparable>
typename RBTree<Comparable>::RBNode * RBTree<Comparable>::findprenode(RBNode * t){
    RBNode * p=t->left;
    while (p->right!=NULL) {
        p=p->right;
    }
    return p;
}


template <class Comparable>
void RBTree<Comparable>::remove(const Comparable & x,RBTree<Comparable>::RBNode * t) {
    if (!t) {
        return;
    }
    if (x < t->element) {
        remove(x, t->left);
    }else if(t->element < x){
        remove(x, t->right);
    }else{
        if (t->left && t->right) { // 是否有左右子树，如果有，改为删除前驱节点
            RBTree<Comparable>::RBNode * prenodeptr = findprenode(t);
            t->element=prenodeptr->element;
            remove(prenodeptr->element, prenodeptr);
        }else {  // 只有一个子树
            RBTree<Comparable>::RBNode * childnodeptr = t->left?t->left:t->right;
            if (! (t->parent)){
                root=childnodeptr;
                
                if (childnodeptr!=NULL) {
                    childnodeptr->color=BLACK;
                    childnodeptr->parent=NULL;
                }
            }else{
                if (childnodeptr!=NULL) childnodeptr->parent = t->parent;
                if (t->parent->left==t) {
                    t->parent->left=childnodeptr;
                    // 此处嵌套整理代码，左边整理
                    // 如果t是红色，跳过
                    // 如果t是黑色
                    if (t->color==BLACK) {
                        // 子节点是红色，将子节点置黑
                        if (childnodeptr && childnodeptr->color==RED) {
                            childnodeptr->color=BLACK;
                            delete t;
                            return;
                        }
                        maintainleft(t->parent);
                    }
                }else{
                    t->parent->right=childnodeptr;
                    // 此处嵌套整理代码，右边整理
                    // 如果t是红色，跳过
                    // 如果t是黑色
                    if (t->color==BLACK) {
                        // 子节点是红色，将子节点置黑
                        if (childnodeptr && childnodeptr->color==RED) {
                            childnodeptr->color=BLACK;
                            delete t;
                            return;
                        }
                        maintainright(t->parent);
                    }
                }
            }
            
            delete t;
        }
    }
}
// example:

//int main() {
//    RBTree<int> t1;
//    
//    for (int i = 0; i < 60; i++) {
//        t1.insert(rand()%60);
//    }
//    
//    for (int i = 0; i < 400; i++) {
//        int a=rand()%60;
//        if (t1.contains(a)) {
//            t1.remove(a);
//            t1.printTree();cout<<endl;
//        }
//    }
//    return 0;
//}

#endif