#include<bits/stdc++.h>
using namespace std;
int bit_[1000006];int N;
void upd(int i){for(++i;i<=N;i+=i&-i)bit_[i]++;}
int qr(int i){int s=0;for(++i;i>0;i-=i&-i)s+=bit_[i];return s;}
int main(){
    string s;
    if(!(cin>>s))return 0;
    int n=s.size();N=n;
    vector<deque<int>> pos(26);
    for(int i=0;i<n;i++)pos[s[i]-'A'].push_back(i);
    int odd=0;for(int c=0;c<26;c++)if(pos[c].size()&1)odd++;
    if(odd>(n&1?1:0)){printf("-1\n");return 0;}
    vector<int> order(n,-1);
    vector<char> used(n,0);
    int l=0,r=n-1;
    for(int i=0;i<n;i++){
        if(used[i])continue;
        int c=s[i]-'A';
        while(!pos[c].empty()&&used[pos[c].front()])pos[c].pop_front();
        int fi=pos[c].front();pos[c].pop_front();used[fi]=1;
        while(!pos[c].empty()&&used[pos[c].back()])pos[c].pop_back();
        if(pos[c].empty()){
            order[(n-1)/2]=fi;
        } else {
            int bi=pos[c].back();pos[c].pop_back();used[bi]=1;
            order[l]=fi;order[r]=bi;l++;r--;
        }
    }
    long long inv=0;
    for(int p=n-1;p>=0;p--){
        inv+=qr(order[p]-1);
        upd(order[p]);
    }
    printf("%lld\n",inv);
    return 0;
}
