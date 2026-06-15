#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int K;
    string s;
    cin >> K >> s;
    
    int n = s.length();
    string cyclic = s + s; // Create cyclic version
    
    string best = "";
    
    // Try all possible substrings of even length
    for(int start = 0; start < n; start++){
        // Try all even lengths starting from this position
        for(int len = 2; len <= n; len += 2){
            string substring = cyclic.substr(start, len);
            
            // Check if it's K-even
            int half = len / 2;
            int differences = 0;
            
            for(int i = 0; i < half; i++){
                if(substring[i] != substring[half + i]){
                    differences++;
                }
            }
            
            if(differences <= K){
                // This is a K-even string
                // Check if it's better than current best
                if(substring.length() > best.length() || 
                   (substring.length() == best.length() && substring < best)){
                    best = substring;
                }
            }
        }
    }
    
    cout << best << endl;
    
    return 0;
}
