#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    long long d;
    cin >> n >> d;
    
    vector<pair<long long, long long>> banks(n); // {position, money}
    vector<int> indices(n); // original indices
    
    for(int i = 0; i < n; i++){
        cin >> banks[i].first >> banks[i].second;
        indices[i] = i + 1; // 1-indexed
    }
    
    long long maxSum = -1;
    int bank1 = -1, bank2 = -1;
    
    // For each bank j, find the best bank i where distance >= d
    int left = 0; // pointer to track valid left banks
    long long maxMoneyLeft = -1;
    int maxMoneyLeftIdx = -1;
    
    for(int j = 0; j < n; j++){
        // Move left pointer to include all banks that are at distance >= d from bank j
        // We need: banks[j].first - banks[i].first >= d
        // Which means: banks[i].first <= banks[j].first - d
        
        long long threshold = banks[j].first - d;
        
        // Include all banks with position <= threshold into our "best left" tracking
        while(left < j && banks[left].first <= threshold){
            if(maxMoneyLeft < 0 || banks[left].second > maxMoneyLeft){
                maxMoneyLeft = banks[left].second;
                maxMoneyLeftIdx = left;
            }
            left++;
        }
        
        // Check if we have any valid left bank
        if(maxMoneyLeft >= 0){
            long long sum = maxMoneyLeft + banks[j].second;
            if(sum > maxSum){
                maxSum = sum;
                bank1 = maxMoneyLeftIdx + 1; // convert to 1-indexed
                bank2 = j + 1;
            }
        }
    }
    
    if(bank1 == -1){
        cout << "-1 -1\n";
    } else {
        cout << bank1 << " " << bank2 << "\n";
    }
    
    return 0;
}
