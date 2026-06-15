#include <bits/stdc++.h>
using namespace std;
int H,W;vector<vector<int>>B;
int main(){scanf("%d %d",&H,&W);B.assign(H,vector<int>(W,0));
for(int i=0;i<H-1;i++){char s[400];scanf("%s",s);for(int j=0;j<W-1;j++)B[i][j]=s[j]-'0';}
vector<vector<int>>A(H,vector<int>(W));
for(int a00=0;a00<2;a00++){
 A[0][0]=a00;bool ok=true;
 for(int i=0;i+1<H;i++)A[i+1][0]=0;
 for(int j=0;j+1<W;j++){int found=-1;for(int v=0;v<2;v++){A[0][j+1]=v;
   bool good=true;for(int i=0;i+1<H;i++){int x=B[i][j]-A[i][j]-A[i][j+1]-A[i+1][j];if(x<0||x>1){good=false;break;}A[i+1][j+1]=x;}
   if(good){found=v;break;}}
   if(found<0){ok=false;break;}A[0][j+1]=found;
   for(int i=0;i+1<H;i++)A[i+1][j+1]=B[i][j]-A[i][j]-A[i][j+1]-A[i+1][j];}
 if(!ok)continue;
 bool valid=true;for(int i=0;i+1<H&&valid;i++)for(int j=0;j+1<W;j++){if(A[i][j]+A[i+1][j]+A[i][j+1]+A[i+1][j+1]!=B[i][j]){valid=false;break;}}
 for(int i=0;i<H&&valid;i++)for(int j=0;j<W;j++)if(A[i][j]<0||A[i][j]>1)valid=false;
 if(valid){for(int i=0;i<H;i++){for(int j=0;j<W;j++)putchar('0'+A[i][j]);putchar('\n');}return 0;}
}
printf("CORRUPT\n");return 0;}
