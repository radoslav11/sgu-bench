#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n, m;
    cin >> n >> m;
    
    if(n == 1){
        cout << 1 % m;
        return 0;
    }
    if(n == 2){
        cout << 2 % m;
        return 0;
    }
    
    vector<long long> row(n+2, 0), tmp(n+2, 0);
    row[0] = 1 % m;
    
    for(int i = 1; i <= n; i++){
        if(i % 2 == 1){
            tmp[0] = 0;
            for(int j = 1; j <= i; j++){
                tmp[j] = (tmp[j-1] + row[j-1]) % m;
            }
        } else {
            tmp[i] = 0;
            for(int j = i-1; j >= 0; j--){
                tmp[j] = (tmp[j+1] + row[j]) % m;
            }
        }
        swap(row, tmp);
    }
    
    long long En;
    if(n % 2 == 1){
        En = row[n];
    } else {
        En = row[0];
    }
    
    cout << 2 * En % m;
    return 0;
}
