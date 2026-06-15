#include<bits/stdc++.h>
using namespace std;

int N;
int avail[4];
long long ans=0;
int ec[200];
vector<array<int,3>> tris;

void solve(int t, int used[4]){
    if(t == N*N){ ans++; return; }
    auto& tri = tris[t];
    int e0=tri[0], e1=tri[1], e2=tri[2];
    int a0=ec[e0], a1=ec[e1], a2=ec[e2];
    for(int c0=0;c0<2;c0++){
        if(a0!=-1&&a0!=c0)continue;
        for(int c1=0;c1<2;c1++){
            if(a1!=-1&&a1!=c1)continue;
            for(int c2=0;c2<2;c2++){
                if(a2!=-1&&a2!=c2)continue;
                int bl=c0+c1+c2;
                if(used[bl]<=0)continue;
                bool s0=(a0==-1),s1=(a1==-1),s2=(a2==-1);
                if(s0)ec[e0]=c0;
                if(s1)ec[e1]=c1;
                if(s2)ec[e2]=c2;
                used[bl]--;
                solve(t+1,used);
                used[bl]++;
                if(s0)ec[e0]=-1;
                if(s1)ec[e1]=-1;
                if(s2)ec[e2]=-1;
            }
        }
    }
}

int main(){
    cin>>N;
    for(int i=0;i<4;i++) cin>>avail[i];
    int H=N*(N+1)/2;
    auto hidx=[&](int r,int c){return r*(r-1)/2+c;};
    auto lidx=[&](int r,int c){return H+r*(r+1)/2+c;};
    auto ridx=[&](int r,int c){return 2*H+r*(r+1)/2+c;};
    for(int r=1;r<=N;r++){
        for(int j=0;j<r;j++)
            tris.push_back({lidx(r-1,j),ridx(r-1,j),hidx(r,j)});
        for(int j=0;j<r-1;j++)
            tris.push_back({hidx(r-1,j),ridx(r-1,j),lidx(r-1,j+1)});
    }
    fill(ec,ec+3*H,-1);
    int used[4]={avail[0],avail[1],avail[2],avail[3]};
    solve(0,used);
    cout<<ans<<endl;
}
