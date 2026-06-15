#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d",&n);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    long long sum=0;
    int finalBeauty=0;
    bool possible=true;
    for(int i=2;i<=n;i++){
        char c[4];
        int v;
        scanf("%s %d",c,&v);
        if(c[0]=='d'){
            pq.push({v,i});
            sum+=v;
        } else {
            // princess
            if(i==n){
                finalBeauty=v;
            } else {
                int cap=v-1;
                while((int)pq.size()>cap){
                    sum-=pq.top().first;
                    pq.pop();
                }
            }
        }
    }
    if((int)pq.size()<finalBeauty){
        printf("-1\n");
        return 0;
    }
    vector<int> kills;
    while(!pq.empty()){
        kills.push_back(pq.top().second);
        pq.pop();
    }
    sort(kills.begin(),kills.end());
    printf("%lld\n",sum);
    printf("%d\n",(int)kills.size());
    for(size_t i=0;i<kills.size();i++){
        printf("%d%c",kills[i], i+1==kills.size()?'\n':' ');
    }
    if(kills.empty()) printf("\n");
    return 0;
}
