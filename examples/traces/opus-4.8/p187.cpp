#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val, pri, sz;
    bool rev;
    Node *l, *r;
};

static Node pool[130005];
int poolc=0;

Node* newNode(int v){
    Node* n=&pool[poolc++];
    n->val=v; n->pri=rand()^(rand()<<15); n->sz=1; n->rev=false; n->l=n->r=nullptr;
    return n;
}

int sz(Node* t){return t?t->sz:0;}
void upd(Node* t){if(t)t->sz=1+sz(t->l)+sz(t->r);}
void push(Node* t){
    if(t&&t->rev){
        swap(t->l,t->r);
        if(t->l)t->l->rev^=true;
        if(t->r)t->r->rev^=true;
        t->rev=false;
    }
}

void split(Node* t, int k, Node*& a, Node*& b){
    if(!t){a=b=nullptr;return;}
    push(t);
    if(sz(t->l)+1<=k){
        split(t->r,k-sz(t->l)-1,t->r,b);
        a=t;
    } else {
        split(t->l,k,a,t->l);
        b=t;
    }
    upd(t);
}

Node* merge(Node* a, Node* b){
    if(!a)return b;
    if(!b)return a;
    if(a->pri>b->pri){
        push(a);
        a->r=merge(a->r,b);
        upd(a);
        return a;
    } else {
        push(b);
        b->l=merge(a,b->l);
        upd(b);
        return b;
    }
}

int outbuf[130005];
int oc=0;
void collect(Node* t){
    if(!t)return;
    push(t);
    collect(t->l);
    outbuf[oc++]=t->val;
    collect(t->r);
}

int main(){
    int N,M;
    scanf("%d %d",&N,&M);
    Node* root=nullptr;
    for(int i=1;i<=N;i++) root=merge(root,newNode(i));
    while(M--){
        int p,q;
        scanf("%d %d",&p,&q);
        Node *a,*b,*c;
        split(root,p-1,a,b);
        split(b,q-p+1,b,c);
        if(b)b->rev^=true;
        root=merge(merge(a,b),c);
    }
    collect(root);
    for(int i=0;i<oc;i++){
        printf("%d",outbuf[i]);
        if(i+1<oc)putchar(' ');
    }
    putchar('\n');
    return 0;
}
