/*
 * AES-128 algorithm implemented with in C++
 * @y-ume
 */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <bitset>
#include <algorithm>
using namespace std;
#define rep(i,a,b) for(int i=(a);i<(b);++i)
#define sz(x) ((int)(x).size())
typedef unsigned char uchar;
typedef unsigned int ui;

const int Nk = 4 , Nb = 4 , Nr = 10;
// ---------------- tables
ui Rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};
uchar S_Box[16][16] = {
  {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
  {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
  {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
  {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
  {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
  {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
  {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
  {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
  {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
  {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
  {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
  {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
  {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
  {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
  {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
  {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
};
uchar Inv_S_Box[16][16] = {
  {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
  {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
  {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
  {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
  {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
  {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
  {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
  {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
  {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
  {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
  {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
  {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
  {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
  {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
  {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
  {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
};
int ShiftRowsP[] = {0,1,2,3,5,6,7,4,10,11,8,9,15,12,13,14};
int InvShiftRowsP[] = {0,1,2,3,7,4,5,6,10,11,8,9,13,14,15,12};
uchar mul[16][256];
bool useCBC = false;
string S_key("#include<string>");
string plaintext("return this.message.split(\'\').reverse().join(\'\');") , ciphertext , re_plaintext;
string buff;
ui subkeys[Nb * (Nr + 1)] , key[Nk];

void GenerateTable() {
  // 8 4 3 1 0
  ui p = 0x0000011b;
  rep(i,0,16) rep(j,0,256) {
    ui t = 0;
    rep(k,0,4) if(i >> k & 1) t ^= j << k;
    for(int k=16;k>=8;--k) if(t >> k & 1)
      t ^= p << (k - 8);
    mul[i][j] = t;
  }
}

void ReadKey() {
  while(true) {
    cout << "input the key, 16 char needed." << endl;
    cout << "default key is \"" << S_key << "\", press enter to continue." << endl;
    getline(cin , buff);
    if(sz(buff) == 0);
    else if(sz(buff) == 8) S_key = buff;
    else continue;
    cout << "current key is :\n" << S_key << "\n----------------" << endl;
    return;
  }
}

ui RotWord(ui w) { return w >> 24 | w << 8;}

ui SubWord(ui w) {
  ui res=0;
  for(int i=0;i<32;i+=8)
    res |= ui(S_Box[w>>(i+4)&15][w>>i&15]) << i;
  return res;
}

void GenerateKeys() {
  rep(i,0,Nk)
    subkeys[i] = key[i] = ui(S_key[i<<2]) << 24 | ui(S_key[i<<2|1]) << 16 | ui(S_key[i<<2|2]) << 8 | ui(S_key[i<<2|3]);
  rep(i,Nk,Nb*(Nr+1)) {
    if(i % Nk == 0) subkeys[i] = subkeys[i - Nk] ^ SubWord(RotWord(subkeys[i - 1])) ^ Rcon[i / Nk - 1];
    else subkeys[i] = subkeys[i - Nk] ^ subkeys[i - 1];
  }
}

void SelectMode() {
  cout << "select mode :" << endl;
  cout << "          0 : CBC mode" << endl;
  cout << "  otherwise : ECB mode" << endl;
  getline(cin , buff);
  if(buff == "0") useCBC = true;
  cout << "current mode is :\n" << (useCBC ? "CBC" : "ECB") << " mode\n----------------" << endl;
}

void ReadPlain() {
  cout << "input the plain text." << endl;
  cout << "default text is \"" << plaintext << "\", press enter to continue." << endl;
  getline(cin , buff);
  if(sz(buff)) plaintext = buff;
  cout << "current plain text is :\n" << plaintext << endl;
  cout << "----------------" << endl;
}

void PKCS7Padding() {
  cout << "PKCS7Padding start!" << endl;
  plaintext += string(16 - sz(plaintext) % 16 , 16 - sz(plaintext) % 16);
  cout << "current plain text is :\n" << plaintext << endl;
  cout << "----------------" << endl;
}

void AddRoundKey(uchar*in,ui*key) {
  rep(i,0,4) {
    in[i] ^= key[i] >> 24;
    in[i+4] ^= key[i] >> 16 & 255;
    in[i+8] ^= key[i] >> 8 & 255;
    in[i+12] ^= key[i] & 255;
  }
}

void SubByte(uchar *in) {
  rep(i,0,16) in[i] = ui(S_Box[in[i]>>4][in[i]&15]);
}

void ShiftRows(uchar *in) {
  static uchar out[16];
  rep(i,4,16) out[i]=in[ShiftRowsP[i]];
  rep(i,4,16) in[i]=out[i];
}

void MixColumns(uchar *in) {
  static uchar a[4];
  rep(i,0,4) {
    rep(j,0,4) a[j]=in[i+(j<<2)];
    in[i]=mul[2][a[0]]^mul[3][a[1]]^a[2]^a[3];
    in[i+4]=a[0]^mul[2][a[1]]^mul[3][a[2]]^a[3];
    in[i+8]=a[0]^a[1]^mul[2][a[2]]^mul[3][a[3]];
    in[i+12]=mul[3][a[0]]^a[1]^a[2]^mul[2][a[3]];
  }
}

void Encrypt() {
  ciphertext = "";
  uchar last[16] = {0};
  for(int s=0;s<sz(plaintext);s+=16) {
    uchar in[16];
    rep(i,0,16) in[i]=plaintext[s+i]^last[i];
    AddRoundKey(in , subkeys);
    rep(r,1,Nr) {
      SubByte(in);
      ShiftRows(in);
      MixColumns(in);
      AddRoundKey(in , subkeys + r * Nb);
    }
    SubByte(in);
    ShiftRows(in);
    AddRoundKey(in , subkeys + Nr * Nb);
    if(useCBC) rep(i,0,16) last[i] = in[i];
    rep(i,0,16) ciphertext += char(in[i]);
  }
}

void InvSubByte(uchar *in) {
  rep(i,0,16) in[i] = ui(Inv_S_Box[in[i]>>4][in[i]&15]);
}

void InvShiftRows(uchar *in) {
  static uchar out[16];
  rep(i,4,16) out[i]=in[InvShiftRowsP[i]];
  rep(i,4,16) in[i]=out[i];
}

void InvMixColumns(uchar *in) {
  static uchar a[4];
  rep(i,0,4) {
    rep(j,0,4) a[j]=in[i+(j<<2)];
    in[i]=mul[14][a[0]]^mul[11][a[1]]^mul[13][a[2]]^mul[9][a[3]];
    in[i+4]=mul[9][a[0]]^mul[14][a[1]]^mul[11][a[2]]^mul[13][a[3]];
    in[i+8]=mul[13][a[0]]^mul[9][a[1]]^mul[14][a[2]]^mul[11][a[3]];
    in[i+12]=mul[11][a[0]]^mul[13][a[1]]^mul[9][a[2]]^mul[14][a[3]];
  }
}

void Decrypt() {
  re_plaintext = "";
  uchar last[16] = {0};
  for(int s=0;s<sz(ciphertext);s+=16) {
    uchar in[16];
    rep(i,0,16) in[i]=ciphertext[s+i];
    AddRoundKey(in , subkeys + Nr * Nb);
    rep(r,1,Nr) {
      InvShiftRows(in);
      InvSubByte(in);
      AddRoundKey(in , subkeys + (Nr - r) * Nb);
      InvMixColumns(in);
    }
    InvShiftRows(in);
    InvSubByte(in);
    AddRoundKey(in , subkeys);
    rep(i,0,16) in[i]^=last[i];
    if(useCBC) rep(i,0,16) last[i]=ciphertext[s+i];
    rep(i,0,16) re_plaintext += char(in[i]);
  }
}

int main() {
  GenerateTable();
  ReadKey();
  GenerateKeys();
  SelectMode();
  ReadPlain();
  PKCS7Padding();
  ofstream cipher("cipher.txt") , plain("plain.txt");
  cout << "Encrypt start!" << endl;
  Encrypt();
  cout << "cipher text is :" << endl;
  cout << ciphertext << endl;
  cout << "hex format is :" << endl;
  rep(i,0,sz(ciphertext)) cout << "0x" << hex << setw(2) << setfill('0') << (unsigned int)(unsigned char)(ciphertext[i]) << " ";
  cout << endl;
  cout << "better to check the cipher.txt." << endl;
  cipher << ciphertext << endl;
  cout << "----------------" << endl;
  cout << "Decrypt start!" << endl;
  Decrypt();
  cout << "plain text is :" << endl;
  cout << re_plaintext << endl;
  cout << "better to check the plain.txt" << endl;
  plain << re_plaintext << endl;
  cout << "----------------" << endl;
  cipher.close();
  plain.close();
  return 0;
}
