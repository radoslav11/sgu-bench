#include <bits/stdc++.h>
using namespace std;

int N;

// reconstruct ranks (1-based) from array a where a[i]=number of larger elements above/before position i
// returns empty vector if invalid
vector<int> reconstruct(const vector<int>& a){
    int n = a.size();
    vector<long long> fen(n+1,0);
    auto upd=[&](int i,int v){ for(i++; i<=n; i+=i&-i) fen[i]+=v; };
    int LOG=1; while((1<<LOG)<=n) LOG++;
    auto findKth=[&](int k)->int{ // smallest 0-based index with prefix free count == k (k 1-based)
        int pos=0; long long kk=k;
        for(int pw=(1<<LOG); pw>0; pw>>=1){
            if(pos+pw<=n && fen[pos+pw]<kk){ pos+=pw; kk-=fen[pos]; }
        }
        return pos; // 0-based slot index
    };
    for(int i=0;i<n;i++) upd(i,1);
    vector<int> res(n);
    for(int i=n-1;i>=0;i--){
        if(a[i]<0 || a[i]>i) return {};
        int s = i - a[i]; // insertion index (0-based) = number of free slots before it
        int slot = findKth(s+1);
        res[i]=slot;
        upd(slot,-1);
    }
    for(auto &x:res) x++; // rank = final0idx+1
    return res;
}

int main(){
    scanf("%d",&N);
    int n=N;
    vector<int> topM(n*n), leftM(n*n);
    for(int i=0;i<n;i++)for(int j=0;j<n;j++) scanf("%d",&topM[i*n+j]);
    for(int i=0;i<n;i++)for(int j=0;j<n;j++) scanf("%d",&leftM[i*n+j]);
    
    vector<int> rankRow(n*n), rankCol(n*n);
    vector<int> rowInv(n*n), colInv(n*n);
    
    // columns -> rankCol
    for(int j=0;j<n;j++){
        vector<int> a(n);
        for(int i=0;i<n;i++) a[i]=topM[i*n+j];
        vector<int> r=reconstruct(a);
        if(r.empty()){ printf("0\n"); return 0; }
        for(int i=0;i<n;i++){
            rankCol[i*n+j]=r[i];
            colInv[j*n + (r[i]-1)] = i;
        }
    }
    // rows -> rankRow
    for(int i=0;i<n;i++){
        vector<int> a(n);
        for(int j=0;j<n;j++) a[j]=leftM[i*n+j];
        vector<int> r=reconstruct(a);
        if(r.empty()){ printf("0\n"); return 0; }
        for(int j=0;j<n;j++){
            rankRow[i*n+j]=r[j];
            rowInv[i*n + (r[j]-1)] = j;
        }
    }
    
    int total=n*n;
    vector<int> indeg(total,0);
    for(int i=0;i<n;i++)for(int j=0;j<n;j++){
        int cell=i*n+j;
        indeg[cell] = (rankRow[cell]>1?1:0) + (rankCol[cell]>1?1:0);
    }
    
    // Kahn
    vector<int> A(total,0);
    vector<int> queue; queue.reserve(total);
    for(int c=0;c<total;c++) if(indeg[c]==0) queue.push_back(c);
    int head=0, val=0;
    while(head < (int)queue.size()){
        int cell=queue[head++];
        val++;
        A[cell]=val;
        int i=cell/n, j=cell%n;
        // row successor
        int rr=rankRow[cell];
        if(rr<n){
            int j2=rowInv[i*n + rr]; // rank rr+1 -> index rr (0-based)
            int s=i*n+j2;
            if(--indeg[s]==0) queue.push_back(s);
        }
        // col successor
        int rc=rankCol[cell];
        if(rc<n){
            int i2=colInv[j*n + rc];
            int s=i2*n+j;
            if(--indeg[s]==0) queue.push_back(s);
        }
    }
    
    if(val != total){ printf("0\n"); return 0; }
    
    // output
    string out;
    out.reserve(total*7);
    char buf[16];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(j) out.push_back(' ');
            int len=sprintf(buf,"%d",A[i*n+j]);
            out.append(buf,len);
        }
        out.push_back('\n');
    }
    fputs(out.c_str(), stdout);
    return 0;
}
