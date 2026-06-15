#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<int> a(n+1);
    for(int i = 1; i <= n; i++){
        cin >> a[i];
    }
    
    vector<int> result;
    for(int i = 1; i <= n; i++){
        result.push_back(i);
        result.push_back(-i);
    }
    
    cout << "YES\n";
    for(int x : result){
        cout << x << " ";
    }
    cout << "\n";
    
    return 0;
}
