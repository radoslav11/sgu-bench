#include<bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    cin >> n >> m;
    
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if(m < 1 || m > 12 || n < 1 || n > days_in_month[m]){
        cout << "Impossible" << endl;
        return 0;
    }
    
    int day_of_year = 0;
    for(int i = 1; i < m; i++){
        day_of_year += days_in_month[i];
    }
    day_of_year += n;
    
    // Jan 1, 2001 is Monday = day 1
    // day_of_year=1 -> Monday=1
    int dow = (day_of_year - 1) % 7 + 1;
    
    cout << dow << endl;
    
    return 0;
}
