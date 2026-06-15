#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m;
    cin>>n>>m;
    vector<int> score(n,0);
    for(int g=0;g<m;g++){
        int a,b;
        cin>>a>>b;
        for(int i=0;i<n;i++){
            int x,y;
            cin>>x>>y;
            int s=0;
            // winner or tie
            auto sign=[](int p,int q){return p>q?1:(p<q?-1:0);};
            if(sign(x,y)==sign(a,b)) s+=2;
            if(x-y==a-b) s+=3;
            if(x==a) s+=1;
            if(y==b) s+=1;
            score[i]+=s;
        }
    }
    for(int i=0;i<n;i++){
        cout<<score[i];
        if(i+1<n) cout<<" ";
    }
    cout<<endl;
}
