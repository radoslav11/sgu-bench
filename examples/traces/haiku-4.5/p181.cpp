#include<bits/stdc++.h>
using namespace std;

int main(){
    long long A, alpha, beta, gamma, M, k;
    cin >> A >> alpha >> beta >> gamma >> M >> k;
    
    map<long long, long long> seen; // value -> index when first seen
    vector<long long> sequence;
    
    long long current = A;
    long long index = 0;
    
    // Build sequence until we find a cycle or reach k
    while(index <= k && seen.find(current) == seen.end()){
        seen[current] = index;
        sequence.push_back(current);
        
        if(index == k){
            cout << current << endl;
            return 0;
        }
        
        // Compute next term: (alpha * current^2 + beta * current + gamma) mod M
        long long next = ((alpha * (current * current % M)) % M + 
                         (beta * current) % M + 
                         gamma) % M;
        current = next;
        index++;
    }
    
    // If we're here, we found a cycle
    if(index <= k && seen.find(current) != seen.end()){
        // We found a cycle
        long long cycle_start = seen[current];
        long long cycle_length = index - cycle_start;
        
        // k is beyond where we've computed, but within the cycle
        long long position_in_cycle = (k - cycle_start) % cycle_length;
        cout << sequence[cycle_start + position_in_cycle] << endl;
    }
    
    return 0;
}
