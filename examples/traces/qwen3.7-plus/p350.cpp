#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int N;
vector<long long> B;
vector<long long> A;

bool is_linearly_independent(const vector<long long>& vec) {
    vector<long long> basis;
    int non_zero_count = 0;
    for (long long x : vec) {
        if (x == 0) continue;
        non_zero_count++;
        long long v = x;
        for (long long b : basis) {
            if ((v ^ b) < v) {
                v ^= b;
            }
        }
        if (v == 0) return false;
        basis.push_back(v);
        sort(basis.rbegin(), basis.rend());
    }
    return basis.size() == (size_t)non_zero_count;
}

bool dfs(int depth) {
    if (depth == N) {
        if (is_linearly_independent(A)) {
            return true;
        }
        return false;
    }
    
    for (long long x : B) {
        if (!A.empty() && x <= A.back()) continue;
        
        bool ok = true;
        for (long long y : A) {
            if (!binary_search(B.begin(), B.end(), x ^ y)) {
                ok = false;
                break;
            }
        }
        
        if (ok) {
            A.push_back(x);
            if (dfs(depth + 1)) return true;
            A.pop_back();
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int M;
    if (!(cin >> M)) return 0;
    
    vector<long long> raw_B(M);
    for (int i = 0; i < M; ++i) {
        cin >> raw_B[i];
    }
    
    B = raw_B;
    sort(B.begin(), B.end());
    B.erase(unique(B.begin(), B.end()), B.end());
    
    int unique_M = B.size();
    N = -1;
    for (int i = 2; i <= 1000; ++i) {
        if (i * (i - 1) / 2 == unique_M) {
            N = i;
            break;
        }
    }
    if (N == -1) {
        N = (1 + sqrt(1 + 8 * unique_M)) / 2;
    }
    
    A.push_back(0);
    
    if (dfs(1)) {
        for (int i = 0; i < N; ++i) {
            cout << A[i] << (i == N - 1 ? "" : " ");
        }
        cout << "\n";
    } else {
        cout << "No solution\n";
    }
    
    return 0;
}
