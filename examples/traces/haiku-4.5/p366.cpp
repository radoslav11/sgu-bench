#include<bits/stdc++.h>
using namespace std;

int N, K;
vector<pair<int,int>> opponents;
int best_diff = INT_MAX;
int best_sum = -1;
vector<int> best_selection;

void solve(int idx, int selected, int A, int B, vector<int>& current) {
    if (selected == K) {
        int diff = abs(A - B);
        int sum = A + B;
        if (diff < best_diff || (diff == best_diff && sum > best_sum)) {
            best_diff = diff;
            best_sum = sum;
            best_selection = current;
        }
        return;
    }
    
    if (idx == N) return;
    if (N - idx < K - selected) return;
    
    current.push_back(idx + 1);
    solve(idx + 1, selected + 1, A + opponents[idx].first, B + opponents[idx].second, current);
    current.pop_back();
    
    solve(idx + 1, selected, A, B, current);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> K;
    opponents.resize(N);
    
    for (int i = 0; i < N; i++) {
        cin >> opponents[i].first >> opponents[i].second;
    }
    
    vector<int> current;
    solve(0, 0, 0, 0, current);
    
    int A = 0, B = 0;
    for (int idx : best_selection) {
        A += opponents[idx-1].first;
        B += opponents[idx-1].second;
    }
    
    cout << A << " " << B << "\n";
    for (int i = 0; i < best_selection.size(); i++) {
        if (i > 0) cout << " ";
        cout << best_selection[i];
    }
    cout << "\n";
    
    return 0;
}
