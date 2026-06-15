#include <bits/stdc++.h>
using namespace std;
int main(){
    int h,w; cin>>h>>w;
    vector<string> g(h); for(auto&s:g)cin>>s;
    vector<vector<int>> rp(h,vector<int>(w,0)),cp(h,vector<int>(w,0));
    for(int r=0;r<h;r++){int c=0;while(c<w){int k=1;while(c-k>=0&&c+k<w&&g[r][c-k]==g[r][c+k])k++;rp[r][c]=k-1;c++;}}
    for(int c=0;c<w;c++){int r=0;while(r<h){int k=1;while(r-k>=0&&r+k<h&&g[r-k][c]==g[r+k][c])k++;cp[r][c]=k-1;r++;}}
    // prefix min of rp along rows for fixed cc, and cp along cols for fixed cr
    // mrp[cc][r] = rp[r][cc], build prefix min in both directions from each center
    // For center (cr,cc), max k where all rows in [cr-k,cr+k] have rp[row][cc]>=k
    // Use: for fixed cc, array A[r]=rp[r][cc]. For each cr, find max k<=min(cr,h-1-cr) s.t. min(A[cr-k..cr+k])>=k
    // Precompute for fixed cc: left_min[r] = min of A[r..cr] going up, but k-dependent - hard
    // Just do O(h*w*min(h,w)) naively
    int br=0,bc=0,bsize=1;
    for(int cr=0;cr<h;cr++)for(int cc=0;cc<w;cc++){
        int maxk=min({rp[cr][cc],cp[cr][cc],cr,h-1-cr,cc,w-1-cc});
        for(int k=maxk;k>=bsize/2;k--){ // only if 2k+1>bsize
            if(2*k+1<=bsize)break;
            bool ok=true;
            for(int dr=1;dr<=k&&ok;dr++)if(rp[cr-dr][cc]<k||rp[cr+dr][cc]<k)ok=false;
            for(int dc=1;dc<=k&&ok;dc++)if(cp[cr][cc-dc]<k||cp[cr][cc+dc]<k)ok=false;
            if(ok){bsize=2*k+1;br=cr-k;bc=cc-k;break;}
        }
    }
    cout<<br+1<<" "<<bc+1<<" "<<br+bsize<<" "<<bc+bsize<<endl;
}
