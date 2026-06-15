#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<pair<long long, long long>> c(n);
    
    for(int i = 0; i < n; i++) cin >> c[i].first >> c[i].second;
    
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int i, int j){
        return c[i] < c[j];
    });
    
    vector<int> len(n, 1), cnt(n, 1);
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < i; j++){
            int u = ord[j], v = ord[i];
            if(c[u].first < c[v].first && c[u].second < c[v].second){
                if(len[j] + 1 > len[i]){
                    len[i] = len[j] + 1;
                    cnt[i] = cnt[j];
                } else if(len[j] + 1 == len[i]){
                    cnt[i] += cnt[j];
                }
            }
        }
    }
    
    int mx = *max_element(len.begin(), len.end());
    long long total = 0;
    for(int i = 0; i < n; i++){
        if(len[i] == mx) total += cnt[i];
    }
    
    set<int> inA, inB;
    function<void(int, int, long long&)> dfs = [&](int u, int d, long long& cnt_path){
        if(d == 0){
            cnt_path++;
            return;
        }
        for(int j = 0; j < n; j++){
            if(c[j].first < c[u].first || c[j].second < c[u].second) continue;
            if(len[j] == d && c[j].first > c[u].first && c[j].second > c[u].second){
                long long tmp = 0;
                dfs(j, d-1, tmp);
                cnt_path += tmp;
            }
        }
    };
    
    // Simplified: list A are all cities in longest paths
    for(int i = 0; i < n; i++){
        if(len[i] == mx) inA.insert(i);
    }
    
    // For list B - cities with count equal to total
    for(int i = 0; i < n; i++){
        if(len[i] == mx && cnt[i] == total){
            inB.insert(i);
        }
    }
    
    vector<int> a(inA.begin(), inA.end());
    vector<int> b(inB.begin(), inB.end());
    
    cout << a.size();
    for(int x : a) cout << " " << x+1;
    cout << "\n" << b.size();
    for(int x : b) cout << " " << x+1;
    cout << "\n";
    
    return 0;
}
