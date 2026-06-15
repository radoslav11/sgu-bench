#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Min-heap: (price, quantity)
    priority_queue<pair<long long, long long>, 
                   vector<pair<long long, long long>>, 
                   greater<pair<long long, long long>>> pq;
    
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        string op;
        iss >> op;
        
        if (op == "ARRIVE") {
            long long n, c;
            iss >> n >> c;
            pq.emplace(c, n);
        } else if (op == "BUY") {
            long long n, t;
            iss >> n >> t;
            
            // First, check if we have enough pieces and if total cost <= t
            long long pieces_left = n;
            long long total_cost = 0;
            bool can_buy = true;
            
            // Create a copy of the queue to check
            vector<pair<long long, long long>> temp;
            while (pieces_left > 0 && !pq.empty()) {
                auto [price, quantity] = pq.top();
                pq.pop();
                
                long long take = min(pieces_left, quantity);
                if (total_cost + take * price > t) {
                    can_buy = false;
                    temp.emplace_back(price, quantity);
                    while (!pq.empty()) {
                        temp.push_back(pq.top());
                        pq.pop();
                    }
                    break;
                }
                
                total_cost += take * price;
                pieces_left -= take;
                
                if (quantity > take) {
                    temp.emplace_back(price, quantity - take);
                }
            }
            
            if (pieces_left > 0) {
                can_buy = false;
                while (!pq.empty()) {
                    temp.push_back(pq.top());
                    pq.pop();
                }
            }
            
            // Restore the original state
            for (auto& batch : temp) {
                pq.push(batch);
            }
            temp.clear();
            
            if (can_buy && total_cost <= t) {
                // Actually remove the pieces
                pieces_left = n;
                total_cost = 0;
                while (pieces_left > 0) {
                    auto [price, quantity] = pq.top();
                    pq.pop();
                    
                    long long take = min(pieces_left, quantity);
                    pieces_left -= take;
                    
                    if (quantity > take) {
                        pq.emplace(price, quantity - take);
                    }
                }
                cout << "HAPPY\n";
            } else {
                cout << "UNHAPPY\n";
            }
        }
    }
    
    return 0;
}
