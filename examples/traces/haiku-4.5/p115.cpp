#include<bits/stdc++.h>
using namespace std;

int main(){
  int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
  int n, m;
  cin >> n >> m;
  
  // Validate month
  if(m < 1 || m > 12){
    cout << "Impossible" << endl;
    return 0;
  }
  
  // Validate day
  if(n < 1 || n > daysInMonth[m]){
    cout << "Impossible" << endl;
    return 0;
  }
  
  // Calculate total days from Jan 1
  int totalDays = 0;
  for(int i = 1; i < m; i++){
    totalDays += daysInMonth[i];
  }
  totalDays += n;
  
  // Calculate day of week (Jan 1 is Monday = 1)
  int dayOfWeek = ((totalDays - 1) % 7) + 1;
  
  cout << dayOfWeek << endl;
  
  return 0;
}
