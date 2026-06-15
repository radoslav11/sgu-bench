#include <bits/stdc++.h>
using namespace std;
int main(){
    double s,m,p;
    cin>>s>>m>>p;
    double r=p/100.0;
    double x;
    if(p==0) x=s/m;
    else x = s*r/(1-pow(1+r,-m));
    printf("%.5f\n",x);
    return 0;
}
