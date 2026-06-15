#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    map<long long, long long> inventory; // price -> count
    
    string query;
    while(cin >> query) {
        if(query == "ARRIVE") {
            long long n, c;
            cin >> n >> c;
            inventory[c] += n;
        }
        else { // BUY
            long long n, t;
            cin >> n >> t;
            
            long long cost = 0;
            long long count = 0;
            vector<pair<long long, long long>> toBuy; // price, quantity
            
            // Try to buy n cheapest pieces
            for(auto& [price, avail] : inventory) {
                long long needed = n - count;
                long long take = min(needed, avail);
                cost += take * price;
                toBuy.push_back({price, take});
                count += take;
                if(count == n) break;
            }
            
            if(count == n && cost <= t) {
                // Purchase successful
                cout << "HAPPY\n";
                // Remove the bought items
                for(auto [price, qty] : toBuy) {
                    inventory[price] -= qty;
                    if(inventory[price] == 0) {
                        inventory.erase(price);
                    }
                }
            }
            else {
                // Purchase failed
                cout << "UNHAPPY\n";
            }
        }
    }
    
    return 0;
}
