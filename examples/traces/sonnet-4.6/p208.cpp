#include<bits/stdc++.h>
using namespace std;
typedef vector<long long> Big;
const long long B=1000000000LL;
Big fromInt(long long x){Big r;if(x==0)r.push_back(0);while(x>0){r.push_back(x%B);x/=B;}return r;}
Big add(Big a,Big b){Big r;long long c=0;for(int i=0;i<(int)max(a.size(),b.size())||c;i++){long long s=c;if(i<(int)a.size())s+=a[i];if(i<(int)b.size())s+=b[i];r.push_back(s%B);c=s/B;}return r;}
Big mul2(Big a,long long x){Big r;long long c=0;for(int i=0;i<(int)a.size()||c;i++){long long s=c;if(i<(int)a.size())s+=a[i]*x;r.push_back(s%B);c=s/B;}while(r.size()>1&&r.back()==0)r.pop_back();return r;}
Big divB(Big a,long long x){Big r(a.size());long long c=0;for(int i=a.size()-1;i>=0;i--){long long cur=a[i]+c*B;r[i]=cur/x;c=cur%x;}while(r.size()>1&&r.back()==0)r.pop_back();return r;}
void print(Big a){printf("%lld",a.back());for(int i=a.size()-2;i>=0;i--)printf("%09lld",a[i]);printf("\n");}
int main(){
    int N,M;scanf("%d%d",&N,&M);
    // Burnside: group is Z_N x Z_M (translations)
    // For each (r,c), fixed colorings = 2^(gcd(N,r_ord)*gcd... 
    // number of orbits of shift(r,c) = N*M / lcm(N/gcd(N,r), M/gcd(M,c))
    // but for r=0, N/gcd(N,0)=N/N=1, so lcm(1, M/gcd(M,c))=M/gcd(M,c)
    // orbits = N*M / (M/gcd(M,c)) = N*gcd(M,c). That seems right for r=0.
    Big sum=fromInt(0);
    for(int r=0;r<N;r++)for(int c=0;c<M;c++){
        int gr=__gcd(N,r==0?N:r);// gcd(N,r) but r=0->gcd=N
        int gc=__gcd(M,c==0?M:c);
        // order of r in Z_N = N/gcd(N,r)
        int ordR=N/gr, ordC=M/gc;
        int L=ordR/__gcd(ordR,ordC)*ordC;// lcm
        int orbits=N/L*(M/L)*L;// = N*M/L
        // 2^orbits
        Big pw=fromInt(1);
        for(int i=0;i<orbits;i++)pw=mul2(pw,2);
        sum=add(sum,pw);
    }
    // Also if N==M, add transpose symmetries? Let's check 2x2 without:
    // sum without transpose: already computed as 28, /4=7 not 6.
    // Hmm need transpose for N=M.
    // For N=M, group size doubles. Add transpose composed with each translation.
    // Transpose+shift(r,c): (i,j)->(j+r mod N, i+c mod N)
    // Fixed point: color[i][j]=color[j+r][i+c] and color[j+r][i+c]=color[i+c+r][j+r+c]...
    // The permutation cycles...
    if(N==M){
        // For each (r,c), consider the permutation T_{r,c}: (i,j)->(j+r,i+c) mod N
        // Find number of cycles
        for(int r=0;r<N;r++)for(int c=0;c<N;c++){
            // Count cycles of T_{r,c}
            vector<vector<bool>> vis(N,vector<bool>(N,false));
            int cycles=0;
            for(int i=0;i<N;i++)for(int j=0;j<N;j++){
                if(!vis[i][j]){
                    cycles++;
                    int ci=i,cj=j;
                    while(!vis[ci][cj]){
                        vis[ci][cj]=true;
                        int ni=(cj+r)%N, nj=(ci+c)%N;
                        ci=ni;cj=nj;
                    }
                }
            }
            Big pw=fromInt(1);
            for(int i=0;i<cycles;i++)pw=mul2(pw,2);
            sum=add(sum,pw);
        }
        // group size = 2*N*N
        print(divB(sum,2LL*N*N));
    } else {
        print(divB(sum,(long long)N*M));
    }
}
