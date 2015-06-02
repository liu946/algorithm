//
//  devhashtable.h
//  al2
//
//  Created by liu on 6/2/15.
//  Copyright (c) 2015 liu. All rights reserved.
//

#ifndef al2_devhashtable_h
#define al2_devhashtable_h
#include <iostream>

using namespace std;
#define max 200
template<class T,class vT>
class DevHashTable {
    struct node{
        T key;
        vT value;
        int nextseek=0;
        node(T k=0,vT v=0,int n=0):key(k),value(v),nextseek(n){}
    };
    hash<T> gethash;
    unsigned hash(T a){
        unsigned int x = (unsigned)gethash(a);
        x%=max;return x;
    }
    FILE * f;
    int num=max * sizeof(int);
public:
    DevHashTable(string a = "data.binary"){
        f=fopen(a.c_str(), "wb+");
        if(f==NULL){
            printf("failed to open the file!!!");
            return;
        }
        fseek(f, sizeof(int)*max, SEEK_SET);
        int t=0;
        fwrite(&t, 4, 1, f);
    }
    ~DevHashTable(){
        fclose(f);
    }
    void set(T k,vT value){
        fseek(f, sizeof(int)*hash(k), SEEK_SET);
        int index;node tn(k,value,0);
        fread(&index, 4, 1, f);
        if (index==0) {
            fseek(f, sizeof(int)*hash(k), SEEK_SET);
            fwrite(&num, 4, 1, f);
            
            fseek(f, num, SEEK_SET);
            fwrite(&tn, sizeof(tn), 1, f);
            num+=sizeof(node);
            return;
        }
        while (index!=0) {
            
            fseek(f, index, SEEK_SET);
            fread(&tn, sizeof(tn), 1, f);
            if (tn.key == k) {
                tn.value =value;
                fseek(f, index, SEEK_SET);
                fwrite(&tn, sizeof(tn), 1, f);
                return;
            }
            if(!tn.nextseek)break;
            index = tn.nextseek;
        }
        fseek(f, index, SEEK_SET);
        tn.nextseek = num;
        
        fwrite(&tn, sizeof(tn), 1, f);
        node tns(k,value,0);
        fseek(f, num, SEEK_SET);num+=sizeof(node);
        fwrite(&tns, sizeof(tn), 1, f);
    }
    vT get(T k){
        fseek(f, sizeof(int)*hash(k), SEEK_SET);
        int index;node tn(0,0,0);
        fread(&index, 4, 1, f);
        while (index!=0) {
            fseek(f, index, SEEK_SET);
            fread(&tn, sizeof(node), 1, f);
            if (tn.key == k) {
                return tn.value;
            }
            if(!tn.nextseek)break;
            index = tn.nextseek;
        }
        throw string("");
        return 0;
    }
};
//
// examples
//
//int main() {
//    DevHashTable<int,int> d;
//    for (int a=0; a<500; a++) {
//        d.set(rand()%200, rand());
//    }
//    for (int i=0; i<200; i++) {
//        try {
//            int a =d.get(i);
//            cout<<i<<": "<<a<<endl;
//        } catch (string) {
//            cout<<i<<": "<<endl;
//        }
//    }
//    return 0;
//}

#endif
