//
//  main.cpp
//  algorithm
//
#include "string"
#include "iostream"
#include "mystl.h"
#include "smatrix.h"
#include "skiplist.h"
#include "pqueue.h"
#include "KMP.h"
#include "autoaffman.h"
using namespace std;
int main(int argc, const char * argv[]) {
    //请压缩10k以下未压缩简单位图-bmp
    autohuffman hfm;
    hfm.encode("/Users/liu/Desktop/a.bmp","/Users/liu/Desktop/title.hfm");
    autohuffman hfm2;
    hfm2.decode("/Users/liu/Desktop/title.hfm","/Users/liu/Desktop/s.bmp");
    return 0;
}
