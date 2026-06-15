#include <bits/stdc++.h>
using namespace std;
string out;
char buf[16];
void emitNum(int x){ int len=sprintf(buf,"%d",x); out.append(buf,len);}
void opAdd(int i){ out.push_back('+'); emitNum(i);}
void opRem(int i){ out.push_back('-'); emitNum(i);}
void opUp(int i){ out.append("++"); emitNum(i);}   // i -> i+1
void opDown(int i){ out.append("--"); emitNum(i);} // i -> i-1

void F(int n,int k);
void R(int n,int k);

void F(int n,int k){
    if(k==0||n==0) return;
    if(n==1){ opAdd(1); return;}
    if(k==1){
        opAdd(1);
        for(int i=1;i<=n-1;i++) opUp(i);
        return;
    }
    F(n-1,k);
    opAdd(n);
    R(n-1,k-1);
}
void R(int n,int k){
    if(k==0||n==0) return;
    if(n==1){ opRem(1); return;}
    if(k==1){
        for(int i=n;i>=2;i--) opDown(i);
        opRem(1);
        return;
    }
    F(n-1,k-1);
    opRem(n);
    R(n-1,k);
}
int main(){
    int n,k;
    if(scanf("%d %d",&n,&k)!=2) return 0;
    out.reserve(1<<22);
    F(n,k);
    opRem(n);
    printf("%s\n", out.c_str());
    return 0;
}
