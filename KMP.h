//
//  KMP.h
//  algorithm
//
//  Created by liu on 15/3/13.
//  Copyright (c) 2015年 liu. All rights reserved.
//

#ifndef algorithm_KMP_h
#define algorithm_KMP_h

#include <iostream>
#include "mystl.h"
#include <string>
using namespace std;

class KMP {
    int * next=NULL;
    char * matchstr=NULL;
    int len=0;
public:
    KMP(const char * str){
        complie(str);
    }
    ~KMP(){
        if (next!=NULL) delete next;
        if (matchstr!=NULL)  delete matchstr;
    }
    void complie(const char * str){
        if (next!=NULL) delete next;
        if (matchstr!=NULL)  delete matchstr;
        
        
        len=strlen(str);
        matchstr=new char[len+1];
        strcpy(matchstr, str);
        next=new int[len];
        for (int i=0; i<len; i++) {
            next[i]=calnext(str, i);
        }
    }
    void printnext(){
        for (int i=0; i<len; i++) {
            cout<<next[i]<<" ";
        }
        cout<<endl;
    }
    
    static int calnext(const char * a,int i){
        if (i==0) {
            return -1;
        }
        char * t = new char[i+1];
        
        strlcpy(t, a, i+1);
        
        int nxt = match(t);
        //cout<<t<<"("<<nxt<<")";
        delete t;
        //如果与首字符相同，且j的前面的1-—k个字符与开头的1-—k个字符不等（或者相等但T[k]==T[j]）（1≤k<j）
        if (a[i]==a[0] && ( nxt == 0 || a[nxt] == a[i] )) {
            return -1;
        }else if(a[nxt] != a[i]){
            return nxt;
        }else{
            return 0;
        }
        
    }
    static int match(char * a){
        int len=strlen(a);
        for (int i=1; i<len; i++) {
            if (cmp(a, a+i)) {
                return len-i;
            }
        }
        return 0;
    }
    static bool cmp(char * a,char * b){
        while ((*a!='\0')&&(*b!='\0')) {
            if (*a!=*b) {
                return false;
            }
            a++,b++;
        }
        return true;
    }
    // 查找函数，可以设置跳过前skip个
    // 函数返回第一次找到的index（0开始），参数skip=index可以快速查找去除index之后的第一个匹配
    int find(const char * text,int skip=-1){
        int i=skip+1,j=0;
        while(text[i]!='\0'){
            if (text[i]==matchstr[j]) {
                i++,j++;
            }else if(next[j]==-1){
                j=0;i++;
            }else{
                j=next[j];
            }
            if(j==len)return i-len;
        }
        return -1;//failure
    }
    // 传入新int指针
    int findall(const char * text,int * & list){
        Queue<int> indexqueue;
        int index=-1;
        int num=0;
        while (-1!=(index=find(text,index))) {
            indexqueue.enqueue(index);
            num++;
        }
        if (num!=0) {
            list = new int[num];
            for (int i=0;!indexqueue.empty();i++) {
                list[i]=indexqueue.dequeue();
            }
        }
        return num;
    }
    
};
//test
//
//KMP kmp("你好");
//int * intp=NULL;
//int n=kmp.findall("你好你好我好大家好。你好吗？好", intp);
//for (int i=0; i<n; i++) {
//    cout<<intp[i]<<" ";
//    }
//    delete intp;
//    cout<<endl;


#endif
