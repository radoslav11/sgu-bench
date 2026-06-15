#include<bits/stdc++.h>
using namespace std;
int main(){
    int N, M;
    cin >> N >> M;
    long long total = 0;
    for(int i = 0; i < M; i++){
        int s;
        cin >> s;
        total += s;
    }
    cout << total % N << endl;
    return 0;
}
