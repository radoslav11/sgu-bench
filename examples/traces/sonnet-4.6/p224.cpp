#include <bits/stdc++.h>
using namespace std;

int n, k;
long long ans = 0;
bool row_used[10], col_used[10], diag1_used[20], diag2_used[20];

void solve(int placed, int start_pos) {
    if (placed == k) { ans++; return; }
    int remaining = n * n - start_pos;
    if (remaining < k - placed) return;
    for (int pos = start_pos; pos < n * n; pos++) {
        int r = pos / n, c = pos % n;
        if (!row_used[r] && !col_used[c] && !diag1_used[r-c+n-1] && !diag2_used[r+c]) {
            row_used[r]=col_used[c]=diag1_used[r-c+n-1]=diag2_used[r+c]=true;
            solve(placed+1, pos+1);
            row_used[r]=col_used[c]=diag1_used[r-c+n-1]=diag2_used[r+c]=false;
        }
    }
}

int main(){
    string line;
    while(getline(cin,line)){
        istringstream iss(line);
        if(iss>>n>>k){
            memset(row_used,0,sizeof(row_used));memset(col_used,0,sizeof(col_used));
            memset(diag1_used,0,sizeof(diag1_used));memset(diag2_used,0,sizeof(diag2_used));
            ans=0; solve(0,0); cout<<ans<<"\n";
        }
    }
}
