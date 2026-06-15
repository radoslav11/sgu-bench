#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    deque<long long> q1, q2;
    
    for (int i = 0; i < n; i++) {
        long long x;
        cin >> x;
        q1.push_back(x);
    }
    
    long long total = 0;
    
    while (q1.size() + q2.size() > 1) {
        long long a, b;
        
        if (q2.empty() || (!q1.empty() && q1.front() <= q2.front())) {
            a = q1.front(); q1.pop_front();
        } else {
            a = q2.front(); q2.pop_front();
        }
        
        if (q2.empty() || (!q1.empty() && q1.front() <= q2.front())) {
            b = q1.front(); q1.pop_front();
        } else {
            b = q2.front(); q2.pop_front();
        }
        
        long long merged = a + b;
        total += merged;
        q2.push_back(merged);
    }
    
    cout << total << endl;
    
    return 0;
}
