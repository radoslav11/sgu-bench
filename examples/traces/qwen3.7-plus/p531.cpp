#include <iostream>
#include <vector>

using namespace std;

struct Bank {
    long long x;
    long long w;
    int id;
};

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    long long d;
    if (!(cin >> n >> d)) return 0;

    vector<Bank> banks(n);
    for (int i = 0; i < n; ++i) {
        cin >> banks[i].x >> banks[i].w;
        banks[i].id = i + 1; // 1-based indexing for output
    }

    long long max_sum = -1;
    int best_i = -1;
    int best_j = -1;

    long long current_max_w = -1;
    int current_best_i = -1;
    int ptr = 0;

    // Iterate through each bank as the right element of a pair
    for (int j = 0; j < n; ++j) {
        // Expand the window of valid left elements
        while (ptr < n && banks[ptr].x <= banks[j].x - d) {
            if (banks[ptr].w > current_max_w) {
                current_max_w = banks[ptr].w;
                current_best_i = banks[ptr].id;
            }
            ptr++;
        }
        
        // If there's at least one valid left element, check if we formed a new maximum sum
        if (current_best_i != -1) {
            long long current_sum = current_max_w + banks[j].w;
            if (current_sum > max_sum) {
                max_sum = current_sum;
                best_i = current_best_i;
                best_j = banks[j].id;
            }
        }
    }

    if (max_sum == -1) {
        cout << "-1 -1\n";
    } else {
        cout << best_i << " " << best_j << "\n";
    }

    return 0;
}
