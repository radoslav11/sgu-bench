#include<bits/stdc++.h>
using namespace std;

int N, ans;
int cnt[4];
vector<int> tri; // tri[i] = type of triangle at position i

int getEdge(int triType, int orient, int edge) {
    // Returns color of edge for given triangle type and orientation
    int base[4][3] = {{0,0,0}, {0,0,1}, {0,1,0}, {1,1,0}};
    return (base[triType][((edge - orient) % 3 + 3) % 3]);
}

bool checkValid(int pos) {
    // Check if placement at pos is valid
    // Would need proper adjacency mapping for the triangle grid
    return true;
}

void backtrack(int pos) {
    if(pos == N*N) {
        ans++;
        return;
    }
    
    for(int t = 0; t < 4; t++) {
        if(cnt[t] > 0) {
            for(int orient = 0; orient < 3; orient++) {
                tri[pos] = (t << 2) | orient;
                if(checkValid(pos)) {
                    cnt[t]--;
                    backtrack(pos+1);
                    cnt[t]++;
                }
            }
        }
    }
}

int main(){
    cin >> N;
    for(int i = 0; i < 4; i++) cin >> cnt[i];
    
    tri.assign(N*N, 0);
    ans = 0;
    
    backtrack(0);
    cout << ans << endl;
    
    return 0;
}
