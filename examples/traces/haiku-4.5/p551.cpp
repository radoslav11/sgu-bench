#include<bits/stdc++.h>
using namespace std;

int main(){
    int n, t1, t2;
    cin >> n >> t1 >> t2;
    
    int petya_count = 0;
    int vasya_count = 0;
    long long petya_time = t1;
    long long vasya_time = t2;
    
    while(petya_count + vasya_count < n) {
        if(petya_time <= vasya_time) {
            petya_count++;
            petya_time += t1;
        } else {
            vasya_count++;
            vasya_time += t2;
        }
    }
    
    long long last_time = max(petya_time - t1, vasya_time - t2);
    cout << petya_count + vasya_count << " " << last_time << endl;
    
    return 0;
}
