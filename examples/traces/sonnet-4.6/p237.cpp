#include <bits/stdc++.h>
using namespace std;
int par[600];
char val[600];
int find_(int x){return par[x]==x?x:par[x]=find_(par[x]);}
bool merge(int a,int b,vector<char>&s){
    a=find_(a);b=find_(b);
    if(a==b)return true;
    if(val[a]!=0&&val[b]!=0&&val[a]!=val[b])return false;
    if(val[a]!=0)par[b]=a;
    else{par[a]=b;if(val[a]==0)val[b]=val[b];}
    if(val[a]!=0)val[find_(a)]=val[a];
    else val[find_(b)]=val[b];
    return true;
}
// This approach is getting complicated, let me write clean solution
int main(){return 0;}
