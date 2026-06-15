#include <bits/stdc++.h>
using namespace std;

int main(){
    long long X,Y,Z;
    int cs=0;
    string line;
    while(getline(cin,line)){
        if(line.find_first_not_of(" \t\r\n")==string::npos) continue;
        stringstream ss(line);
        if(!(ss>>X>>Y>>Z)) continue;
        cs++;
        long long dims[3]={X,Y,Z};
        // total edges
        long long E = (X-1)*Y*Z + X*(Y-1)*Z + X*Y*(Z-1);
        // count odd-degree vertices:
        // a vertex is odd iff number of active dims (size>1) in which it is at boundary is odd
        // For each active dim, "at boundary" gives 2 choices (the two faces) contributing parity 1,
        // "not boundary" gives (size-2) choices contributing parity 0.
        // We need to count combinations over the three dims where total boundary-count is odd.
        // For each dim: ways to be at boundary = (size>1?2:0... but if size==1 it's neither, contributes 0)
        //   boundaryWays b_i, interiorWays i_i.
        //   size==1: this coordinate is fixed (1 cell), at boundary? It doesn't contribute parity (not active).
        //   treat as: contributes parity 0 always, count of cells =1.
        long long bw[3], iw[3];
        for(int d=0;d<3;d++){
            long long s=dims[d];
            if(s==1){ bw[d]=0; iw[d]=1; } // not active: 1 cell, parity-0
            else { bw[d]=2; iw[d]=s-2; }
        }
        // count vertices with odd number of "boundary" picks among active dims
        // enumerate parity using inclusion: number of cells with odd sum =
        //  total/2 - product(iw - bw? ) ... use formula:
        // odd = ( prod(iw+bw) - prod(iw-bw) ) / 2
        long long prodSum=1, prodDiff=1;
        for(int d=0;d<3;d++){ prodSum*=(iw[d]+bw[d]); prodDiff*=(iw[d]-bw[d]); }
        long long odd = (prodSum - prodDiff)/2;
        
        long long ans;
        if(odd==0) ans=E;
        else ans = E + odd/2 - 1;
        
        cout<<"Case #"<<cs<<": "<<ans<<"\n";
    }
    return 0;
}
