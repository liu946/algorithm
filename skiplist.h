//
//  skiplist.h
//  algorithm
//
//  Created by liu on 15/3/10.
//  Copyright (c) 2015年 liu. All rights reserved.
//
//  EXAMPLE
//int main(int argc, const char * argv[]) {
//    // insert code here...
//    SkipList<int> a;
//    for (int i=2; i<50; i++) {
//        a.insert(i);
//    }
//    for (int i=2; i<50; i++) {
//        a.insert(i);
//    }
//    a.printlist();
//    for (int i=2; i<50; i+=2) {
//        a.deletenode(i);
//    }
//    for (int i=2; i<50; i+=2) {
//        a.deletenode(i);
//    }
//    a.printlist();
//    return 0;
//}


#ifndef algorithm_skiplist_h
#define algorithm_skiplist_h

#include <iostream>
#include "string"
#include "mystl.h"

// 为何不使用“是”，而是用“有一个”？没有顶层结果导致的后果是，必须使用递归，C++没有伪递归。

// 完成不封顶的层数
// 稳定性
//

using namespace std;
template <class T>
class SkipNode {
public:
    T data;
    int level = 0; //至少需要一层，编号为0
    SkipNode ** next = NULL;
    SkipNode(T dt,int maxlevel = 0){
        data=dt;
        while(rand()%2 && (level<maxlevel || !maxlevel) ){
            // 概率符合 && （不超过最大层数 || 最大层数设置为无限 0 ） 可以增加
            level++;
        }
        next=new SkipNode * [level+1];
        for (int i=0; i<=level; i++) {
            next[i]=NULL;
        }
    }
    ~SkipNode(){
        delete next;
    }
};

template <class T>
class SkipList {
    SkipNode<T> ** head=NULL;
    int maxlevel = 0;
public:
    SkipList(){
        head=new SkipNode<T> * [maxlevel+1];
    }
    ~SkipList(){
        SkipNode<T> * ptr=head[0];
        SkipNode<T> * _ptr;
        while (ptr!=NULL) {
            _ptr=ptr;
            ptr=ptr->next[0];
            delete _ptr;
        }
        delete head;
    }
    void insert(T data){
        SkipNode<T> * p = new SkipNode<T>(data);
        if (p->level>maxlevel) { // 新节点的level比表的最大level高
            updatelevel(p->level);
        }
        // 跳到该层
        SkipNode<T> *cur=head[maxlevel];
        for (int i=maxlevel; i>=0;i-- ) {
            
            
            //如果这层是新建立出来的新层，便插入节点
            if (cur==NULL && i<=p->level) {
                head[i]=p;
                cur=head[i-1];
                continue;
            }
            if (head[i]->data > p->data) {
                if (i <= p->level ) {
                    //插入第一位
                    p->next[i]=head[i];
                    head[i]=p;
                    
                }
                //注意：如果没有横向移动，向下移动指针
                cur=head[i-1];
                continue;
            }
            while (cur->data <= p->data) {//z注意这里左闭
                if (cur->next[i]==NULL || p->data < cur->next[i]->data) {//右开 可以保持原始顺序
                    if(i<=p->level){
                        p->next[i]=cur->next[i];
                        cur->next[i]=p;
                    }
                    break;
                }else{
                    cur=cur->next[i];
                }
            }
        }
    }
    void printlist(){
        // 打印头
        printf("data\t\t");
        for (int i=0; i<=maxlevel; i++) {
            printf("%d\t",head[i]);
        }
        SkipNode<T> * p= head[0];
        while (p!=NULL) {
            cout<<endl;
            printf("%d:",p);
            cout<<p->data<<"\t";
            for (int i=0; i<=maxlevel; i++) {
                if(i<=p->level)printf("%7d\t",(p->next[i]));
                else printf("\t|\t");
            }
            p=p->next[0];
        }
        cout<<endl;
    }
    void updatelevel(int l){
        SkipNode<T> ** newhead =new SkipNode<T> * [l+1];
        for (int i=0; i<= l; i++) {
            if(i<=maxlevel){
                newhead[i]=head[i];
            }else{
                newhead[i]=NULL;
            }
        }
        delete head;
        maxlevel=l;
        head=newhead;
    }
    void alterlevel(){
        int i=maxlevel;
        while (head[i]==NULL) {
            i--;
        }
        if (i==maxlevel) {
            return;
        }
        maxlevel=i;
        SkipNode<T> ** newhead=new SkipNode<T> *[i+1];
        for (; i>=0; i--) {
            newhead[i]=head[i];
        }
        delete head;
        head=newhead;
        
    }
    void deletenode(T data){
        SkipNode<T> *cur=head[maxlevel];
        SkipNode<T> *dp=NULL;
        // 为了对应重复出现的情况，临时存储需要替换的指针
        Stack<SkipNode<T> **> stk;
        
        for (int i=maxlevel; i>=0;i-- ) {
            if (head[i]->data >= data) {
                if (head[i]->data == data){
                    if (dp==NULL) {
                        dp=head[i];
                        stk.push(& head[i]);
                    }else if (dp==head[i]){
                        stk.push(& head[i]);
                    }else{
                        stk.popall();
                        dp=head[i];
                        stk.push(& head[i]);
                    }
                    //                    dp=head[i];
                    //                    head[i]=head[i]->next[i];
                    
                }
                //注意：如果没有横向移动，向下移动指针
                cur=head[i-1];
                continue;
            }
            while (cur->data < data) {
                
                if (cur->next[i]==NULL || data <= cur->next[i]->data) {
                    //最后一个小于data向下走
                    if (cur->next[i]!=NULL &&cur->next[i]->data == data) {
                        if (dp==NULL) {
                            dp=cur->next[i];
                            stk.push(& cur->next[i]);
                        }else if (dp==cur->next[i]){
                            stk.push(& cur->next[i]);
                        }else{
                            stk.popall();
                            dp=cur->next[i];
                            stk.push(& cur->next[i]);
                        }
                        //                        dp=cur->next[i];
                        //                        cur->next[i]=cur->next[i]->next[i];
                    }
                    break;
                }else{
                    cur=cur->next[i];
                }
            }
        }
        
        if (dp!=NULL) {
            for (int i=0;!stk.empty();i++) {
                SkipNode<T> ** skn = stk.pop();
                *skn=dp->next[i];
            }
            if (dp->level==maxlevel) {
                alterlevel();
            }
            delete dp;
        }
        
    }
    void deletelevel(){
        int i=maxlevel;
        while (head[i]==NULL) i--;
        SkipNode<T> ** newhead =new SkipNode<T> * [i+1];
        maxlevel=i;
        for (; i>=0; i--) {
            newhead[i]=head[i];
        }
        delete head;
        head=newhead;
    }
    bool find(T data ){
        SkipNode<T> *cur=head[maxlevel];
        for (int i=maxlevel; i>=0;i-- ) {
            if (head[i]->data >= data) {
                if (head[i]->data == data)return true;
                //注意：如果没有横向移动，向下移动指针
                cur=head[i-1];
                continue;
            }
            while (cur->data <= data) {//z注意这里左闭
                if (cur->data == data) return true;
                if (cur->next[i]==NULL || data < cur->next[i]->data) {//右开 可以保持原始顺序
                    break;
                }else{
                    cur=cur->next[i];
                }
            }
        }
        return false;
    }
    
};
//test
//SkipList<int> l;
//for (int i=0; i<100; i++) {
//    l.insert(rand()%100);
//}
//l.printlist();
//



#endif
