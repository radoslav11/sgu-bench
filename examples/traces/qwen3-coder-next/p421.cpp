#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <set>
#include <complex>

using namespace std;

struct Product {
    long double log_sum;
    int sign;
    vector<int> indices;
    
    Product(long double ls, int s, vector<int> idx) 
        : log_sum(ls), sign(s), indices(idx) {}
};

struct Compare {
    bool operator()(const Product& p1, const Product& p2) const {
        // For priority_queue (max-heap), return true if p1 has lower priority than p2
        if (p1.sign != p2.sign) {
            return p1.sign < p2.sign; // 1 > 0 > -1
        }
        if (p1.sign == 0) {
            return false; // equal
        }
        if (p1.sign == 1) {
            return p1.log_sum < p2.log_sum;
        } else {
            return p1.log_sum > p2.log_sum;
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    long long k;
    cin >> n >> m >> k;
    
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    // Precompute log of absolute values
    vector<long double> log_abs(n);
    for (int i = 0; i < n; i++) {
        if (a[i] == 0) {
            log_abs[i] = -1e300;
        } else {
            log_abs[i] = logl((long double)abs(a[i]));
        }
    }
    
    priority_queue<Product, vector<Product>, Compare> pq;
    set<vector<int>> seen;
    
    // Start with the combination of the last m elements (largest by value)
    vector<int> start(m);
    for (int i = 0; i < m; i++) {
        start[i] = n - m + i;
    }
    
    long double log_sum = 0;
    int sign = 1;
    for (int i = 0; i < m; i++) {
        if (a[start[i]] == 0) {
            sign = 0;
            log_sum = -1e300;
            break;
        }
        if (a[start[i]] < 0) {
            sign *= -1;
        }
        log_sum += log_abs[start[i]];
    }
    pq.emplace(log_sum, sign, start);
    seen.insert(start);
    
    Product result;
    for (long long i = 1; i <= k; i++) {
        result = pq.top();
        pq.pop();
        
        // Generate next combinations
        vector<int> indices = result.indices;
        for (int pos = m - 1; pos >= 0; pos--) {
            int next_min = (pos > 0) ? indices[pos-1] + 1 : 0;
            if (indices[pos] > next_min) {
                vector<int> new_indices = indices;
                new_indices[pos]--;
                for (int j = pos + 1; j < m; j++) {
                    new_indices[j] = new_indices[j-1] + 1;
                }
                if (seen.find(new_indices) == seen.end()) {
                    long double new_log_sum = 0;
                    int new_sign = 1;
                    for (int j = 0; j < m; j++) {
                        if (a[new_indices[j]] == 0) {
                            new_sign = 0;
                            new_log_sum = -1e300;
                            break;
                        }
                        if (a[new_indices[j]] < 0) {
                            new_sign *= -1;
                        }
                        new_log_sum += log_abs[new_indices[j]];
                    }
                    pq.emplace(new_log_sum, new_sign, new_indices);
                    seen.insert(new_indices);
                }
            }
        }
    }
    
    // Calculate actual product
    long long product = 1;
    for (int idx : result.indices) {
        product *= a[idx];
    }
    cout << product << '\n';
    
    return 0;
}
