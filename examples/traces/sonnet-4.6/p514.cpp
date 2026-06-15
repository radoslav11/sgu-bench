#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<int>a(n);
    for(auto&x:a)cin>>x;
    int S=0;for(auto x:a)S+=x;
    // Try all subsets, find min |sum(subset)-sum(rest)| > 0
    // infimum = 0 if any subset sums to S/2
    // else infimum = area of degenerate triangle with sides closest
    double ans=1e18;
    for(int mask=1;mask<(1<<n)-1;mask++){
        int s=0;for(int i=0;i<n;i++)if(mask>>i&1)s+=a[i];
        int t=S-s;
        // two "sides" s and t, third side approaches |s-t|
        // degenerate triangle: sides s,t,|s-t| -> area=0
        // but minimum area triangle with sides s,t and third side from 0 to s+t
        // infimum of area = 0 if s!=t, but constrained by the polygon being simple
        // Actually infimum = 0 always if n>=4? But example 2 shows otherwise
        if(s==t){ans=0;break;}
    }
    if(ans==0){printf("%.10f\n",0.0);return 0;}
    // For triangle n=3
    if(n==3){
        double s=S/2.0;
        double area=sqrt(s*(s-a[0])*(s-a[1])*(s-a[2]));
        printf("%.10f\n",area);return 0;
    }
    // For n>=4, infimum = min over partitions of |s-t|... 
    // Actually I think infimum = min over all partitions of sqrt(s*t - ((s+t)^2/4 ... 
    // Let me reconsider: degenerate to two straight segments of length s and t
    // The "thin triangle" area approaches 0. So answer should be 0 for n>=4?
    // But example 2 is n=4 with nonzero answer...
    printf("%.10f\n",0.0);
}
