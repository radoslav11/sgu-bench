#include <bits/stdc++.h>
using namespace std;
int main(){
    long long N,H;
    cin>>N>>H;
    // level r from top (0-indexed), pumpkins r^2+1..(r+1)^2
    // these pumpkins sit at y = N-1-r from bottom? Let's check:
    // N=5,H=0: y=0, r=N-1-H=4, pumpkins 17..25 - wrong
    // Try: r = N-1-H, but maybe height of level r is (N-1)-r
    // pumpkin 1 is alone at top, height = N-1 from bottom
    // Actually height of pumpkin at level r = (N-1) - r
    // N=5,H=0: (N-1)-r=0 => r=4, pumpkins 17..25 - still wrong
    // Let me re-examine: at N=5,H=0: p1,p2,p3,p5 are there - 4 pumpkins
    // r=3: 10..16? No. 
    // Hmm level r has 2r+1 pumpkins. r=0:1, r=1:3, r=2:5...
    // at H=0 we have 4 pumpkins which doesn't fit 2r+1.
    // So N=5 is incomplete last level.
    long long r=N-1-H;
    if(r<0){cout<<endl;return 0;}
    long long start=r*r+1, end=min((r+1)*(r+1),N);
    if(start>N){return 0;}
    // positions: center at 0, level r has 2r+1 spots from -r to r
    // pumpkins fill center first, then alternating left-right
    // order in output: leftmost to rightmost
    // pumpkin start+k fills position based on k
    // For k=0: pos 0; k=1: pos -1; k=2: pos +1; k=3: pos -2; k=4: pos +2
    vector<pair<long long,long long>> res;
    for(long long i=start;i<=end;i++){
        long long k=i-start;
        long long pos;
        if(k==0) pos=0;
        else if(k%2==1) pos=-(k+1)/2;
        else pos=k/2;
        res.push_back({pos,i});
    }
    sort(res.begin(),res.end());
    for(int i=0;i<res.size();i++){
        if(i) cout<<" ";
        cout<<res[i].second;
    }
    cout<<endl;
}
