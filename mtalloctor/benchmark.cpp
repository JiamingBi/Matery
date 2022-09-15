#include<iostream>
#include <sys/time.h>
#include <mpp.hpp>
using namespace std;
int main(){
    struct timeval t1,t2;
    void *ptr;
    double timeuse;
    gettimeofday(&t1,NULL);
    for(long i=0;i<50000000;i++){
        srand(time(NULL));
        ptr=malloc(rand()%4096);
        free(ptr);
    }
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    cout<<"time = "<<timeuse<<endl;  //输出时间（单位：ｓ）
    gettimeofday(&t1,NULL);
    for(long i=0;i<50000000;i++){
        srand(time(NULL));
        ptr=mpp::MemoryAllocator::Allocate(rand()%4096);
        mpp::MemoryAllocator::Deallocate(ptr);
    }
    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;

    cout<<"time = "<<timeuse<<endl;  //输出时间（单位：ｓ）

}