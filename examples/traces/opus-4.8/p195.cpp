#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d",&n);
    vector<int> par(n+1);
    for(int i=2;i<=n;i++) scanf("%d",&par[i]);
    vector<char> matched(n+1,0);
    vector<int> receivers;
    for(int i=n;i>=2;i--){
        if(!matched[i] && !matched[par[i]]){
            matched[i]=1;
            matched[par[i]]=1;
            receivers.push_back(i);
        }
    }
    sort(receivers.begin(),receivers.end());
    printf("%lld\n",(long long)receivers.size()*1000);
    for(size_t i=0;i<receivers.size();i++){
        printf("%d%c",receivers[i], i+1<receivers.size()?' ':'\n');
    }
    if(receivers.empty()) printf("\n");
    return 0;
}
