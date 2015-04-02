//
//  mystl.h
//  BSTree
//
//  Created by liu on 11/25/14.
//  Copyright (c) 2014 liu. All rights reserved.
//

//
//  mystl.h
//  simplestl
//
//  Created by liu on 14-10-22.
//  Copyright (c) 2014年 liu. All rights reserved.
//

#ifndef __simplestl__mystl__
#define __simplestl__mystl__
#include <fstream>
#include <iostream>
using namespace::std;
template <typename T>
class Node {
public:
    Node * next;
    T data;
    Node(T _dat,Node * p = NULL):data(_dat){
        next = p;
        
    }
    Node(){}
};
//栈类
template <typename T> class Stack {
public:
    Node<T>* head;
    //初始化一个栈
    Stack(){
        this->head=NULL;
    }
    //析构自动释放内存
    ~Stack(){
        Node<T>* p;
        while (this->head != NULL) {
            p=head;
            head=head->next;
            delete p;
        }
    }
    //判空
    bool empty(){
        return head == NULL;
    }
    //弹栈
    T pop(){
        if (empty()) {
            string err("EMPTY STACK!");
            throw err;
        }
        Node<T>* p = head;
        head = head->next;
        T _data = p->data;
        delete p;
        return _data;
    }
    void popall(){
        while (!empty()) {
            pop();
        }
    }
    //栈顶
    T top(){
        if (empty()) {
            string err("EMPTY STACK!");
            throw err;
        }
        return head->data;
    }
    //入栈
    Stack<T>* push(T data){
        Node<T>* p = new Node<T>(data,head);
        if (p==NULL) {
            string err("OUT OF MEMERAY!");
            throw err;
        }
        head = p;
        return this;
    }
    //非栈功能函数，调试使用
    void show(){
        cout<<"\n##########\n";
        Node<T>* p=head;
        int i=1;
        while (p!=NULL) {
            cout<<i<<"|  "<<p->data<<endl;
        }
    }
};
//队列
template <typename T>class Queue {
protected:
    Node<T> * frontptr;
    Node<T> * rearptr;
    
public:
    //初始化队列
    Queue(){
        frontptr=NULL;
        rearptr=NULL;
    }
    //析构
    ~Queue(){
        Node<T> *p;
        while (frontptr!=rearptr) {
            p=frontptr;
            frontptr=frontptr->next;
            delete p;
        }
        delete frontptr;
    }
    //判空
    bool empty(){
        return frontptr==NULL;
    }
    //将出队列元素
    T front(){
        if (empty()) {
            string err("EMPTY QUEUE!");
            throw err;
        }
        return frontptr->data;
    }
    //入队
    Queue * enqueue(T data){
        if (empty()) {
            rearptr=frontptr=new Node<T>(data);
            return this;
        }
        rearptr->next=new Node<T>(data);
        rearptr=rearptr->next;
        return this;
    }
    //出队
    T dequeue(){
        if (empty()) {
            string err("EMPTY QUEUE!");
            throw err;
        }
        T data=frontptr->data;
        if (frontptr==rearptr) {
            frontptr=rearptr=NULL;
        }else{
            frontptr=frontptr->next;
        }
        return data;
    }
    //显示队列内容
    void showqueue(){
        if (empty()) {
            string err("EMPTY QUEUE!");
            throw err;
        }
        cout<<"\n##########\nQUEUE FRONT\n";
        int i(0);
        Node<T>* p=frontptr;
        for (; p!=rearptr; i++,p=p->next) {
            cout<<i<<" : "<<p->data<<endl;
        }
        cout<<i<<" : "<<p->data<<endl;
    }
};
//树节点模板类
template <typename T>
class TreeNode {
public:
    TreeNode * left;
    TreeNode * right;
    T data;
    TreeNode(T _dat,TreeNode* _lc=NULL,TreeNode * _rc=NULL){
        data=_dat;
        left=_lc;
        right=_rc;
    }
};
//树使用的常量
namespace tree {
#define PROORDER 0
#define INORDER 1
#define POSTORDER 2
    
}
template <class T>
class Forest;
//树模板类
template <typename T,typename Tnode=TreeNode<T>>
class Tree {
protected:
    TreeNode<T>* root;
    //非递归遍历节点
    void showtreewithtablenode(function<void (T)>show,TreeNode<T>* localroot){
        if (localroot==NULL) {
            return;
        }else{
            show(localroot->data);
            if (localroot->left==NULL&&localroot->right==NULL) {
                return;
            }
            cout<<'(';
            if (localroot->left) {
                showtreewithtablenode(show, localroot->left);
            }
            
            cout<<',';
            if (localroot->right) {
                showtreewithtablenode(show, localroot->right);
                
            }
            cout<<')';
        }
    }
    //删除一个树
    void deleteTree(TreeNode<T>* _localroot){
        if (_localroot==NULL) {
            return;
        }else{
            deleteTree(_localroot->left);
            deleteTree(_localroot->right);
            delete _localroot;
        }
    }
    //使用字符串生成一个字符树
    TreeNode<T> * creatTreeWithString(T * &str,T nullmark){
        T _ch=str[0];
        str++;
        if (_ch==nullmark) {
            return NULL;
        }else{
            TreeNode<T> *  _tree = new Tnode(_ch);
            _tree->left=creatTreeWithString(str,nullmark);
            _tree->right=creatTreeWithString(str,nullmark);
            return _tree;
        }
    }
    //模板函数，结合三种遍历方式
    template<char ORDER=PROORDER>
    void traversalnode(void(func)(TreeNode<T>* thisnode),TreeNode<T>* localroot){
        if (localroot==NULL) {
            return;
        }else{
            if(ORDER==PROORDER)func(localroot);
            traversalnode(func, localroot->left);
            if(ORDER==INORDER)func(localroot);
            traversalnode(func, localroot->right);
            if (ORDER==POSTORDER)func(localroot);
        }
    }
    
    //模板函数，结合三种遍历方式，其中的回调函数能够获得路径
    template<char ORDER=PROORDER>
    void traversalnode(void(func)(TreeNode<T>* thisnode,string,void *),TreeNode<T>* localroot,string path,void * ptr=NULL){
        if (localroot==NULL) {
            return;
        }else{
            if(ORDER==PROORDER)func(localroot,path,ptr);
            traversalnode<ORDER>(func, localroot->left,path+"L",ptr);
            if(ORDER==INORDER)func(localroot,path,ptr);
            traversalnode<ORDER>(func, localroot->right,path+"R",ptr);
            if (ORDER==POSTORDER)func(localroot,path,ptr);
        }
    }
    // 支持lambda表达式和operater()的闭包调用方式。
    template<char ORDER=PROORDER>
    void traversalnode(function<void(TreeNode<T>*,string)>func,TreeNode<T>* localroot,string path){
        if (localroot==NULL) {
            return;
        }else{
            if(ORDER==PROORDER)func(localroot,path);
            traversalnode<ORDER>(func, localroot->left,path+"L");
            if(ORDER==INORDER)func(localroot,path);
            traversalnode<ORDER>(func, localroot->right,path+"R");
            if (ORDER==POSTORDER)func(localroot,path);
        }
    }
    
    //子树的深度，递归使用
    int subtreedeep(TreeNode<T>* localroot ,int deep=0){
        if (localroot==NULL) {
            return deep;
        }else{
            int ld=subtreedeep(localroot->left,deep+1);
            int lr=subtreedeep(localroot->right,deep+1);
            return ld>lr ? ld:lr ;
        }
    }
    //前中序遍历构造树
    TreeNode<T>* creatTreeWithFRLandMRL(T * frl,T* mrl,int len){
        if (len<=0) {
            return NULL;
        }
        TreeNode<T> * _tree = new Tnode(*frl);
        int index = nodestringfind(mrl, *frl, len);
        _tree->left = creatTreeWithFRLandMRL(frl+1, mrl, index);
        _tree->right = creatTreeWithFRLandMRL(frl+index+1, mrl+index+1, len-index-1);
        return _tree;
    }
    //查找树节点
    int nodestringfind(T * str,T d,int maxlen){
        for (int i=0; i<maxlen; i++) {
            if (d==str[i]) {
                return i;
            }
        }
        return -1;
    }
    int deepnode(TreeNode<T>* localroot){
        if (localroot==NULL) {
            return 0;
        }else{
            int l=deepnode(localroot->left);
            int r=deepnode(localroot->right);
            return 1+(l>r?l:r);
        }
    }
public:
    //构造
    Tree(TreeNode<T>* _root =NULL){
        root = _root;
    }
    Tree(Forest<T> & fst){
        root=new TreeNode<T>( fst.treelist[0]->getroot()->data);
        
        conformforest(fst.treelist[0]->getroot(),root);
        TreeNode<T>* p=root;
        for(int i=1;i<fst.num;i++,p=p->right){
            conformforest(fst.treelist[i]->getroot(),p->right);
        }
    }
    void conformforest(TreeNode<T> * x,TreeNode<T>* & nx){
        nx=new TreeNode<T>(x->data);
        if (x->left) {
            conformforest(x->left, nx->left);
            if (x->right) {
                conformforest(x->right, nx->left->right);
            }
        }

    }
    bool empty(){
        return root==NULL;
    }
    //析构
    Tree * refreshTreeFormstring(char * str,T nullmark){
        int cont=0;
        for (int i=0; str[i]!='\0'; i++) {
            if (str[i]==nullmark) {
                cont++;
            }else{
                cont--;
            }
        }
        if (cont!=1) {
            string err("cant match");
            throw err;
        }
        deleteTree();                 //clear old tree
        root = creatTreeWithString(str, nullmark);
        return this;
    }
    //前中序构造树
    Tree * refreshTreeFormFRLandMRL(T * frl,T* mrl,int len){
        deleteTree();
        root = creatTreeWithFRLandMRL(frl, mrl,len);
        return this;
    }
    //遍历树的多种方式
    template<char ORDER=PROORDER>
    void traversal(void(func)(TreeNode<T>* thisnode)){
        traversalnode<ORDER>(func, root);
    }
    template<char ORDER=PROORDER>
    void traversal(void(func)(TreeNode<T>* thisnode,string ,void *),void * ptr=NULL){
        traversalnode<ORDER>(func, root,"",ptr);
    }
    // 支持lambda表达式和operater()的闭包调用方式。
    template<char ORDER=PROORDER>
    void traversal(function<void(TreeNode<T>*,string)>foo=[=](TreeNode<T>* node,string){cout<<" "<<node->data;}){
        traversalnode<ORDER>(foo, root,"");
    }
    void traversalpost(function<void(TreeNode<T>*,string)>foo=[=](TreeNode<T>* node,string){cout<<" "<<node->data;}){
        traversal<POSTORDER>(foo);
    }
    //树长度
    unsigned long static nodedeepformpath(string path){
        return path.length();
    }
    //层序
    void levelorder(){
        Queue<TreeNode<T> *> tq;
        if (root==NULL) {
            return;
        }
        tq.enqueue(root);
        while (!tq.empty()) {
            TreeNode<T>* p= tq.dequeue();
            cout<<p->data;
            if (p->left!=NULL) {
                tq.enqueue(p->left);
            }
            if (p->right!=NULL) {
                tq.enqueue(p->right);
            }
        }
    }
    //找到node位置
    string  getnodepath(T nodedata){
        string  _path;
        auto getnodepathcallbackfunc = [&](TreeNode<T> * thisnode,string path) {
            if (nodedata==thisnode->data) {
                _path=path;
            }
        };
        traversal(getnodepathcallbackfunc);
        return _path;
    }
    //路径返回节点
    T getnodebypath(string path){
        TreeNode<T>*ptr=root;
        while (path!="") {
            if (ptr==NULL) {
                string err("🌲中无此节点！");
                throw err;
            }
            switch (path[0]) {
                case 'L':
                    ptr=ptr->left;
                    break;
                case 'R':
                    ptr=ptr->right;
                    break;
                default:
                    string err("路径不合法！");
                    throw err;
                    break;
            }
            path.erase(0,1);
        }
        return ptr->data;
    }
    
    int treedeep(int deep=0){
        return subtreedeep(root);
    }
    //非递归前序中序建立树
    Tree * refreshTreeFormFRLandMRLnorecursion(T * frl,T * mrl,int len){
        deleteTree();
        Stack<TreeNode<T>*> stk;
        for (int i=0; i<len; i++) {
            if (stk.empty()) {
                TreeNode<T> * newnodeptr = new Tnode(frl[i]);
                if (root==NULL) {
                    root=newnodeptr;
                }
                stk.push(newnodeptr);
                continue;
            }else{
                
                int sindex=nodestringfind(mrl, stk.top()->data, len);
                int cindex=nodestringfind(mrl, frl[i], len);
                if (sindex==-1||cindex==-1) {
                    string err("match failed!");
                    throw err;
                }
                if(sindex>cindex){
                    stk.top()->left=new Tnode(frl[i]);
                    stk.push(stk.top()->left);
                }else{
                    TreeNode<T>* faptr=stk.top();
                    while (sindex<cindex) {
                        faptr=stk.pop();
                        sindex=stk.empty() ? len : nodestringfind(mrl, stk.top()->data, len);//bugpoint 1 26.1.03 过根break;
                        cindex=nodestringfind(mrl, frl[i], len);
                    }
                    faptr->right=new Tnode(frl[i]);
                    stk.push(faptr->right);
                }
            }
        }
        return this;
    }

    //广义表输出
    void showtreewithtable(function<void (T)>show=[=](T t){cout<<t;} ){
        
        cout<<'(';
        showtreewithtablenode(show, root);
        cout<<')';
    }
    //非递归前序
    void showtreeprenorecursion(){
        TreeNode<T>* p=root;
        while (p!=NULL) {
            
            if (p->left==NULL) {
                cout<<p->data;
                p=p->right;
            }else{
                TreeNode<T> * pp=p->left;
                if (pp->right==NULL) {
                    cout<<p->data;
                    pp->right=p;
                    p=p->left;
                    continue;
                }
                while (pp->right!=NULL && pp->right!=p) {
                    pp=pp->right;
                }
                if (pp->right==NULL) {
                    cout<<p->data;
                    pp->right=p;
                    p=p->left;
                }else{
                    pp->right=NULL;
                    //cout<<p->data;
                    p=p->right;
                }
            }
        }
    }
    //非递归中序
    void showtreeinnorecursion(){
        TreeNode<T>* p=root;
        while (p!=NULL) {
            
            if (p->left==NULL) {
                cout<<p->data;
                p=p->right;
            }else{
                TreeNode<T> * pp=p->left;
                if (pp->right==NULL) {
                    pp->right=p;
                    p=p->left;
                    continue;
                }
                while (pp->right!=NULL && pp->right!=p) {
                    pp=pp->right;
                }
                if (pp->right==NULL) {
                    pp->right=p;
                    p=p->left;
                }else{
                    pp->right=NULL;
                    cout<<p->data;
                    p=p->right;
                }
            }
        }
    }
    //非递归后序
    void showtreelastnorecursion(){
        T * td=new T;
        TreeNode<T>* newroot=new Tnode(*td);
        newroot->left=root;
        TreeNode<T>* p=newroot;
        while (p!=NULL) {
            
            if (p->left==NULL) {
                p=p->right;
            }else{
                TreeNode<T> * pp=p->left;
                if (pp->right==NULL) {
                    pp->right=p;
                    p=p->left;
                    continue;
                }
                while (pp->right!=NULL && pp->right!=p) {
                    pp=pp->right;
                }
                if (pp->right==NULL) {
                    pp->right=p;
                    p=p->left;
                }else{
                    T tempdata=p->data;
                    do{
                        pp=p->left;
                        while (pp->right->data!=tempdata) {
                            pp=pp->right;
                        }
                        if (pp->right->data==p->data) {
                            pp->right=NULL;
                        }
                        cout<<pp->data;
                        tempdata=pp->data;
                    }while(pp!=p->left);
                    p=p->right;
                }
            }
        }
        delete newroot;
        delete td;
    }
    int deep(){
       return  deepnode(this->root);
    }
    TreeNode<T>* & getroot(){
        return root;
    }
    ~Tree(){
        deleteTree();
    }
    void deleteTree(){
        deleteTree(root);
    }
    
    
    
};
//森林

template <class T>
class Forest {

    
public:
    Tree<T> ** treelist=NULL;
    unsigned int num=0;
    //递归的生产一个树，切换儿子兄弟树
    void conforest(TreeNode<T> * x,TreeNode<T>* & nx){
        if (x==NULL) {
            return;
        }
        nx=new TreeNode<T>(x->data);
        if (x->left) {
            conforest(x->left, nx->left);
            if (x->left->right) {
                conforest(x->left->right,nx->right);
            }
        }
    }
    Forest(Tree<T> & ForestTree){
        num=0;TreeNode<T>*p=ForestTree.getroot();while(p!=NULL){p=p->right;num++;}
        treelist=new Tree<T>*[num];
        p=ForestTree.getroot();
        for (int i=0; i<num; i++,p=p->right) {
            treelist[i]=new Tree<T>;
            conforest(p,treelist[i]->getroot());
        }
    }
    void printforest(){
        for (int i=0; i<num; i++) {
            treelist[i]->showtreewithtable();
            cout<<endl;
        }
    }
    ~Forest(){
        if (treelist==NULL) {
            return;
        }
        for (int i=0; i<num; i++) {
            delete treelist[i];
        }
        delete[] treelist;
    }
};

template <class T>
class ClueTreeNode :public TreeNode<T> {
    
    
public:
    ClueTreeNode<T> * clue=NULL;
    ClueTreeNode<T>(T _dat,ClueTreeNode* _lc=NULL,ClueTreeNode * _rc=NULL,ClueTreeNode * _clue=NULL):TreeNode<T>(_dat,_lc,_rc),clue(_clue){}
};

template <class T>
class ClueTree :public Tree<T,ClueTreeNode<T>>{
protected:
    ClueTreeNode<T> * cluehead=NULL;
public:
    template<char ORDER=PROORDER>
    void cluetree(){
        
        ClueTreeNode<T> * pnode=NULL;
        auto foo=[&](TreeNode<T>* node,string){
            if (pnode==NULL) {
                cluehead = pnode=(ClueTreeNode<T> *)node;
            }else{
                pnode->clue=(ClueTreeNode<T> *)node;
                pnode=(ClueTreeNode<T> *)node;
            }
        };
        if (PROORDER==ORDER) {
            this->traversal(foo);
        }else if(POSTORDER==ORDER){
            this->traversalpost(foo);
        }
    }
    void travelwithclue(){
        ClueTreeNode<T>* pnode=cluehead;
        while (pnode!=NULL) {
            cout<<pnode->data;
            cout<<" ";
            pnode=pnode->clue;
        }
    }
    // 特化方法，利用前序线索中序遍历
    void medtravelwithpreclue(){
        cluetree();//建立前序线索
        TreeNode<T> * p=this->root;
        Stack<TreeNode<T>*> stk;
        while (true){
            while (p->left!=NULL) {
                stk.push(p);//前序压栈
                p=p->left;
            }
            stk.push(p);
            p=p->right==NULL?((ClueTreeNode<T>*)p)->clue:p->right;
            if (p!=NULL) {
                TreeNode<T>* px;
                do{
                    px=stk.pop();
                    if (px==NULL)break;
                    cout<<px->data<<' ';
                }while (px->right!=p);
            }else{
                break;
            }
        }
        while (!stk.empty()) {
            p=stk.pop();
            cout<<p->data<<' ';
        }
        
        
    }
};





//链表类，提供了基本的添加删除求长操作
template <typename T>
class List {
protected:
    Node<T> * head=NULL;
    
public:
    List(){head=NULL;}
    List(const List & l){
        //拷贝构造函数
        //析构当前量
        Node<T>* ptr,*ptr2;
        while (head!=NULL) {
            ptr=head;
            head=head->next;
            delete ptr;
        }
        //
        ptr=l.head;
        if (ptr!=NULL) {
            head=new Node<T>(ptr->data);
            ptr2=head;
            ptr=ptr->next;
        }
        while (ptr!=NULL) {
            ptr2->next=new Node<T>(ptr->data);
            ptr=ptr->next;
            ptr2=ptr2->next;
        }
        
    }
    ~List(){
        Node<T>* ptr;
        while (head!=NULL) {
            ptr=head;
            head=head->next;
            delete ptr;
        }
    }
    List & operator=(List & l){
        Node<T>* ptr,*ptr2;
        while (head!=NULL) {
            ptr=head;
            head=head->next;
            delete ptr;
        }
        //
        ptr=l.head;
        if (ptr!=NULL) {
            head=new Node<T>(ptr->data);
            ptr2=head;
            ptr=ptr->next;
        }
        while (ptr!=NULL) {
            ptr2->next=new Node<T>(ptr->data);
            ptr=ptr->next;
            ptr2=ptr2->next;
        }
        return *this;
    }
    bool isempty(){
        return head==NULL;
    }
    
    void deletenode(int index){
        Node<T>* p=head;
        if (index ==0) {
            head=p->next;
            delete p;
            return;
        }
        for (int i=0;i<index-1;i++,p=p->next);
        Node<T>* ptr=p->next;
        p->next=p->next->next;
        delete ptr;
        
    }
    
    int deep(){
        int i=0;
        Node<T>* p=head;
        while (p!=NULL) {
            i++;
            p=p->next;
        }
        return i;
    }
    //去第一个
    T getfirst(){
        if (isempty()) {
            string err("List empty");
            throw err;
        }
        return head->data;
    }
    void append(T newnodedata){
        Node<T> *p=head;
        if(p==NULL){
            head=new Node<T>(newnodedata) ;
            return;
        }
        while (p->next!=NULL) {
            p=p->next;
        }
        p->next=new Node<T>(newnodedata);
        
    }
    List operator+(List<T> L){
        List<T> ans;
        Node<T> *p,*px;
        if (this->head==NULL) {
            return L;
        }
        ans.head=new Node<T>(this->head->data);
        px=ans.head;
        for(p=head->next;p;p=p->next,px=px->next) {
            px->next=new Node<T>(p->data);
        }
        for(p=L.head;p;p=p->next,px=px->next) {
            px->next=new Node<T>(p->data);
        }
        return ans;
    }
    //遍历
    void traversal(function<void (T)> foo){
        Node<T>* ptr=head;
        while (ptr!=NULL) {
            foo(ptr->data);
            if (ptr->next==NULL) {
                return;
            }
            ptr=ptr->next;
        }
    }
    //求长
    int length(){
        return this->deep();
    }
    //寻找
    int find(T data){
        int len=length();
        Node<T> * ptr=this->head;
        for (int i=0; i<len; i++) {
            if (ptr->data==data) {
                return i;
            }
            ptr=ptr->next;
        }
        return -1;
    }

    //    void showlist(){
    //        Node<T>* ptr=head;
    //        while (ptr!=NULL) {
    //            shownode(ptr->data);
    //            ptr=ptr->next;
    //        }
    //    }
    //    virtual void shownode(T x){
    //        cout<<x;
    //    }
    
};
//有序链表，插入删除时能够自动排序，需要完成虚函数cmp
template <typename T>
class OrderList:public List<T> {
public:
    void insert(T newnodedata){
        Node<T>*ptr;
        ptr=this->head;
        if (ptr==NULL) {
            Node<T> * p=new Node<T>;
            p->data=newnodedata;
            p->next=NULL;
            this->head=p;
            return;
        }
        if (cmp(newnodedata,ptr->data)) {
            Node<T> * p =new Node<T>;
            p->data = newnodedata;
            p->next=this->head;
            this->head=p;
            return;
        }
        while (ptr->next!=NULL ) {
            if (cmp(ptr->data,newnodedata) &&cmp(newnodedata,ptr->next->data)) {
                Node<T> * p=new Node<T>;
                p->data=newnodedata;
                p->next=ptr->next;
                ptr->next=p;
                return;
            }
            ptr=ptr->next;
        }
        Node<T> * p=new Node<T>;
        p->data=newnodedata;
        p->next=NULL;
        ptr->next=p;
    }
    
    virtual bool cmp(T n1,T n2)=0;
};

//向量类
template <typename T>
class Factor:public List<T> {
    
public:
    //运算符重载【】
    T & operator[](int index){
        int i=0;
        Node<T> * ptr=this->head;
        if (this->isempty()) {
            string err("can't index.");
            throw err;
        }
        while(i++<index){
            ptr=ptr->next;
            if (ptr==NULL) {
                string err("can't index.");
                throw err;
            }
        }
        return ptr->data;
    }
    Factor & operator=(const Factor & l){
        Node<T>* ptr,ptr2;
        while (this->head!=NULL) {
            ptr=this->head;
            this->head=this->head->next;
            delete ptr;
        }
        //
        ptr=l.head;
        if (ptr!=NULL) {
            this->head=new Node<T>(ptr->data);
            ptr2=this->head;
            ptr=ptr->next;
        }
        while (ptr!=NULL) {
            ptr2->next=new Node<T>(ptr->data);
            ptr=ptr->next;
            ptr2=ptr2->next;
        }
        return *this;
    }
    
};
template <typename T>
class ArrExpand {
    
public:
    static bool Halffind(T * arr,int len,T x){
        if (len==0) {
            return false;
        }
        if(arr[len/2]==x){
            return true;
        }else if(arr[len/2]>x){
            return  Halffind(arr, len/2, x);
        }else{
            return  Halffind(arr+len/2+1, len/2-1+len%2, x);
        }
    }
    static void quicksort( T * arr,int len){
        int i=0,j=len-1;
        T val=arr[0];/*指定参考值val大小*/
        if(len>1)/*确保数组长度至少为2，否则无需排序*/
        {
            while(i<j)/*循环结束条件*/
            {
                /*从后向前搜索比val小的元素，找到后填到arr[i]中并跳出循环*/
                for(;j>i;j--)
                if(arr[j]<val)
                {
                    arr[i++]=arr[j];
                    break;
                }
                /*从前往后搜索比val大的元素，找到后填到arr[j]中并跳出循环*/
                for(;i<j;i++)
                if(arr[i]>val)
                {
                    arr[j--]=arr[i];
                    break;
                }
            }
            arr[i]=val;/*将保存在val中的数放到a[i]中*/
            quicksort(arr,i);/*递归，对前i个数排序*/
            quicksort(arr+i+1,len-i-1);/*对i+2到len这len-1-i个数排序*/
        }
        
    }
    static void insertsort(T *array,int n)
    {
        int i,j;
        T temp;
        for(i=1;i<n;i++)
        {
            temp=*(array+i);
            for(j=i ; j>0 && *(array+j-1)>temp; j--)
            {
                *(array+j)=*(array+j-1);
            }
            *(array+j)=temp;
        }
    }
    static void selectsort(T * arr,int n)
    {
        int i,j,min,t;
        for(i=0;i<n-1;i++)
        {
            min=i;//查找最小值
            for(j=i+1;j<n;j++)
                if(arr[min]>arr[j])
                    min=j;//交换
            if(min!=i)
            {
                t=arr[min];
                arr[min]=arr[i];
                arr[i]=t;
            }
        }
    }
    static void bubblesort(int *a,int n)//n为数组a的元素个数
    {
        int i,j;
        T temp;
        for(j=0;j<n-1;j++)
            for(i=0;i<n-1-j;i++)
            {
                if(a[i]>a[i+1])//数组元素大小按升序排列
                {
                    temp=a[i];
                    a[i]=a[i+1];
                    a[i+1]=temp;
                }
            }
    }
    
};

//带数据段的行，用于邻接表表的一行
template <typename T>
class GraphRow:public Factor<int> {
    
public:
    T rowname;
    bool operator==( T x2){
        return rowname==x2;
    }
    GraphRow(T x){
        rowname=x;
        this->head=NULL;
    }
};
//定义表数据结构，增加一些方法
template <typename T>
class GraphRowsFactor:public Factor<GraphRow<T>> {
public:
    //找节点
    int find(T data){
        int len=this->length();
        Node<GraphRow<T>> * ptr=this->head;
        for (int i=0; i<len; i++) {
            if (ptr->data==data) {
                return i;
            }
            ptr=ptr->next;
        }
        return -1;
    }
    //连接新节点
    void append(T newnodedata){
        Node<GraphRow<T>> *p=this->head;
        if(p==NULL){
            this->head=new Node<GraphRow<T>>(newnodedata) ;
            return;
        }
        while (p->next!=NULL) {
            p=p->next;
        }
        p->next=new Node<GraphRow<T>>(newnodedata);
        
    }
    
};
//含有邻接表的有向图
template <typename T>
class DircGraph {
protected:
    GraphRowsFactor<T> rows;
    
private:
    //遍历节点
    void traversalnode(function<void (T)>show,int index,bool * visited){
        if (visited[index])return;
        show(rows[index].rowname);
        visited[index]=true;
        for (int i=0; i<rows[index].length(); i++) {
            traversalnode(show, rows[index][i], visited);
        }
        
    }
    //遍历邻接表行
    void traversalnoderows(function<void (GraphRow<T> &)>excuterows,int index,bool * visited){
        if (visited[index])return;
        excuterows(rows[index]);
        visited[index]=true;
        for (int i=0; i<rows[index].length(); i++) {
            traversalnoderows(excuterows, rows[index][i], visited);
        }
        
    }
    
public:
    GraphRow<T> & operator[](int i){
        return rows[i];
    }
    int length(){
        return rows.length();
    }
    //加边或点
    void addarrow(T a,T b){
        int index1=rows.find(a);
        if (index1==-1) {
            index1=rows.length();
            rows.append(a);
            
        }
        int index2=rows.find(b);
        if (index2==-1) {
            index2=rows.length();
            rows.append(b);
            
        }
        if (index1==index2||directto(a, b)) {
            return;
        }
        rows[index1].append(index2);
    }
    //判断边存在
    bool directto(T a,T b){
        int index1=rows.find(a);
        int index2=rows.find(b);
        if (index1==-1||index2==-1) {
            string err("no such node");
            throw err;
        }
        return  rows[index1].find(index2)!=-1;
    }
    //判断边存在
    bool hastheline(T a,T b){
        int index1=rows.find(a);
        int index2=rows.find(b);
        if (index1==-1||index2==-1) {
            return false;
        }
        return  rows[index1].find(index2)!=-1;
    }
    //递归遍历
    void traversal(function<void (T)>show=[=](T t){cout<<" "<<t;} ){
        bool * visited=new bool[rows.length()];
        for (int i=0; i<rows.length(); i++) {
            visited[i]=false;
        }
        for (int i=0; i<rows.length(); i++) {
            if (!visited[i]) {
                traversalnode(show,i,visited);
            }
        }
        delete [] visited;
    }
    //遍历行
    void traversalrows(function<void (GraphRow<T>&)>excutenode ){
        bool * visited=new bool[rows.length()];
        for (int i=0; i<rows.length(); i++) {
            visited[i]=false;
        }
        for (int i=0; i<rows.length(); i++) {
            if (!visited[i]) {
                traversalnoderows(excutenode,i,visited);
            }
        }
        delete [] visited;
    }
    //非递归遍历
    void traversalnorecursion(function<void (T)>show=[=](T t){cout<<" "<<t;} ){
        
        bool * visited=new bool[rows.length()];
        for (int i=0; i<rows.length(); i++) {
            visited[i]=false;
        }
        Stack<int> stk;
        
        for (int i=0; i<rows.length(); i++) {
            if (!visited[i]) {
                stk.push(i);
                while (!stk.empty()) {
                    int index=stk.pop();
                    if (visited[index]) break;
                    show(rows[index].rowname);
                    visited[index]=true;
                    for (int j=0; j<rows[index].length(); j++) {
                        stk.push(rows[index][j]);
                    }
                }
            }
        }
        delete [] visited;
    }
    //层序便利
    void traversallevel(function<void (T)>show=[=](T t){cout<<" "<<t;} ){
        
        bool * visited=new bool[rows.length()];
        for (int i=0; i<rows.length(); i++) {
            visited[i]=false;
        }
        Queue<int> que;
        
        for (int i=0; i<rows.length(); i++) {
            if (!visited[i]) {
                que.enqueue(i);
                while (!que.empty()) {
                    int index=que.dequeue();
                    if (visited[index]) break;
                    show(rows[index].rowname);
                    visited[index]=true;
                    for (int j=0; j<rows[index].length(); j++) {
                        que.enqueue(rows[index][j]);
                    }
                }
            }
        }
        delete [] visited;
    }
    //输出邻接表
    void traversallist()
    {
        for (int i=0; i<rows.length(); i++) {
            cout<<endl<<rows[i].rowname<<" : ";
            for (int j=0; j<rows[i].length(); j++) {
                cout<<rows[rows[i][j]].rowname<<" ";
            }
        }
    }
    //从文件构造
    void loadformfile(string filename){
        fstream file(filename);
        char c1,c2;
        while (file>>c1 && file >>c2) {
            addarrow(c1, c2);
        }
    }
    //存入文件
    void savetofile(string filename){
        fstream file(filename,ios::out);
        
        traversalrows([&](GraphRow<T>& line){
            for (int i=0; i<line.length(); i++) {
                file<<line.rowname<<rows[line[i]].rowname;
            }
        });
    }
    //查找行
    int find(T x){
        return rows.find(x);
    }
};
//无向图
template <typename T>
class Graph:public DircGraph<T> {
    
public:
    //覆盖加边方法
    void addline(T a,T b){
        this->addarrow(a,b);
        this->addarrow(b,a);
    }
};
#endif /* defined(__simplestl__mystl__) */

//examples
//
//
//ClueTree<char> tree;
//tree.refreshTreeFormstring("abd0gj000eh000cf0i000", '0');
//tree.cluetree();
//tree.traversal();
//cout<<endl<<' ';
//tree.travelwithclue();
//
//cout<<endl<<' ';
//tree.traversal<INORDER>();
//cout<<endl<<' ';
//tree.medtravelwithpreclue();
//cout<<endl<<' ';
//
//Forest<char>fst((Tree<char> &)tree);
//fst.printforest();
//Tree<char> trfst(fst);
//trfst.showtreewithtable();
