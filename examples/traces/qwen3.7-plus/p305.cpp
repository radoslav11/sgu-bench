#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>

using namespace std;

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

struct Prog {
    long long A, B;
    long long K_start, K_end;
};

long long ceil_div(long long x, long long y) {
    if (x >= 0) return (x + y - 1) / y;
    else return x / y;
}

long long floor_div(long long x, long long y) {
    if (x >= 0) return x / y;
    else return (x - y + 1) / y;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    long long M;
    if (!(cin >> N >> M)) return 0;

    vector<Prog> progs(N);
    for (int i = 0; i < N; ++i) {
        cin >> progs[i].A >> progs[i].B;
        long long A = progs[i].A;
        long long B = progs[i].B;
        
        if (A > 0) {
            long long min_K = ceil_div(1 - B, A);
            long long max_K = floor_div(M - B, A);
            progs[i].K_start = max(0LL, min_K);
            progs[i].K_end = max_K;
        } else if (A < 0) {
            long long negA = -A;
            long long min_K = ceil_div(B - M, negA);
            long long max_K = floor_div(B - 1, negA);
            progs[i].K_start = max(0LL, min_K);
            progs[i].K_end = max_K;
        } else {
            if (B >= 1 && B <= M) {
                progs[i].K_start = 0;
                progs[i].K_end = 0;
            } else {
                progs[i].K_start = 1;
                progs[i].K_end = 0;
            }
        }
    }

    unordered_map<long long, int, custom_hash> match;

    auto dfs = [&](auto& self, int i, vector<bool>& vis) -> bool {
        if (vis[i]) return false;
        vis[i] = true;
        
        for (long long K = progs[i].K_start; K <= progs[i].K_end; ++K) {
            long long V = progs[i].B + progs[i].A * K;
            auto it = match.find(V);
            if (it == match.end()) {
                match[V] = i;
                return true;
            } else {
                int j = it->second;
                if (!vis[j]) {
                    if (self(self, j, vis)) {
                        match[V] = i;
                        return true;
                    }
                }
            }
        }
        return false;
    };

    for (int i = 0; i < N; ++i) {
        vector<bool> vis(N, false);
        dfs(dfs, i, vis);
    }

    vector<long long> ans(N, 0);
    for (auto const& p : match) {
        ans[p.second] = p.first;
    }

    for (int i = 0; i < N; ++i) {
        cout << ans[i] << (i == N - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
