#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    if(scanf("%d",&n)!=1) return 0;
    vector<long long> d(n);
    for(int i=0;i<n;i++){
        double x;
        scanf("%lf",&x);
        long long um = llround(x*1000000.0);
        d[i]=um-2000000;
    }
    vector<int> pos, neg;
    for(int i=0;i<n;i++){
        if(d[i]>=0) pos.push_back(i);
        else neg.push_back(i);
    }
    int pi=0, ni=0;
    long long sum=0;
    vector<int> order;
    for(int k=0;k<n;k++){
        if(sum<=0){
            if(pi<(int)pos.size()){ int j=pos[pi++]; sum+=d[j]; order.push_back(j+1); }
            else { int j=neg[ni++]; sum+=d[j]; order.push_back(j+1); }
        } else {
            if(ni<(int)neg.size()){ int j=neg[ni++]; sum+=d[j]; order.push_back(j+1); }
            else { int j=pos[pi++]; sum+=d[j]; order.push_back(j+1); }
        }
    }
    printf("yes\n");
    for(int i=0;i<n;i++) printf("%d%c", order[i], i+1<n?' ':'\n');
    return 0;
}
