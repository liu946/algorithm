//
//  hashtable.h
//  al2
//
//  Created by liu on 15/5/10.
//  Copyright (c) 2015年 liu. All rights reserved.
//
#ifndef HASHTABLE_H
#define HASHTABLE_H


#include <functional>

#define MAXLONG 4
template <class KeyType,class ElementType>
class hashtable {
    struct node{
        KeyType k;
        ElementType e;
        node * listlast=NULL;
        node * listnext;
        node * next;
        node(KeyType _k,node * l, node * n):k(_k),listnext(l),next(n){}
    };
    node ** dataptr;
    node * list=NULL;
    hash<KeyType> gethash;
    unsigned int maxlong;
    unsigned int num;
    constexpr const static float maxloaded = 0.75;
    unsigned hash(KeyType a){
        unsigned int x = (unsigned)gethash(a);
        x%=maxlong;return x;
    }
    node * newnode(KeyType k){
        node * & p = dataptr[hash(k)];
        p = new node(k,list,p);
        if (list!=NULL) {
            list->listlast=p;
        }
        
        list=p;
        num++;
        return p;
    }
    node * exist(KeyType k){
        node * p = dataptr[hash(k)];
        while (p!=NULL) {
            if (p->k==k) {
                return p;
            }
            p=p->next;
        }
        return NULL;
    }
    
public:
    hashtable(unsigned _maxlong=MAXLONG){
        maxlong=_maxlong;
        dataptr = new node * [maxlong];
        for (unsigned i=0; i<maxlong; i++) {
            dataptr[i]=NULL;
        }
    }
    ~hashtable(){
        deletetable(dataptr, list);
    }
    static void deletetable(node ** dp,node * lp){
        // 清除散列结构数据
        node * temp_ptr1 = lp;
        node * temp_ptr2;
        while (temp_ptr1) {
            temp_ptr2=temp_ptr1;
            temp_ptr1=temp_ptr1->listnext;
            delete temp_ptr2;
        }
        //
        delete [] dp;
    }
    ElementType & operator[](KeyType k){
        node * ptr=exist(k);
        if (!ptr) {
            if (maxlong * maxloaded <= (float)num) {
                // 散列表的扩张
                node ** olddata=dataptr;
                node * oldlist=list;
                dataptr = new node * [maxlong*=2];
                list=NULL;
                num=0;
                for (unsigned i=0; i<maxlong; i++) {
                    dataptr[i]=NULL;
                }
                for (node * tp=oldlist;tp; tp=tp->listnext) {
                    (*this)[tp->k]=tp->e;
                }
                deletetable(olddata, oldlist);
            }
            ptr=newnode(k);
        }
        return ptr->e;
    }
    ElementType & get(KeyType k){
        node * ptr=exist(k);
        if (!ptr) {
            throw string("KeyError");
        }
        return ptr->e;
    }
    void unset(KeyType k){
        node * p = dataptr[hash(k)];
        node ** p2= &dataptr[hash(k)];
        while (p!=NULL) {
            if (p->k==k) {
                // 横链上删除此节点
                (*p2)=p->next;
                // 双向链上删除节点
                if (!(p->listlast)) {
                    p->listlast->listnext=p->listnext;
                }else{
                    this->list=p->listnext;
                }
                if (!(p->listnext)) {
                    p->listnext->listlast=p->listlast;
                }
                delete p;
                num--;
                return;
            }
            p2=&(p->next);
            p=p->next;
            
        }
    }

};
// example:

//    hashtable<string,string> s;
//    string str[]={"111","112","121","122","211","212","221","222"};
//    for (int i=0; i<7; i++) {
//        s[str[i]]=str[i+1];
//    }
//    for (int i=0; i<7; i++) {
//        cout<<s.get(str[i]);cout<<endl;
//    }

#endif