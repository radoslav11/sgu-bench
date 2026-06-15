#include<bits/stdc++.h>
using namespace std;

int main(){
  long long m, n;
  cin >> m >> n;
  
  // Make m <= n
  if(m > n) swap(m, n);
  
  // If either dimension is 1
  if(m == 1){
    cout << 1 << endl;
    return 0;
  }
  
  // If m is odd, no cornerless tiling possible (except 1xn)
  if(m % 2 == 1){
    cout << 0 << endl;
    return 0;
  }
  
  // If n is odd
  if(n % 2 == 1){
    cout << 0 << endl;
    return 0;
  }
  
  // Both even
  if(m == 2){
    cout << 1 << endl;
    return 0;
  }
  
  // Both even and >= 4
  long long exp = (m/2 - 1) * (n/2 - 1);
  long long result = 1LL << exp;  // 2^exp
  cout << result << endl;
  
  return 0;
}
