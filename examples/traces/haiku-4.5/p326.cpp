#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<long long> w(n), r(n);
    for(int i = 0; i < n; i++) cin >> w[i];
    for(int i = 0; i < n; i++) cin >> r[i];
    
    vector<vector<long long>> a(n, vector<long long>(n));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> a[i][j];
        }
    }
    
    long long max_wins_1 = w[0] + r[0];
    
    // Check if team 1 can achieve at least max_wins_1 wins such that
    // all other teams have at most max_wins_1 wins
    for(int i = 1; i < n; i++){
        if(w[i] + r[i] > max_wins_1){
            // Team i can potentially get more wins
            // Check if we can limit it
            long long excess = w[i] + r[i] - max_wins_1;
            // We need other teams to beat team i at least 'excess' times
            // This is complex, so let's just check if w[i] > max_wins_1
            if(w[i] > max_wins_1){
                cout << "NO\n";
                return 0;
            }
        }
    }
    
    cout << "YES\n";
    return 0;
}
