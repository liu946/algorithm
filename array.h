//
//  array.h
//  algorithm
//
//  Created by liu on 15/3/19.
//  Copyright (c) 2015年 liu. All rights reserved.
//

#ifndef algorithm_array_h
#define algorithm_array_h
#include "string"
#include "iostream"
#include "mystl.h"
using namespace std;

template <class T>
class Array {
    unsigned int maxindex=0;
    T * data=NULL;
public:
    Array(unsigned int m=50){
        maxindex=m;
        data=new T[m];
    }
    ~Array(){
        if (data!=NULL) {
            delete data;
        }
    }
    T & operator[](unsigned int i){
        if (i<maxindex) {
            return data[i];
        } else {
            maxindex*=2;
            T * p=new T[maxindex];
            for (unsigned int j=0; j<maxindex/2; j++) {
                p[j]=data[j];
            }
            delete data;
            data=p;
            return data[i];
        }
    }
};
#endif
