//
//  autoaffman.h
//  algorithm
//
//  Created by liu on 15/3/30.
//  Copyright (c) 2015年 liu. All rights reserved.
//

#ifndef algorithm_autoaffman_h
#define algorithm_autoaffman_h
#include "mystl.h"
using namespace std;
class binary{
public:
    string binstr="";
    unsigned long bitnum=0;
    //构造函数，空、使用01串、使用内存块
    binary(){}
    binary(string strwith01){
        binstr=strwith01;
        bitnum=strwith01.length();
    }
    binary(char * buf,unsigned long _bitnum){
        initbinary(buf,_bitnum);
    }
    void initbinary(char * buf,unsigned long _bitnum){
        bitnum=_bitnum;
        char ct=0x1;
        binstr="";
        buf+=_bitnum/8-1;
        for (;; buf--) {
            for (int j=7; j>=0; j--) {
                if ( (*buf) &(ct<<j)  ) {
                    binstr+="1";
                }else{
                    binstr+="0";
                }
                if (--_bitnum==0) {
                    return;
                }
            }
        }
    }
    //得到01二进制串
    string getbinstring(){
        return binstr;
    }
    //按位打印
    void printbinstr(){
        int b16=0;
        
        for (int i=0; i<bitnum; i++) {
            printf("%c",binstr[i]);
            if (binstr[i]=='1') {
                b16<<=1;
                b16+=1;
                
            }else{
                b16<<=1;
            }
            if (!((i+1)%4)) {
                printf(" %x\n",b16);
                b16=0;
            }
        }
    }
    //使用二进制写内存
    void getbindata(char * binp){
        binp+=bitnum/8-1;
        *binp=0;
        for (int i=0;i<binstr.length(); i++) {
            if (binstr[i]=='1') {
                (*binp)|=0x1<<(7-i%8);
            }
            if (!((i+1)%8)) {
                binp--;
                *binp=0;
            }
        }
        
    }
    //按二进制写入文件
    void savetofile(string filename){
        fstream file(filename,ios::out|ios::binary);
        file.write((char*) &bitnum, sizeof(unsigned long));
        char * buf=new char[bitnum/8+1];
        
        getbindata(buf);
        file.write(buf, bitnum/8+2);
        delete [] buf;
        file.close();
        
    }
    //按二进制读取文件
    void loadformfile(string filename){
        fstream file(filename,ios::in|ios::binary);
        file.read((char*) &bitnum, sizeof(unsigned long));
        char * buf=new char[bitnum/8+1];
        file.read(buf, bitnum/8+1);
        
        file.close();
        initbinary(buf, bitnum);
        delete [] buf;
    }
    void savefile_c(const char * file){
        FILE * fp;
        fp=fopen(file, "wb");
        for(;binstr.length();){
            fprintf(fp, "%c",popbyte());
        }
        fclose(fp);
        
    }
    void loadfile_c(const char * file){
        FILE * fp;
        fp=fopen(file, "rb");
        char f;
        int i=0;
        while (~fscanf(fp,"%c",&f)) {
            i++;
            if (i==100) {
                i=0;
            }
            writebyte((unsigned char)f);
        }
        fclose(fp);
    }
    void writebit(bool bit){
        if (bit) {
            binstr+="1";
        }else{
            binstr+="0";
        }
        bitnum++;
    }
    void writebyte(unsigned char x){
        for (int i=0; i<8; i++,x=x<<1) {
            writebit (x&0x80);
        }
    }
    bool popbit(){
        bool ans=(binstr[0]=='1');
        binstr.erase(0,1);
        bitnum--;
        return ans;
    }
    unsigned char popbyte(){
        unsigned char a=0;
        for (int i=0; i<8; i++) {
            a=a<<1;
            if (binstr[0]=='1') {
                a=a | 0x01;
            }
            binstr.erase(0,1);
            bitnum--;
        }
        return a;
    }
};


class autohuffmannode {
public:
    int dt=0;//-1为空，-2为非叶节点
    int weight=0;
    autohuffmannode * left=NULL;
    autohuffmannode * right=NULL;
    autohuffmannode * parent=NULL;
    List<int> ctn;
    autohuffmannode(int _dt=-1,int _weight=0,autohuffmannode * _parent=NULL ):dt(_dt),weight(_weight),parent(_parent){
        ctn.append(_dt);
    }
    bool notleaf(){return dt<-1;}
    
};
class autohuffman {
    autohuffmannode * root;
    int weightarr[258];
    binary hfmcode;
    binary orgcode;
public:
    autohuffman(){
        root=new autohuffmannode(-1);
        for (int i=0; i<256; i++) {
            weightarr[i]=0;
        }
    }
    
    void decode(const char * filename,const char * tofile){
        hfmcode.loadfile_c(filename);
        
        while (hfmcode.binstr.length()) {
            autohuffmannode * p=root;
            while (p->notleaf()) {
                if(hfmcode.popbit()==false){
                    p=p->left;
                }else{
                    p=p->right;
                }
            }
            if (p->dt==-1) {
                // 如果是虚拟节点
                unsigned char c=hfmcode.popbyte();
                orgcode.writebyte(c);
                encodechar(c,false);
            }else{
                //如果不是虚拟节点
                unsigned char c=p->dt;
                orgcode.writebyte(c);
                encodechar(c,false);
            }
        }
        
        orgcode.savefile_c(tofile);
    }
    void encode(const char * filename,const char * tofile){
        FILE *fp;
        fp=fopen(filename, "rb");
        char f;
        int i=0;
        while (~fscanf(fp,"%c",&f)) {
            i++;
            if (i==100) {
                i=0;
            }
            encodechar((unsigned char)f);
        }
        fclose(fp);
        orgcode.printbinstr();
        orgcode.savefile_c(tofile);
    }
    void encodechar(int byte,bool output=true){
        autohuffmannode * p=root;
        if (root->ctn.find(byte)==-1 ) {
            //没找到，去匹配-1
            while(p->notleaf()){
                p->ctn.append(byte);
                p->weight++;
                meantain(p);
                if (p->left->ctn.find(-1)==-1) {
                    // out put
                    if(output){
                        //cout<<1;
                        orgcode.writebit(1);
                    }
                    
                    p=p->right;
                }else{
                    if (output) {
                        //cout<<0;
                        orgcode.writebit(0);
                    }
                    p=p->left;
                }
            }
            if (output) {
                orgcode.writebyte((char)byte);
                //cout<<(char)byte;
            }
            //找到空节点
            if (p->dt==-1) {
                p->dt=-2;//set as unleaf node
                p->weight++;
                p->ctn.append(byte);
                p->left=new autohuffmannode(byte,1,p);// set new char node
                p->right=new autohuffmannode(-1,0,p);// new abstract node
            }
        }else{
            //找到，去匹配byte
            while(p->notleaf()){
                p->weight++;
                meantain(p);
                if (p->left->ctn.find(byte)==-1) {
                    p=p->right;
                    if(output){
                        //cout<<1;
                        orgcode.writebit(1);
                    }
                }else{
                    if(output){
                        //cout<<0;
                        orgcode.writebit(0);
                    }
                    p=p->left;
                }
            }
            if (p->dt==byte) {
                p->weight++;
                meantain(p);
            }
            
        }
        
    }
    void exchange(autohuffmannode * & a,autohuffmannode * & b){
        autohuffmannode *temp=a;
        a=b;
        b=temp;
    }
    void meantain(autohuffmannode *  addedptr){
        // 主程序为一个层序遍历，用于保证编码最佳性
        Queue<autohuffmannode *> tq;
        if (root==NULL) {
            return;
        }
        tq.enqueue(root);
        while (!tq.empty()) {
            autohuffmannode* p= tq.dequeue();
            if (addedptr==p) {
                //如果先找到这个节点，说明前面没有更小的节点， 那么就不用换
                return ;
            }
            if(p->weight < addedptr->weight){
                //如果在之前找到了重量更小的需要换位
                meantainnode(p, addedptr);
                return ;
            }
            if (p->left!=NULL) {
                tq.enqueue(p->left);
            }
            if (p->right!=NULL) {
                tq.enqueue(p->right);
            }
        }
        return;
    }
    void meantainnode(autohuffmannode *p,autohuffmannode *addedptr){
        //p 与 addedptr 换位
        exchange(p->parent->left==p?p->parent->left:p->parent->right,
                 addedptr->parent->left==addedptr?addedptr->parent->left:addedptr->parent->right);
        exchange(p->parent, addedptr->parent);
        
        //调整上层包含字符
        while (p->parent!=NULL ) {
            List<int> txp(p->parent->left->ctn+p->parent->right->ctn);
            
            p->parent->ctn = txp;
            p->parent->weight=p->parent->left->weight+p->parent->right->weight;
            
            p=p->parent;
        }
        while (addedptr->parent!=NULL ) {
            List<int> txp(addedptr->parent->left->ctn+addedptr->parent->right->ctn);
            addedptr->parent->ctn = txp;
            addedptr->parent->weight=addedptr->parent->left->weight+addedptr->parent->right->weight;
            
            addedptr=addedptr->parent;
        }
        List<int> txp(root->left->ctn+root->right->ctn);
        root->ctn = txp;
        //
    }
    
};
#endif


//examples
//autohuffman hfm;
//hfm.encode("/Users/liu/Desktop/a.bmp","/Users/liu/Desktop/title.hfm");
//autohuffman hfm2;
//hfm2.decode("/Users/liu/Desktop/title.hfm","/Users/liu/Desktop/s.bmp");
//
//
////    binary b;
////    for (int i=0; i<20; i++) {
////        b.writebit(i%2);
////    }
////    b.writebyte('V');
////    b.writebyte(254);
////    b.printbinstr();


