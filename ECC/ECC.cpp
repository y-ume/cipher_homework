#include <iostream>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define rep(i,a,b) for(int i=(a);i<(b);++i)

ll rd() {return ll(rand())<<30^ll(rand())<<16^rand();}

void exgcd(ll a,ll b,ll&x,ll&y){
  if(!b) x = 1 , y = 0;
  else exgcd(b,a%b,y,x) , y -= a/b*x;
}
ll inverse(ll a,ll b){ll x,y;exgcd(a,b,x,y);return (x%b+b)%b;}
ll Pow(ll x,ll t,ll P){ll r=1;for(;t;t>>=1,x=x*x%P)if(t&1)r=r*x%P;return r;}

const ll p = 998244353 , k = 30 , a = 1 , b = 0 , nA = 3456789 , nB = 2345678;
ll m , re_m;
pii Pm , G(323844906,499498872), PA;

ll add(ll a,ll b) {return (a+b)%p;}
ll mul(ll a,ll b) {return a*b%p;}
ll Legendre(ll a) { return Pow(a,(p-1)/2,p); }
pii mul(pii a,pii b,ll P) {
  return pii((int)add(mul(a.first,b.first),mul(mul(a.second,b.second),P)) ,
      (int)add(mul(a.first,b.second),mul(a.second,b.first)));
}
pii Pow(pii x,ll t,ll P){pii r(1,0);for(;t;t>>=1,x=mul(x,x,P))if(t&1)r=mul(r,x,P);return r;}

ll Cipolla(ll x) {
  while(true) {
    ll a = rd() % p;
    ll t = add(mul(a,a),p-x);
    if(Legendre(t)==p-1) {
      return Pow(pii(a,1),(p+1)/2,t).first;
    }
  }
}

void Embed() {
  cout << "Embedding start ~~~" << endl;
  rep(i,0,k) {
    ll x = m * k + i;
    ll y2 = add(x,mul(mul(x,x),x));
    cout << "x = " << x << " , y^2 = " << y2 << " , ";
    if(Legendre(y2)==p-1) {
      cout << y2 << " doesn't have quadratic residue." << endl;
    } else {
      ll y = Cipolla(y2);
      Pm = pii(x,y);
      cout << y2 << " has quadratic residue " << y << " , Pm = (" << Pm.first << " , " << Pm.second << ")." << endl;
      cout << "Embedding finished ~~~" << endl;
      return ;
    }
  }
  cout << "Oops! Plain can't be embedded!";
  exit(0);
}

const pii inf(-1,0);
pii Add(pii a,pii b) {
  if(a==inf) return b;
  if(b==inf) return a;
  if(a!=b&&a.first==b.first) return inf;
  int k=a==b?mul(add(mul(mul(a.first,a.first),3),1),inverse(mul(2,a.second),p)) :
    mul(add(b.second,p-a.second),inverse(add(b.first,p-a.first),p));
  int x = add(mul(k,k),add(p-a.first,p-b.first));
  int y = add(mul(k,add(a.first,p-x)),p-a.second);
  return pii(x,y);
}
pii Pow(pii x,ll t) {pii r=inf;for(;t;t>>=1,x=Add(x,x))if(t&1)r=Add(r,x);return r;}
pii inv(pii x) {if(x==inf)return inf;return pii(x.first,add(0,p-x.second));}

int main() {
  cout << "p is " << p << " , k = " << k << " , a = " << a << " , b = " << b << endl;
  cout << "Elliptic curve : y^2 = x^3 + x" << endl;
  PA = Pow(G , nA);
  cout << "nA = " << nA << " , G = (" << G.first << " , " << G.second << ") , PA = nA*G = (" << PA.first << " , " << PA.second << ")" << endl;
  cout << "Please input the cipher (< p / 30 ~= 33000000): " << endl;
  cin >> m;
  cout << "Plain is: " << m << endl;
  Embed();
  pii nBG = Pow(G,nB);
  pii Pm_nBPA = Add(Pm,Pow(PA,nB));
  cout << "Pm = (" << Pm.first << " , " << Pm.second << ")" << endl;
  cout << "Cipher is: { nB*G = (" << nBG.first << " , " << nBG.second << ") , Pm+nB*PA = (" << Pm_nBPA.first << " , " << Pm_nBPA.second << ")}" << endl;
  pii re_Pm = Add(Pm_nBPA,inv(Pow(nBG,nA)));
  cout << "After decrypt: re_Pm = Pm+nB*PA-nA*(nB*G) = (" << re_Pm.first << " , " << re_Pm.second << ")" << endl;
  cout << "re_m = " << re_Pm.first / k << endl;
  return 0;
}
