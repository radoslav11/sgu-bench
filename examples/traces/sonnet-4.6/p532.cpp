#include <bits/stdc++.h>
using namespace std;
bool covered(vector<pair<int,int>>& segs, int a, int b){
    int cur=a;
    vector<pair<int,int>> v;
    for(auto& s:segs) if(s.second>a&&s.first<b) v.push_back({max(s.first,a),min(s.second,b)});
    sort(v.begin(),v.end());
    for(auto& p:v){if(p.first>cur)return false;cur=max(cur,p.second);}
    return cur>=b;
}
int main(){
    int n; cin>>n;
    map<int,vector<pair<int,int>>> H,V;
    for(int i=0;i<n;i++){
        int x1,y1,x2,y2; cin>>x1>>y1>>x2>>y2;
        if(y1==y2){if(x1>x2)swap(x1,x2);H[y1].push_back({x1,x2});}
        else{if(y1>y2)swap(y1,y2);V[x1].push_back({y1,y2});}
    }
    vector<int> ys,xs;
    for(auto& p:H)ys.push_back(p.first);
    for(auto& p:V)xs.push_back(p.first);
    sort(ys.begin(),ys.end());sort(xs.begin(),xs.end());
    long long ans=0;
    for(int i=0;i<(int)ys.size();i++)for(int j=i+1;j<(int)ys.size();j++){
        int y1=ys[i],y2=ys[j];
        vector<int> vx;
        for(auto& p:V){int x=p.first;auto& s=p.second;if(covered(s,y1,y2))vx.push_back(x);}
        for(int a=0;a<(int)vx.size();a++)for(int b=a+1;b<(int)vx.size();b++){
            int x1=vx[a],x2=vx[b];
            if(covered(H[y1],x1,x2)&&covered(H[y2],x1,x2))ans++;
        }
    }
    cout<<ans<<endl;
}
