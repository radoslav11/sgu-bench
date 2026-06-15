#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; cin>>N;
    long long cnt=0;
    for(int A=1;A<N;A++){
        for(int B=A+1;B<=N-A;B++){
            // need y>=1, B*y <= N-A, (N-B*y)%A==0, (N-B*y)/A>=1
            // B*y ≡ N (mod A), y in [1, (N-A)/B]
            int g=__gcd(A,B);
            if(N%g!=0) continue; // Hmm wait, need N-A-B>=0 and solvable
            // Actually need (N - B*y) % A == 0 and N-B*y >= A
            // So B*y ≡ N mod A, min y satisfying this in [1,(N-A)/B]
            int r = N % A;
            int br = B % A;
            // br * y ≡ r (mod A)
            int gg = __gcd(br, A);
            if(r % gg != 0) continue;
            // find smallest positive y
            int a2=A/gg, b2=br/gg, r2=r/gg;
            // b2*y ≡ r2 (mod a2), gcd(b2,a2)=1
            // find inverse of b2 mod a2
            // extended gcd
            // y0 = r2 * inv(b2) mod a2
            // smallest positive y0
            if(a2==0){cnt++;continue;}
            long long inv=1; // compute b2^-1 mod a2
            // use extended Euclidean
            long long old_r=b2,old_s=1,cur_r=a2,cur_s=0;
            while(cur_r){
                long long q=old_r/cur_r;
                long long tmp=cur_r; cur_r=old_r-q*cur_r; old_r=tmp;
                tmp=cur_s; cur_s=old_s-q*cur_s; old_s=tmp;
            }
            inv=old_s;
            long long y0=((long long)r2*inv % a2 + a2)%a2;
            if(y0==0) y0=a2;
            if((long long)B*y0 <= N-A) cnt++;
        }
    }
    cout<<cnt<<endl;
}
