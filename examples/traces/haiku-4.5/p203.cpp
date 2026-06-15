#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    priority_queue<long long, vector<long long>, greater<long long>> pq;
    
    for (int i = 0; i < n; i++) {
        long long p;
        cin >> p;
        pq.push(p);
    }
    
    long long totalCost = 0;
    
    while (pq.size() > 1) {
        long long first = pq.top();
        pq.pop();
        long long second = pq.top();
        pq.pop();
        
        long long merged = first + second;
        totalCost += merged;
        pq.push(merged);
    }
    
    cout << totalCost << endl;
    
    return 0;
}
