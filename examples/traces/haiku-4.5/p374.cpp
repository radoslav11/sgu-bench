#include<iostream>
#include<cmath>
using namespace std;

int main(){
  long long a, b, k;
  cin >> a >> b >> k;
  
  long long result = 1;
  long long base = a + b;
  
  for(int i = 0; i < k; i++){
    result *= base;
  }
  
  cout << result << endl;
  
  return 0;
}
