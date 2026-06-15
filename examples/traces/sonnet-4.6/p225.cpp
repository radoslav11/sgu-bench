#include <bits/stdc++.h>
using namespace std;
typedef vector<long long> Big;
const long long BASE=1000000000LL;
Big Zero(){return {0};}
Big One(){return {1};}
Big add(const Big&a,const Big&b){Big c;long long carry=0;for(int i=0;i<(int)max(a.size(),b.size())||carry;i++){long long s=carry;if(i<(int)a.size())s+=a[i];if(i<(int)b.size())s+=b[i];c.push_back(s%BASE);carry=s/BASE;}return c;}
void print(const Big&a){printf("%lld",a.back());for(int i=(int)a.size()-2;i>=0;i--)printf("%09lld",a[i]);printf("\n");}
int n,k;
int main(){
    scanf("%d%d",&n,&k);
    int M=1<<n;
    // dp[p2][p1][cnt]: ways with prev-prev mask p2, prev mask p1, cnt knights placed so far
    // After processing rows 0..row
    // Use map or 3d array
    // M<=1024, k<=100
    // 1024*1024*101 entries... 100M entries, too large
    // But most will be zero. Use map.
    map<tuple<int,int,int>,Big> dp,ndp;
    // Row 0: p2=0 (no prev-prev), p1=0 (no prev)
    // Actually let's do it differently:
    // After row -1: state (0,0,0)->1
    // Process each row
    dp[{0,0,0}]=One();
    auto conf1=[&](int a,int b){// 1 apart
        for(int i=0;i<n;i++)if((a>>i)&1){if(i+2<n&&((b>>(i+2))&1))return true;if(i-2>=0&&((b>>(i-2))&1))return true;}return false;};
    auto conf2=[&](int a,int b){// 2 apart
        for(int i=0;i<n;i++)if((a>>i)&1){if(i+1<n&&((b>>(i+1))&1))return true;if(i-1>=0&&((b>>(i-1))&1))return true;}return false;};
    for(int row=0;row<n;row++){
        ndp.clear();
        for(auto&[st,val]:dp){
            auto[p2,p1,cnt]=st;
            for(int m=0;m<M;m++){
                int c=__builtin_popcount(m);
                if(cnt+c>k)continue;
                if(conf1(p1,m))continue;
                if(conf2(p2,m))continue;
                auto key=make_tuple(p1,m,cnt+c);
                ndp[key]=add(ndp.count(key)?ndp[key]:Zero(),val);
            }
        }
        dp=ndp;
    }
    Big ans=Zero();
    for(auto&[st,val]:dp){
        if(get<2>(st)==k)ans=add(ans,val);
    }
    print(ans);
}
