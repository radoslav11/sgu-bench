#include<bits/stdc++.h>
using namespace std;

bool canFold(vector<int> positions, int maxPos){
    if(positions.empty()) return true;
    sort(positions.begin(), positions.end());
    
    // Try each possible fold line (between integers 0.5, 1.5, ..., maxPos-0.5)
    for(int foldBefore = 1; foldBefore <= maxPos; foldBefore++){
        set<int> mapped;
        bool valid = true;
        for(int pos : positions){
            int newPos;
            if(pos <= foldBefore){
                newPos = pos;
            } else {
                newPos = 2 * foldBefore + 1 - pos;
                if(newPos < 1) valid = false;
            }
            mapped.insert(newPos);
        }
        if(valid && mapped.size() == 1) return true;
    }
    return false;
}

int main(){
    int n, m, k;
    cin >> n >> m >> k;
    
    vector<int> rows, cols;
    for(int i = 0; i < k; i++){
        int x, y;
        cin >> x >> y;
        rows.push_back(x);
        cols.push_back(y);
    }
    
    if(canFold(rows, n) && canFold(cols, m)){
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
