#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<string> mn(n),wn(n);
    map<string,int> mi,wi;
    vector<vector<int>> mp(n,vector<int>(n));
    vector<vector<int>> wr(n,vector<int>(n));
    for(int i=0;i<n;i++){cin>>mn[i];mi[mn[i]]=i;}
    vector<vector<string>> mps(n,vector<string>(n));
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)cin>>mps[i][j];
    for(int i=0;i<n;i++){cin>>wn[i];wi[wn[i]]=i;}
    vector<vector<string>> wps(n,vector<string>(n));
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)cin>>wps[i][j];
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)mp[i][j]=wi[mps[i][j]];
    for(int j=0;j<n;j++)for(int k=0;k<n;k++)wr[j][mi[wps[j][k]]]=k;
    vector<int> wp(n,-1),mpart(n,-1),np(n,0);
    queue<int> free;
    for(int i=0;i<n;i++)free.push(i);
    while(!free.empty()){
        int m=free.front();free.pop();
        int w=mp[m][np[m]++];
        if(wp[w]==-1){wp[w]=m;mpart[m]=w;}
        else{int m2=wp[w];if(wr[w][m]<wr[w][m2]){wp[w]=m;mpart[m]=w;mpart[m2]=-1;free.push(m2);}else free.push(m);}
    }
    cout<<"YES\n";
    for(int i=0;i<n;i++)cout<<mn[i]<<" "<<wn[mpart[i]]<<"\n";
}
