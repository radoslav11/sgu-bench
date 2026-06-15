#include <iostream>
#include <vector>

using namespace std;

long long L, R, K;

int sum_digits(long long n) {
    int s = 0;
    while (n > 0) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

struct Info {
    int new_B;
    int new_rem;
    long long added_p;
};

Info trans1[165][100][1005];
bool trans1_computed[165][100][1005];

Info get_trans1(int s, int B, int rem) {
    if (trans1_computed[s][B][rem]) {
        return trans1[s][B][rem];
    }
    long long p = 0;
    long long current_sum = 0;
    long long need = (rem == 0 ? K : rem);
    int curr_B = B;
    
    for (int step = 0; step < 100; ++step) {
        int t = curr_B + step;
        int val = sum_digits(t);
        current_sum += s + val;
        
        if (current_sum >= need) {
            p++;
            current_sum = 0;
            need = K;
        }
    }
    
    Info res;
    if (current_sum > 0) {
        res.new_B = 0;
        res.new_rem = K - current_sum;
        res.added_p = p;
    } else {
        res.new_B = 0;
        res.new_rem = 0;
        res.added_p = p;
    }
    
    trans1[s][B][rem] = res;
    trans1_computed[s][B][rem] = true;
    return res;
}

long long memo_p[165][100505];
int memo_state[165][100505];
bool visited[165][100505];

void precalc_jumps() {
    for (int i = 0; i < 165; ++i) {
        for (int j = 0; j < 100505; ++j) {
            visited[i][j] = false;
        }
    }
}

struct JumpRes {
    long long p;
    int state;
};

JumpRes jump_100k(int shift, int state) {
    if (visited[shift][state]) {
        return {memo_p[shift][state], memo_state[shift][state]};
    }
    
    int curr_state = state;
    long long total_p = 0;
    
    for (int i = 0; i < 100000; ++i) {
        int B = curr_state / 1005;
        int rem = curr_state % 1005;
        int s = shift + sum_digits(i);
        if (s > 162) s = 162; // safety
        
        Info inf = get_trans1(s, B, rem);
        total_p += inf.added_p;
        curr_state = inf.new_B * 1005 + inf.new_rem;
    }
    
    visited[shift][state] = true;
    memo_p[shift][state] = total_p;
    memo_state[shift][state] = curr_state;
    
    return {total_p, curr_state};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> L >> R >> K)) return 0;
    
    for (int s = 1; s <= 162; ++s) {
        for (int B = 0; B < 100; ++B) {
            for (int rem = 0; rem <= 1000; ++rem) {
                trans1[s][B][rem] = get_trans1(s, B, rem);
            }
        }
    }
    
    precalc_jumps();
    
    long long total_passengers = 0;
    long long current_l = L;
    int current_B = L % 100;
    int current_rem = 0;
    
    // Process tickets until a multiple of 100
    if (current_l % 100 != 0) {
        long long end_block = (current_l / 100 + 1) * 100;
        long long limit = min(R, end_block - 1);
        long long current_sum = 0;
        long long need = K;
        for (long long i = current_l; i <= limit; ++i) {
            current_sum += sum_digits(i);
            if (current_sum >= need) {
                total_passengers++;
                current_sum = 0;
                need = K;
            }
        }
        if (current_sum > 0) {
            current_rem = K - current_sum;
        } else {
            current_rem = 0;
        }
        current_l = end_block;
    }
    
    // Now current_l is a multiple of 100
    long long limit_100k = (current_l / 10000000) * 10000000; // align to 100k blocks of 100 (i.e. 10^7)
    if (limit_100k < current_l) {
        limit_100k += 10000000;
    }
    
    long long process_limit = min(R, limit_100k - 1);
    
    // Process block by block up to process_limit
    while (current_l <= process_limit && current_l <= R) {
        long long A = current_l / 100;
        int shift = sum_digits(A / 100000);
        int state = current_B * 1005 + current_rem;
        
        JumpRes res = jump_100k(shift, state);
        total_passengers += res.p;
        current_B = res.state / 1005;
        current_rem = res.state % 1005;
        
        current_l += 10000000;
    }
    
    // Process remaining blocks one by one
    while (current_l <= R) {
        long long A = current_l / 100;
        int s = sum_digits(A);
        Info inf = get_trans1(s, current_B, current_rem);
        total_passengers += inf.added_p;
        current_B = inf.new_B;
        current_rem = inf.new_rem;
        current_l += 100;
    }
    
    cout << total_passengers << "\n";
    
    return 0;
}
