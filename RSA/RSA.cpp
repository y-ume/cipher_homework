#include <iostream>
using namespace std;
typedef long long ll;

ll rd() {return ll(rand())<<30^ll(rand())<<16^rand();}

void exgcd(ll a,ll b,ll&x,ll&y){
  if(!b) x = 1 , y = 0;
  else exgcd(b,a%b,y,x) , y -= a/b*x;
}
ll inverse(ll a,ll b){ll x,y;exgcd(a,b,x,y);return (x%b+b)%b;}
ll Pow(ll x,ll t,ll P){ll r=1;for(;t;t>>=1,x=x*x%P)if(t&1)r=r*x%P;return r;}

const ll p = 17989 , q = 13553 , n = p * q , phi_n = (p - 1) * (q - 1);
ll e , d , m , M , re_m;

int main() {
  do e = rand() % phi_n; while(__gcd(e , phi_n) != 1);
  d = inverse(e , phi_n);
  cout << "p = " << p << " , q = " << q << " , n = " << n << " , phi_n = " << phi_n << endl;
  cout << "e = " << e << " , d = " << d << endl;
  cout << "Please input the cipher (< n): " << endl;
  cin >> m;
  cout << "Plain is: " << m << endl;
  cout << "Cipher is: " << (M = Pow(m , e , n)) << endl;
  cout << "After decrypt: " << (re_m = Pow(M , d , n)) << endl;
  return 0;
}
