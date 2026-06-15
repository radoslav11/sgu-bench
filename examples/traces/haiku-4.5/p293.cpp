#include<bits/stdc++.h>
using namespace std;

int n;
string s;
map<pair<int,string>, vector<pair<int,int>>> memo;

vector<pair<int,int>> solve(int pos, string current){
    if(pos == 2*n){
        return {};
    }
    
    auto key = make_pair(pos, current);
    if(memo.count(key)) return memo[key];
    
    // Add next character
    current[pos] = s[pos];
    
    // Try all swaps
    for(int i = 0; i <= pos; i++){
        for(int j = i; j <= pos; j++){
            string next = current;
            if(i < j) swap(next[i], next[j]);
            
            bool ok = true;
            if((pos+1) % 2 == 1){
                int len = pos + 1;
                for(int k = 0; k < len/2; k++){
                    if(next[k] != next[len-1-k]){
                        ok = false;
                        break;
                    }
                }
            }
            
            if(ok){
                auto res = solve(pos+1, next);
                if(res.size() || pos+1 == 2*n){
                    auto ans = res;
                    ans.insert(ans.begin(), {i == j ? 0 : i+1, j+1});
                    return memo[key] = ans;
                }
            }
        }
    }
    
    return memo[key] = vector<pair<int,int>>();
}

int main(){
    cin >> n >> s;
    auto res = solve(0, string(2*n, '?'));
    if(res.empty()){
        cout << "He\n";
    } else {
        cout << "Qc\n";
        for(auto [a,b] : res) cout << a << " " << b << "\n";
    }
    return 0;
}
