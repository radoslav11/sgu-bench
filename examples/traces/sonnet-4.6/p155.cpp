#include <bits/stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(false);cin.tie(NULL);
    int n;cin>>n;
    vector<int>k(n),a(n);
    for(int i=0;i<n;i++)cin>>k[i]>>a[i];
    vector<int>idx(n);iota(idx.begin(),idx.end(),0);
    sort(idx.begin(),idx.end(),[&](int x,int y){return k[x]<k[y];});
    vector<int>par(n,0),lc(n,0),rc(n,0);
    stack<int>st;
    for(int i=0;i<n;i++){
        int cur=idx[i],last=-1;
        while(!st.empty()&&a[st.top()]>a[cur]){last=st.top();st.pop();}
        if(last!=-1){lc[cur]=last+1;par[last]=cur+1;}
        if(!st.empty()){int top=st.top();rc[top]=cur+1;par[cur]=top+1;}
        st.push(cur);
    }
    cout<<"YES\n";
    for(int i=0;i<n;i++)cout<<par[i]<<" "<<lc[i]<<" "<<rc[i]<<"\n";
}
