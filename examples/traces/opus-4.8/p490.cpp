#include <bits/stdc++.h>
using namespace std;

int main(){
    int T;
    scanf("%d",&T);
    bool first=true;
    for(int tc=0;tc<T;tc++){
        int H,W,N;
        scanf("%d %d %d",&H,&W,&N);
        if(!first) printf("\n");
        first=false;
        
        int h=H-2, w=W-2;
        long long maxHoles=0;
        if(h>=1 && w>=1) maxHoles=( (long long)h*w +1)/2;
        
        if(N<1 || N>maxHoles){
            printf("Impossible\n");
            continue;
        }
        
        // build grid all black '.'
        vector<string> grid(H, string(W,'.'));
        
        // collect interior cells of each parity
        vector<pair<int,int>> c0, c1;
        for(int r=1;r<=H-2;r++){
            for(int c=1;c<=W-2;c++){
                if(((r+c)&1)==0) c0.push_back({r,c});
                else c1.push_back({r,c});
            }
        }
        vector<pair<int,int>>& cand = (c0.size()>=c1.size())? c0 : c1;
        
        // carve N holes
        for(int i=0;i<N;i++){
            int r=cand[i].first, c=cand[i].second;
            grid[r][c]='#';
        }
        
        for(int r=0;r<H;r++){
            printf("%s\n", grid[r].c_str());
        }
    }
    return 0;
}
