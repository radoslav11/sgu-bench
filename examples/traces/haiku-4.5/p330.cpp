#include<bits/stdc++.h>
using namespace std;

vector<long long> getDivisors(long long n) {
    vector<long long> divs;
    for(long long i = 2; i * i <= n; i++) {
        if(n % i == 0) {
            divs.push_back(i);
            if(i != n/i) {
                divs.push_back(n/i);
            }
        }
    }
    return divs;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long A, B;
    cin >> A >> B;
    
    map<long long, long long> parent;
    queue<long long> q;
    
    q.push(A);
    parent[A] = -1;
    
    bool found = false;
    
    while(!q.empty() && !found) {
        long long curr = q.front();
        q.pop();
        
        if(curr == B) {
            found = true;
            break;
        }
        
        if(curr > 2*B) continue;
        
        vector<long long> divs = getDivisors(curr);
        
        for(long long d : divs) {
            long long next = curr + d;
            
            if(next > 2*B) continue;
            if(parent.count(next)) continue;
            
            parent[next] = curr;
            q.push(next);
            
            if(next == B) {
                found = true;
                break;
            }
        }
    }
    
    if(!found) {
        cout << "Impossible\n";
    } else {
        vector<long long> path;
        long long curr = B;
        while(curr != -1) {
            path.push_back(curr);
            curr = parent[curr];
        }
        reverse(path.begin(), path.end());
        for(long long x : path) {
            cout << x << "\n";
        }
    }
    
    return 0;
}
