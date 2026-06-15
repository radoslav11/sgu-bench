#include <bits/stdc++.h>
using namespace std;
int main(){
    int K, N;
    cin >> K >> N;
    int cnt[10] = {0};
    for(int i=0;i<N;i++){
        string s; cin >> s;
        cnt[s[0]-'0']++;
    }
    int pages = 2;
    for(int d=0;d<10;d++){
        if(cnt[d]>0){
            pages += (cnt[d]+K-1)/K;
        }
    }
    cout << pages << endl;
    return 0;
}
