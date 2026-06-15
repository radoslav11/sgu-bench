#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long l, r;
    cin >> l >> r;
    
    long long answer = 0;
    
    for(long long num = l; num <= r; num++){
        string s = to_string(num);
        string original = s;
        sort(s.begin(), s.end());
        
        set<long long> similar;
        
        do {
            long long perm = stoll(s);
            if(perm >= l && perm <= r){
                similar.insert(perm);
            }
        } while(next_permutation(s.begin(), s.end()));
        
        // If exactly 2 similar numbers exist in range (including itself)
        if(similar.size() == 2){
            answer++;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}
