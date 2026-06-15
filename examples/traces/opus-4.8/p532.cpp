#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d",&n);
    // horizontals: y, x1, x2 ; verticals: x, y1, y2
    vector<array<long long,3>> H; // y, xmin, xmax
    vector<array<long long,3>> V; // x, ymin, ymax
    for(int i=0;i<n;i++){
        long long x1,y1,x2,y2;
        scanf("%lld %lld %lld %lld",&x1,&y1,&x2,&y2);
        if(y1==y2){
            // horizontal
            H.push_back({y1, min(x1,x2), max(x1,x2)});
        } else {
            // vertical (x1==x2)
            V.push_back({x1, min(y1,y2), max(y1,y2)});
        }
    }
    int hn = H.size();
    int vn = V.size();
    if(hn<2 || vn<2){ printf("0\n"); return 0; }
    
    int words = (hn+63)/64;
    // for each vertical, bitset of horizontals it intersects
    vector<vector<uint64_t>> bs(vn, vector<uint64_t>(words,0));
    
    for(int i=0;i<vn;i++){
        long long vx=V[i][0], vy1=V[i][1], vy2=V[i][2];
        for(int j=0;j<hn;j++){
            long long hy=H[j][0], hx1=H[j][1], hx2=H[j][2];
            if(hx1<=vx && vx<=hx2 && vy1<=hy && hy<=vy2){
                bs[i][j>>6] |= (1ULL<<(j&63));
            }
        }
    }
    
    long long ans=0;
    for(int i=0;i<vn;i++){
        for(int j=i+1;j<vn;j++){
            long long c=0;
            for(int w=0;w<words;w++){
                c += __builtin_popcountll(bs[i][w] & bs[j][w]);
            }
            ans += c*(c-1)/2;
        }
    }
    printf("%lld\n",ans);
    return 0;
}
