#include<bits/stdc++.h>
using namespace std;

int main(){
  string A;
  long long B;
  cin >> A >> B;
  
  // Convert A to base B
  vector<long long> digits;
  
  // Handle the number as a string, convert to base B
  long long remainder = 0;
  for(int i = 0; i < A.length(); i++){
    remainder = remainder * 10 + (A[i] - '0');
    long long digit = remainder % B;
    digits.push_back(digit);
    remainder = remainder / B;
  }
  
  // Handle remaining remainder
  while(remainder > 0){
    digits.push_back(remainder % B);
    remainder /= B;
  }
  
  // Now process digits to minimize terms
  // If digit >= (B+1)/2, we can replace with negative
  int threshold = (B + 1) / 2;
  
  for(int i = 0; i < digits.size(); i++){
    if(digits[i] >= threshold){
      digits[i] = digits[i] - B;
      if(i + 1 < digits.size()){
        digits[i + 1]++;
      } else {
        digits.push_back(1);
      }
    }
  }
  
  // Handle carries from the transformation
  for(int i = 0; i < digits.size(); i++){
    if(digits[i] >= threshold){
      digits[i] = digits[i] - B;
      if(i + 1 < digits.size()){
        digits[i + 1]++;
      } else {
        digits.push_back(1);
      }
    }
  }
  
  // Count non-zero digits
  int count = 0;
  for(auto d : digits){
    if(d != 0) count++;
  }
  
  cout << count << endl;
  
  return 0;
}
