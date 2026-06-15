#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<int> a;
vector<int> f;

static inline int val(int idx, int j){
    int v = a[idx] + j;
    if(v >= m) v -= m;
    return v;
}

int booth(int j){
    int twoN = 2*n;
    // reset f
    for(int p=0;p<twoN;p++) f[p] = -1;
    int k = 0;
    for(int jj=1; jj<twoN; jj++){
        int i = f[jj-k-1];
        int sj = val(jj % n, j);
        int cmpIdx = (k+i+1)%n;
        while(i != -1 && sj != val((k+i+1)%n, j)){
            if(sj < val((k+i+1)%n, j)) k = jj - i - 1;
            i = f[i];
        }
        if(i == -1 && sj != val((k+i+1)%n, j)){
            if(sj < val((k+i+1)%n, j)) k = jj;
            f[jj-k] = -1;
        } else {
            f[jj-k] = i+1;
        }
    }
    return k;
}

int main(){
    int k;
    scanf("%d %d %d", &n, &m, &k);
    a.resize(n);
    for(int i=0;i<n;i++) scanf("%d", &a[i]);
    f.assign(2*n, -1);
    
    string out;
    out.reserve(m*3);
    char buf[16];
    for(int j=0;j<m;j++){
        int start = booth(j);
        int idx = (start + k - 1) % n;
        int v = val(idx, j);
        int len = sprintf(buf, "%d\n", v);
        out.append(buf, len);
    }
    fputs(out.c_str(), stdout);
    return 0;
}
