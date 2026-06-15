#include<bits/stdc++.h>
using namespace std;
int h,w; vector<string> g;
struct Man{ vector<int> d1,d2; int n;
  void build(const string&s){ n=s.size(); d1.assign(n,0); d2.assign(n,0);
    for(int i=0,l=0,r=-1;i<n;i++){int k=(i>r)?1:min(d1[l+r-i],r-i+1);
      while(i-k>=0&&i+k<n&&s[i-k]==s[i+k])k++; d1[i]=k; if(i+k-1>r){l=i-k+1;r=i+k-1;}}
    for(int i=0,l=0,r=-1;i<n;i++){int k=(i>r)?0:min(d2[l+r-i+1],r-i+1);
      while(i-k-1>=0&&i+k<n&&s[i-k-1]==s[i+k])k++; d2[i]=k; if(i+k-1>r){l=i-k;r=i+k-1;}}
  }
  bool pal(int l,int r){ int len=r-l+1; int c=(l+r)/2;
    if(len&1) return d1[c]>=len/2+1; else return d2[c+1]>=len/2; }
};
vector<Man> rowM, colM;
int ar1,ac1,an;
bool check(int n){
  static vector<vector<char>> H,V; H.assign(h,vector<char>(w,0)); V.assign(h,vector<char>(w,0));
  for(int i=0;i<h;i++)for(int j=0;j+n<=w;j++) H[i][j]=rowM[i].pal(j,j+n-1);
  for(int j=0;j<w;j++)for(int i=0;i+n<=h;i++) V[i][j]=colM[j].pal(i,i+n-1);
  static vector<vector<int>> vr,hr; vr.assign(h+1,vector<int>(w,0)); hr.assign(h,vector<int>(w+1,0));
  for(int j=0;j<w;j++)for(int i=h-1;i>=0;i--) vr[i][j]=H[i][j]?vr[i+1][j]+1:0;
  for(int i=0;i<h;i++)for(int j=w-1;j>=0;j--) hr[i][j]=V[i][j]?hr[i][j+1]+1:0;
  for(int i=0;i+n<=h;i++)for(int j=0;j+n<=w;j++) if(vr[i][j]>=n&&hr[i][j]>=n){ar1=i;ac1=j;an=n;return true;}
  return false;
}
int main(){
  scanf("%d %d",&h,&w); g.resize(h); for(auto&s:g){char b[800];scanf("%s",b);s=b;}
  rowM.resize(h); for(int i=0;i<h;i++)rowM[i].build(g[i]);
  colM.resize(w); for(int j=0;j<w;j++){string c(h,' ');for(int i=0;i<h;i++)c[i]=g[i][j];colM[j].build(c);}
  int best=1,br=0,bc=0;
  {int lo=1,hi=min(h,w),res=1,rr=0,rc=0; while(lo<=hi){int mid=(lo+hi)/2; if(mid%2==0)mid--; if(mid<lo){lo=mid+2;continue;} if(check(mid)){res=mid;rr=ar1;rc=ac1;lo=mid+2;}else hi=mid-1;} if(res>best){best=res;br=rr;bc=rc;}}
  {int lo=2,hi=min(h,w),res=0,rr=0,rc=0; while(lo<=hi){int mid=(lo+hi)/2; if(mid%2==1)mid--; if(mid<lo){lo=mid+2;continue;} if(check(mid)){res=mid;rr=ar1;rc=ac1;lo=mid+2;}else hi=mid-1;} if(res>best){best=res;br=rr;bc=rc;}}
  printf("%d %d %d %d\n",br+1,bc+1,br+best,bc+best);
}
