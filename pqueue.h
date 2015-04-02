//
//  pqueue.h
//  algorithm
//
//  Created by liu on 15/3/13.
//  Copyright (c) 2015年 liu. All rights reserved.
//

#ifndef algorithm_pqueue_h
#define algorithm_pqueue_h

#include "string"
#include "iostream"

using namespace std;
#define PQUEUELENGTH 20
template <class T>
class Pqueue {
    T * dataarr;
    Pqueue *next=NULL;
    unsigned int i=0;//用于保存末尾所在位置,0空，= maxnum 满
    unsigned int maxnum=0;
public:
    Pqueue(unsigned int max=PQUEUELENGTH){
        maxnum=max;
        dataarr=new T[max];
    }
    ~Pqueue(){
        delete dataarr;
        if (next!=NULL) {
            delete next;
        }
    }
    unsigned int getindex(){return i;}
    void insert(T data){
        // 满检查，如果满，建立新的堆来插入元素
        if (i>=maxnum) {
            if (next==NULL) {
                next=new Pqueue<T>(maxnum);
            }
            next->insert(data);
            return;
        }
        //使用新的i替换this->i 防止对i更改;
        int i=this->i;
        dataarr[i]=data;
        while (i>0 && dataarr[i/2]<dataarr[i]) {
            // 交换父子对象位置
            T tmp=dataarr[i/2];dataarr[i/2]=dataarr[i];dataarr[i]=tmp;
            i/=2;
        }
        this->i++;
    }
    
    T maximum(){
        T max;
        if (next!=NULL) {
            max=next->maximum();
            if (max>dataarr[0]) {
                return max;
            }else{
                return dataarr[0];
            }
        }else{
            return dataarr[0];
        }
    }
    bool empty(){
        return i==0;
    }
    T extractmax(){
        if (next!=NULL && !next->empty()) {
            T nextmax=next->maximum();
            if (this->empty() || nextmax>=dataarr[0]) {
                // 如果之后的堆中的最大元素大，则输出之后的。
                return next->extractmax();
            }
        }
        // 去除此堆的最大元素
        if (empty()) {
            string err("pqueue empty");
            throw err;
        }
        T max=dataarr[0];
        dataarr[0]=dataarr[i-1];
        i--;
        maxheapify(0);
        //        // 每次删除将之后的堆向前移动。则打开代码
        //        if (i>=maxnum && next!=NULL && !(next->empty())) {
        //            // 如果是满的去除元素，就要从后面的拿过来一个元素
        //            insert(next->extractmax());
        //            if(next->empty()){delete next;next=NULL;}
        //        }
        return max;
    }
private:
    void maxheapify(unsigned int index){
        unsigned int l=index*2+1;
        unsigned int r=index*2+2;
        unsigned int maxindex;
        if (l <= i && dataarr[l]>dataarr[i]) {
            maxindex=l;
        }else{
            maxindex=index;
        }
        if (r<=i && dataarr[r]>dataarr[maxindex]) {
            maxindex=r;
        }
        if (maxindex!=index) {
            // 交换最大值位置
            T tmp=dataarr[maxindex];dataarr[maxindex]=dataarr[index];dataarr[index]=tmp;
            maxheapify(maxindex);
        }
    }
};

//test
//Pqueue<int> pq(60);
//for (int i=0; i<100; i++) {
//    pq.insert(rand()%100);
//}
//while (!pq.empty()) {
//    cout<<pq.extractmax()<<" ";
//    }
//    
#endif
