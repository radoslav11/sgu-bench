#include<bits/stdc++.h>
using namespace std;

int n;
vector<int> a;
map<tuple<int,int,vector<int>>, int> memo;

int grundy(int left, int right, vector<int> heaps) {
    // Base case: no heaps left
    if (left > right) {
        return 0;
    }
    
    // Create state key
    auto state = make_tuple(left, right, heaps);
    if (memo.count(state)) {
        return memo[state];
    }
    
    set<int> reachable;
    
    // Try taking from left heap
    for (int take = 1; take <= heaps[left]; take++) {
        vector<int> newHeaps = heaps;
        newHeaps[left] -= take;
        
        int newLeft = left;
        while (newLeft <= right && newHeaps[newLeft] == 0) {
            newLeft++;
        }
        
        int g = grundy(newLeft, right, newHeaps);
        reachable.insert(g);
    }
    
    // Try taking from right heap
    for (int take = 1; take <= heaps[right]; take++) {
        vector<int> newHeaps = heaps;
        newHeaps[right] -= take;
        
        int newRight = right;
        while (newRight >= left && newHeaps[newRight] == 0) {
            newRight--;
        }
        
        int g = grundy(left, newRight, newHeaps);
        reachable.insert(g);
    }
    
    // Calculate MEX
    int mex = 0;
    while (reachable.count(mex)) {
        mex++;
    }
    
    memo[state] = mex;
    return mex;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    a.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    int g = grundy(0, n - 1, a);
    
    if (g != 0) {
        cout << "FEDOR" << endl;
    } else {
        cout << "SERGEY" << endl;
    }
    
    return 0;
}
