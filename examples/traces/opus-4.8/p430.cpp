#include <bits/stdc++.h>
using namespace std;

int n,m;
bool adj[7][7];
double X[7],Y[7];
mt19937 rng(987654321);
double rnd(double a,double b){ return a + (b-a)*(rng()/(double)UINT32_MAX); }

const double EDGE_TOL=1e-9;
const double MARGIN=0.02;

bool checkPair(int v,double cx,double cy,vector<int>&order,int idx){
  for(int j=0;j<idx;j++){
    int u=order[j];
    double dx=cx-X[u], dy=cy-Y[u];
    double d=sqrt(dx*dx+dy*dy);
    if(adj[v][u]){
      if(fabs(d-1.0)>EDGE_TOL) return false;
    } else {
      if(d<MARGIN) return false;
      if(fabs(d-1.0)<MARGIN) return false;
    }
  }
  return true;
}

bool rec(vector<int>&order,int idx){
  if(idx==n) return true;
  int v=order[idx];
  vector<int> nb;
  for(int j=0;j<idx;j++) if(adj[v][order[j]]) nb.push_back(order[j]);
  if(nb.empty()){
    for(int t=0;t<50;t++){
      double cx=rnd(-3,3), cy=rnd(-3,3);
      if(checkPair(v,cx,cy,order,idx)){X[v]=cx;Y[v]=cy; if(rec(order,idx+1))return true;}
    }
    return false;
  } else if(nb.size()==1){
    int a=nb[0];
    for(int t=0;t<50;t++){
      double ang=rnd(0,2*M_PI);
      double cx=X[a]+cos(ang), cy=Y[a]+sin(ang);
      if(checkPair(v,cx,cy,order,idx)){X[v]=cx;Y[v]=cy; if(rec(order,idx+1))return true;}
    }
    return false;
  } else {
    int a=nb[0],b=nb[1];
    double dx=X[b]-X[a], dy=Y[b]-Y[a];
    double d=sqrt(dx*dx+dy*dy);
    if(d<1e-9||d>2.0) return false;
    double a0=d/2.0;
    double h2=1.0-a0*a0;
    if(h2<-1e-12) return false;
    if(h2<0)h2=0;
    double h=sqrt(h2);
    double mx=X[a]+dx*0.5, my=Y[a]+dy*0.5;
    double ux=-dy/d, uy=dx/d;
    for(int s=0;s<2;s++){
      double cx=mx+(s?h:-h)*ux;
      double cy=my+(s?h:-h)*uy;
      if(checkPair(v,cx,cy,order,idx)){X[v]=cx;Y[v]=cy; if(rec(order,idx+1))return true;}
    }
    return false;
  }
}

vector<int> makeOrder(){
  vector<int> order; vector<char> used(n,0);
  while((int)order.size()<n){
    int bestcnt=-1; vector<int> cands;
    for(int v=0;v<n;v++) if(!used[v]){
       int c=0; for(int u:order) if(adj[v][u])c++;
       if(c>bestcnt){bestcnt=c;cands.clear();cands.push_back(v);}
       else if(c==bestcnt) cands.push_back(v);
    }
    int pick=cands[rng()%cands.size()];
    used[pick]=1; order.push_back(pick);
  }
  return order;
}

int main(){
  scanf("%d %d",&n,&m);
  for(int i=0;i<m;i++){
    int a,b; scanf("%d %d",&a,&b);
    adj[a][b]=adj[b][a]=true;
  }
  if(n==1){
    printf("Yes\n0.0 0.0\n");
    return 0;
  }
  clock_t start=clock();
  bool found=false;
  while(true){
    vector<int> order=makeOrder();
    if(rec(order,0)){found=true;break;}
    if((double)(clock()-start)/CLOCKS_PER_SEC > 0.62) break;
  }
  if(!found){
    printf("No\n");
  } else {
    printf("Yes\n");
    for(int i=0;i<n;i++){
      printf("%.15f %.15f\n",X[i],Y[i]);
    }
  }
  return 0;
}
