#include <bits/stdc++.h>
using namespace std;

int n;
long long bit[65540];

void update(int i){
    for(; i<=n; i+=i&-i) bit[i]++;
}
long long query(int i){
    long long s=0;
    for(; i>0; i-=i&-i) s+=bit[i];
    return s;
}

int main(){
    scanf("%d",&n);
    vector<int> a(n);
    for(int i=0;i<n;i++) scanf("%d",&a[i]);
    vector<int> vals(a);
    sort(vals.begin(),vals.end());
    vals.erase(unique(vals.begin(),vals.end()),vals.end());
    long long ans=0;
    for(int i=0;i<n;i++){
        int r=lower_bound(vals.begin(),vals.end(),a[i])-vals.begin()+1;
        ans += query(n)-query(r);
        update(r);
    }
    printf("%lld\n",ans);
    return 0;
}
