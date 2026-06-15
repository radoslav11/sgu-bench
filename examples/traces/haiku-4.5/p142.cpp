#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    string key;
    cin >> n >> key;
    
    // Use BFS to find shortest string not in key
    queue<string> q;
    q.push("a");
    q.push("b");
    
    while(!q.empty()){
        string candidate = q.front();
        q.pop();
        
        // Check if candidate is in key
        if(key.find(candidate) == string::npos){
            // Found the keyword!
            cout << candidate.length() << "\n";
            cout << candidate << "\n";
            return 0;
        }
        
        // Add extensions of this candidate
        q.push(candidate + "a");
        q.push(candidate + "b");
    }
    
    return 0;
}
