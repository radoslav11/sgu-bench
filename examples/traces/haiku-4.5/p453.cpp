#include <bits/stdc++.h>
using namespace long long;

int main(){
  long long l, v1, v2, t;
  cin >> l >> v1 >> v2 >> t;
  
  long long num = t * (v1 + v2);
  long long ans = num / l;
  
  cout << ans << endl;
  
  return 0;
}
