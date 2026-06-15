#include <bits/stdc++.h>
using namespace std;

static double prob[8192][6][6];

int main(){
    int N;
    scanf("%d",&N);
    double p[13];
    for(int i=0;i<12;i++) scanf("%lf",&p[i]);
    double sum=0;
    for(int i=0;i<N;i++){ double x; scanf("%lf",&x); sum+=x; }
    p[12]= sum/N;
    
    // group masks by popcount
    vector<vector<int>> byCount(14);
    for(int mask=0; mask<8192; mask++){
        byCount[__builtin_popcount(mask)].push_back(mask);
    }
    
    double ans[12]={0};
    prob[0][0][0]=1.0;
    
    for(int cnt=0; cnt<=12; cnt++){
        for(int mask: byCount[cnt]){
            for(int c=0;c<6;c++){
                for(int v=0;v<6;v++){
                    double pr = prob[mask][c][v];
                    if(pr<=0) continue;
                    // for each remaining envelope s
                    for(int s=0; s<13; s++){
                        if(mask & (1<<s)) continue; // removed
                        // count consecutive removed preceding (s-1, s-2,...)
                        int count=1;
                        int k=(s-1+13)%13;
                        while(k!=s && (mask&(1<<k))){
                            count++;
                            k=(k-1+13)%13;
                        }
                        double sp = (double)count/13.0;
                        double ev = pr*sp;
                        double ps = p[s];
                        int newmask = mask|(1<<s);
                        // success -> club point
                        if(c+1==6){
                            ans[v] += ev*ps;
                        } else {
                            prob[newmask][c+1][v] += ev*ps;
                        }
                        // failure -> viewer point
                        if(v+1==6){
                            ans[11-c] += ev*(1-ps);
                        } else {
                            prob[newmask][c][v+1] += ev*(1-ps);
                        }
                    }
                }
            }
        }
    }
    
    for(int i=0;i<12;i++){
        printf("%.3f\n", ans[i]);
    }
    return 0;
}
