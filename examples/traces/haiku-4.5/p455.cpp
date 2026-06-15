#include <bits/stdc++.h>
using namespace std;

int main(){
  long long A, B, C;
  cin >> A >> B >> C;
  
  unordered_map<long long, long long> first_occurrence;
  long long x = 1;
  long long LIMIT = 2000000;
  
  for(long long i = 0; i <= LIMIT; i++){
    if(first_occurrence.count(x)){
      cout << i << endl;
      return 0;
    }
    first_occurrence[x] = i;
    x = (A * x + B) % C;
  }
  
  cout << -1 << endl;
  return 0;
}
