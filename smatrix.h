//
//  smatrix.h
//  algorithm
//
//  Created by liu on 15/3/19.
//  Copyright (c) 2015年 liu. All rights reserved.
//

#ifndef algorithm_smatrix_h
#define algorithm_smatrix_h
#include "string"
#include "iostream"
#include "mystl.h"
using namespace std;

class mnode {
public:
    unsigned int i,j;
    int data;
    mnode * next;
    mnode(unsigned int ii,unsigned int jj,int d,mnode * n=NULL){
        i=ii;j=jj;data=d;next=n;
    }
};


class smatrix {
    mnode * head=NULL;
    unsigned int c=0,r=0;
public:
    smatrix(unsigned int rr ,unsigned int cc){
        r=rr;c=cc;
    }
    void set(unsigned int i,unsigned int j,int d){
        if (i>=r||j>=c) {
            throw string("out of index");
        }
        if (head==NULL ) {
            
            if (d==0) {
                return;
            }
            head=new mnode(i,j,d,head);
            return;
        }
        if (head->i == i && head->j == j) {
            if (d==0) {
                mnode *p=head;
                head=head->next;
                delete p;
                return;
            }
            head->data=d;
            return;
        }else if( head->i > i||(head->i == i && head->j > j )){
            if (d==0) {
                return;
            }
            head=new mnode(i,j,d,head);
            return;
        }
        mnode * p=head;
        for (; p->next!=NULL && (p->next->i < i||( p->next->i == i && p->next->j < j )); p=p->next);
        if (p->next==NULL|| p->next->i!=i || p->next->j != j) {
            // insert
            if (d==0) {
                return;
            }
            mnode *pp=p->next;
            p->next=new mnode(i,j,d,pp);
        }else{
            if (d==0){
                mnode *pp=p->next;
                p->next=p->next->next;
                delete pp;
                return;
            }
            p->next->data=d;
        }
    }
    int get(unsigned int i,unsigned int j){
        mnode * p=head;
        while (p!=NULL ) {
            if ( p->i==i && p->j==j) {
                return p->data;
            }
            p=p->next;
        }
        throw string("no index");
    }
    void T(){
        mnode * p=head;
        exchange(r, c);
        Queue<mnode *> * queue=new Queue<mnode *> [r];
        while (p!=NULL ) {
            exchange(p->i, p->j);
            queue[p->i].enqueue(p);
            p=p->next;
        }
        p=NULL;
        for (int i=0; i<r; i++) {
            while (queue[i].empty())i++;
            while (!queue[i].empty()) {
                if (p==NULL) {
                    p=head=queue[i].dequeue();
                }else{
                    p->next=queue[i].dequeue();
                    p=p->next;
                }
            }
        }
        p->next=NULL;
        delete [] queue;
        
    }
    void exchange(unsigned int & i,unsigned int & j){
        unsigned int t= i;i=j;j=t;
    }
    void printm(){
        mnode * p=head;
        for (unsigned int i=0; i<r; i++) {
            for (unsigned int j=0; j<c; j++) {
                if (p!=NULL && p->i==i && p->j==j) {
                    printf("%5d",p->data);
                    p=p->next;
                }else{
                    printf("%5d",0);
                }
            }
            cout<<endl;
        }
    }
};
//test
//smatrix a(10,11);
//for (int i=0; i<1000; i++) {
//    a.set(rand()%10, rand()%11, rand()%100);
//}
//for (int i=0; i<100; i++) {
//    a.set(rand()%10, rand()%11, 0);
//}
//a.printm();
//a.T();
//cout<<endl;
//a.printm();
#endif
