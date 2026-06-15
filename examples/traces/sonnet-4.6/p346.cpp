#include <bits/stdc++.h>
using namespace std;
int main(){
    int balls[7];
    for(int i=0;i<7;i++) cin>>balls[i];
    string state; cin>>state;
    int r=balls[0];
    int score=0;
    int cv[]={2,3,4,5,6,7};
    if(r==0 && state=="RED"){
        score+=7;
    } else {
        while(r>0){
            if(state=="RED"){score+=7; state="COLOUR";}
            else {score+=1; r--; state="RED";}
        }
        if(state=="RED") score+=7;
    }
    for(int i=1;i<7;i++) if(balls[i]) score+=cv[i-1];
    cout<<score<<endl;
}
