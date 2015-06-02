//
//  sort.h
//  al2
//
//  Created by liu on 15/5/24.
//  Copyright (c) 2015年 liu. All rights reserved.
//

#include <math.h>
#include "iostream"
using namespace std;
//堆排序
//整理节点time:O(lgn)
template <typename T>
inline void simpleswap(T & a,T & b) {
    T t =a;
    a = b;
    b = t;
}
template<typename T>
void MinHeapify( T * arry,int size,int element){
    int lchild=element*2+1,rchild=lchild+1;//左右子树
    while(rchild<size)//子树均在范围内
    {
        if(arry[element]>=arry[lchild] && arry[element]>=arry[rchild])//如果比左右子树都小，完成整理
        {
            return;
        }
        if(arry[lchild]>=arry[rchild])//如果左边最大
        {
            simpleswap<T>(arry[element],arry[lchild]);//把左面的提到上面
            element=lchild;//循环时整理子树
        }
        else//否则右面最大
        {
            simpleswap<T>(arry[element],arry[rchild]);//同理
            element=rchild;
        }
        lchild=element*2+1;
        rchild=lchild+1;//重新计算子树位置
    }
    if(lchild<size&&arry[lchild]>arry[element])//只有左子树且子树小于自己
    {
        simpleswap<T>(arry[lchild],arry[element]);
    }
    return;
}
//堆排序time:O(nlgn)
template<typename T>
void HeapSort(T * arry,int size){
    int i;
    for(i=size-1;i>=0;i--)//从子树开始整理树
    {
        MinHeapify(arry,size,i);
    }
    while(size>0)//拆除树
    {
        simpleswap<T>(arry[size-1],arry[0]);//将根（最小）与数组最末交换
        size--;//树大小减小
        MinHeapify(arry,size,0);//整理树
    }
    return;
}
/*
    快排调整
 */
template <class T>
inline T * __unguarded_partition(T * first,T * last,T media) {
    while (first<last) {
        while (*first<=media)first++;
        while (*last>=media)last--;
        if (first>last)break;
        simpleswap<T>(*first,*last);
    }
    return first;
}


const unsigned int __stl_threshold = 16;

inline unsigned int __lg(unsigned int a){ //对数计算
    unsigned int i=0;
    while (!(a<<i & 0x80000000)) {
        i++;
    }
    return 32-i;
}
template <class T>
void __final_instertion_sort(T* first,unsigned int n) {
    for (int i=1; i<n; i++) {
        T temp = *(first+i);
        int j;
        for (j=i; j>0 && *(first +j-1)>temp; j--) {
            *(first+j)=*(first+j-1);
        }
        *(first+j) = temp;
    }
}
template <class T>
void __introsort_loop(T * first,T * last,unsigned int depth_limit);
template <class T>
inline void sort(T * first, T * last) {
    if (first != last) {
        __introsort_loop<T>(first, last,  __lg(last - first) << 1);
        __final_instertion_sort<T>(first, last-first);
    }
}
template <class T>
inline T  __median(T a,T b,T c) {
    if((a>b && b>c) || (c>b && b>a)){
        return b;
    }else if((a>c && c>b)||(b>c && c>a)){
        return c;
    }else{
        return a;
    }
}

template <class T>
void __introsort_loop(T * first,
                      T * last,
                      unsigned int depth_limit) {
    while (last - first > __stl_threshold) {            // 技巧1 ：小于某阈值时放弃排序
        if (depth_limit == 0) {
            HeapSort(first, last-first);            // 技巧2 ： 如果迭代深度过深，说明快排在此情况下退化，使用堆排序改善。
            return;
        }
        --depth_limit;
        T * cut = __unguarded_partition(first, last-1, T(__median(*first, *(first + (last - first)/2),*(last - 1))));    // 技巧3 ：三点取中
        __introsort_loop(cut, last,  depth_limit);  // 技巧4 ：使用循环可以减少一半递归调用
        last = cut;
    }
}
/* 测试三分点
template <class T>
void __introsort_loop(T * first,
                      T * last,
                      unsigned int depth_limit) {
    while (last - first > __stl_threshold) {            // 技巧1 ：小于某阈值时放弃排序
        if (depth_limit == 0) {
            HeapSort(first, last-first);            // 技巧2 ： 如果迭代深度过深，说明快排在此情况下退化，使用堆排序改善。
            return;
        }
        --depth_limit;
        T * cut = __unguarded_partition(first,
                                        last-1, T(
            __median(*first,
                     *(first + (last - first)*(depth_limit%2+1)/3),
                     *(last - 1))));    // 技巧3 ：三点取中
        __introsort_loop(cut, last,  depth_limit);  // 技巧4 ：使用循环可以减少一半递归调用
        last = cut;
    }
}
//*/










