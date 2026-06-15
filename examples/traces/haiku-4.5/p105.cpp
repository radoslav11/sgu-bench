#include<bits/stdc++.h>
using namespace std;

long long digitSum(long long n) {
    long long sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    cin >> n;
    
    long long count = 0;
    long long cumulativeSum = 0;
    
    for (long long i = 1; i <= n; i++) {
        cumulativeSum += digitSum(i);
        if (cumulativeSum % 3 == 0) {
            count++;
        }
    }
    
    cout << count << endl;
    
    return 0;
}
