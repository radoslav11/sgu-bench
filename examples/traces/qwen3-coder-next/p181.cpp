#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int main() {
    long long A, alpha, beta, gamma, M, k;
    cin >> A >> alpha >> beta >> gamma >> M >> k;
    
    // f(x) = (alpha * x^2 + beta * x + gamma) mod M
    auto f = [&](long long x) -> long long {
        return (alpha * x * x + beta * x + gamma) % M;
    };
    
    // Floyd's cycle detection to find the cycle start and length
    long long x = A;
    long long y = A;
    long long cycle_start = -1;
    long long cycle_length = 0;
    
    // Phase 1: Find the intersection point in the cycle
    do {
        x = f(x);           // tortoise moves 1 step
        y = f(f(y));        // hare moves 2 steps
    } while (x != y);
    
    // Phase 2: Find the start of the cycle
    x = A;
    while (x != y) {
        x = f(x);
        y = f(y);
        cycle_start++;
    }
    
    // Now x == y at the start of the cycle
    // Phase 3: Find the length of the cycle
    long long start_of_cycle = x;
    y = f(x);
    cycle_length = 1;
    while (x != y) {
        y = f(y);
        cycle_length++;
    }
    
    // Now determine x_k
    if (k <= cycle_start) {
        // Before the cycle
        long long cur = A;
        for (int i = 0; i < k; i++) {
            cur = f(cur);
        }
        cout << cur << endl;
    } else {
        // Inside the cycle
        long long offset_in_cycle = (k - cycle_start) % cycle_length;
        long long cur = start_of_cycle;
        for (int i = 0; i < offset_in_cycle; i++) {
            cur = f(cur);
        }
        cout << cur << endl;
    }
    
    return 0;
}
