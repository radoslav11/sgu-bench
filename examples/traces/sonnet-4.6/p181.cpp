#include <bits/stdc++.h>
using namespace std;

int main() {
    long long A, alpha, beta, gamma, M, k;
    cin >> A >> alpha >> beta >> gamma >> M >> k;
    
    vector<long long> seq;
    map<long long, long long> seen;
    
    long long cur = A;
    seq.push_back(cur);
    seen[cur] = 0;
    
    for (long long i = 1; i <= k; i++) {
        long long prev = seq.back();
        cur = (alpha * prev * prev + beta * prev + gamma) % M;
        
        if (seen.count(cur)) {
            long long cycle_start = seen[cur];
            long long cycle_len = i - cycle_start;
            long long pos = cycle_start + (k - cycle_start) % cycle_len;
            cout << seq[pos] << endl;
            return 0;
        }
        
        seq.push_back(cur);
        seen[cur] = i;
        
        if (i == k) {
            cout << cur << endl;
            return 0;
        }
    }
    
    cout << seq[k] << endl;
    return 0;
}
