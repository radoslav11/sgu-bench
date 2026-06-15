#include <iostream>
#include <numeric>

using namespace std;

long long ans = 0;
long long P;
long long M;
int N;
int cnt_arr[55];
long long gcd_table[55][55];

long long power(long long base, long long exp) {
    long long res = 1;
    base %= P;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % P;
        base = (base * base) % P;
        exp /= 2;
    }
    return res;
}

void dfs(int rem_N, int max_c, long long current_O, long long current_denom) {
    if (rem_N == 0) {
        long long term = (power(M, current_O) * power(current_denom, P - 2)) % P;
        ans = (ans + term) % P;
        return;
    }
    for (int c = 1; c <= max_c; ++c) {
        if (rem_N < c) continue;
        
        long long delta_O = c / 2;
        for (int i = 1; i <= N; ++i) {
            if (cnt_arr[i] > 0) {
                delta_O += 1LL * cnt_arr[i] * gcd_table[c][i];
            }
        }
        
        cnt_arr[c]++;
        long long new_denom = (current_denom * c) % P;
        new_denom = (new_denom * cnt_arr[c]) % P;
        
        dfs(rem_N - c, c, current_O + delta_O, new_denom);
        
        cnt_arr[c]--;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> N >> M >> P)) return 0;
    
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            gcd_table[i][j] = std::gcd(i, j);
        }
    }
    
    dfs(N, N, 0, 1);
    
    cout << ans << "\n";
    
    return 0;
}
