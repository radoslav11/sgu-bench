#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<int> rem(n);
    int total=0;
    for(int i=0;i<n;i++){cin>>rem[i]; total+=rem[i];}
    int G=total/2;
    cout<<G<<"\n";
    auto pickOther=[&](int cur)->int{
        int best=-1;
        for(int i=0;i<n;i++) if(i!=cur && rem[i]>0){
            if(best==-1||rem[i]>rem[best]) best=i;
        }
        return best;
    };
    int a=-1,b=-1;
    for(int i=0;i<n;i++) if(rem[i]>0){
        if(a==-1||rem[i]>rem[a]){b=a;a=i;}
        else if(b==-1||rem[i]>rem[b]) b=i;
    }
    int cur;
    {
        rem[a]--; rem[b]--;
        int w,l;
        if(rem[a]>=rem[b]){w=a;l=b;} else {w=b;l=a;}
        cout<<w+1<<" "<<l+1<<"\n";
        cur=w;
    }
    for(int g=1;g<G;g++){
        int o=pickOther(cur);
        rem[cur]--; rem[o]--;
        int w,l;
        if(rem[cur]>=rem[o]){w=cur;l=o;} else {w=o;l=cur;}
        cout<<w+1<<" "<<l+1<<"\n";
        cur=w;
    }
    return 0;
}
