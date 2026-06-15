#include <bits/stdc++.h>
using namespace std;
int main(){
    int digit[10],up_btn,down_btn,dash_btn;
    int a,b,c,d;
    cin>>a>>b>>c>>d; digit[1]=a;digit[2]=b;digit[3]=c;up_btn=d;
    cin>>a>>b>>c>>d; digit[4]=a;digit[5]=b;digit[6]=c;down_btn=d;
    cin>>a>>b>>c; digit[7]=a;digit[8]=b;digit[9]=c;
    cin>>a>>b; dash_btn=a;digit[0]=b;
    int X,Y; cin>>X>>Y;
    if(X==Y){cout<<0;return 0;}
    vector<int> dist(100,INT_MAX);
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>> pq;
    dist[X]=0; pq.push({0,X});
    while(!pq.empty()){
        auto[cost,u]=pq.top();pq.pop();
        if(cost>dist[u])continue;
        if(up_btn){int v=(u+1)%100;if(dist[u]+1<dist[v]){dist[v]=dist[u]+1;pq.push({dist[v],v});}}
        if(down_btn){int v=(u-1+100)%100;if(dist[u]+1<dist[v]){dist[v]=dist[u]+1;pq.push({dist[v],v});}}
        for(int c2=0;c2<=9;c2++)if(digit[c2]){int v=c2;if(dist[u]+1<dist[v]){dist[v]=dist[u]+1;pq.push({dist[v],v});}}
        if(dash_btn)for(int c2=10;c2<=99;c2++){int d1=c2/10,d2=c2%10;if(digit[d1]&&digit[d2]){if(dist[u]+3<dist[c2]){dist[c2]=dist[u]+3;pq.push({dist[c2],c2});}}}
    }
    cout<<(dist[Y]==INT_MAX?-1:dist[Y]);
}
