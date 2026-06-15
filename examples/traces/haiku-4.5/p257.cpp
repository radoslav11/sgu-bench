#include<bits/stdc++.h>
using namespace std;

int P, O, S, N;
vector<string> crystals;
string result;

bool solve(int idx, int p_debt, int o_debt, int s_debt) {
    // If we've assigned all crystals
    if (idx == N) {
        return p_debt <= 0 && o_debt <= 0 && s_debt <= 0;
    }
    
    string& crystal = crystals[idx];
    
    // Try assigning to Popov
    if (p_debt > 0) {
        int value = (crystal[0] == 'B') ? 2 : 1;
        result[idx] = 'P';
        if (solve(idx + 1, p_debt - value, o_debt, s_debt)) {
            return true;
        }
    }
    
    // Try assigning to Parkin
    if (o_debt > 0) {
        int value = (crystal[1] == 'B') ? 2 : 1;
        result[idx] = 'O';
        if (solve(idx + 1, p_debt, o_debt - value, s_debt)) {
            return true;
        }
    }
    
    // Try assigning to Studnev
    if (s_debt > 0) {
        int value = (crystal[2] == 'B') ? 2 : 1;
        result[idx] = 'S';
        if (solve(idx + 1, p_debt, o_debt, s_debt - value)) {
            return true;
        }
    }
    
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> P >> O >> S >> N;
    
    crystals.resize(N);
    result.resize(N);
    
    for (int i = 0; i < N; i++) {
        cin >> crystals[i];
    }
    
    if (solve(0, P, O, S)) {
        cout << result << endl;
    } else {
        cout << "no solution" << endl;
    }
    
    return 0;
}
