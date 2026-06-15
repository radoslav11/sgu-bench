#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M,K;
    scanf("%d %d %d",&N,&M,&K);
    deque<string> pile;
    char buf[10];
    for(int i=0;i<N;i++){scanf("%s",buf);pile.push_back(buf);}
    for(int i=0;i<M;i++){
        scanf("%s",buf);
        string op(buf);
        if(op[0]=='A'){
            string s=op.substr(4,op.size()-5);
            pile.push_front(s);
        } else {
            int r=min(K,(int)pile.size());
            reverse(pile.begin(),pile.begin()+r);
        }
    }
    for(auto&s:pile)printf("%s\n",s.c_str());
}
