#include<bits/stdc++.h>
using namespace std;

int main(){
  long long n;
  cin >> n;
  
  for(long long h = 1; h <= 1000000; h++){
    long long minSum = 14*h + 7;
    long long maxSum = 19*h + 2;
    
    if(n >= minSum && n <= maxSum){
      cout << h << endl;
      return 0;
    }
    
    if(minSum > n) break;
  }
  
  cout << -1 << endl;
  return 0;
}
