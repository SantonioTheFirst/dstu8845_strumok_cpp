#include <iostream>
using namespace std;
#include "strumok.h"
#include <cstring>
#include <sstream>


dstu8845::dstu8845(uint64_t *key, uint8_t key_size, uint64_t *iv, bool verbose = false, bool big_endian = true){

  this->z_0 = 0;

  if (key_size == 32) {
      this->key_size = key_size;
      memcpy(this->iv, iv, 32);
      memcpy(this->key, key, 32);
      this->S[0] = this->key[3] ^ this->iv[0];
      this->S[1] = this->key[2];
      this->S[2] = this->key[1] ^ this->iv[1];
      this->S[3] = this->key[0] ^ this->iv[2];
      this->S[4] = this->key[3];
      this->S[5] = this->key[2] ^ this->iv[3];
      this->S[6] = ~this->key[1];
      this->S[7] = ~this->key[0];
      this->S[8] = this->key[3];
      this->S[9] = this->key[2];
      this->S[10] = ~this->key[1];
      this->S[11] = this->key[0];
      this->S[12] = this->key[3];
      this->S[13] = ~this->key[2];
      this->S[14] = this->key[1];
      this->S[15] = ~this->key[0];
  } else if (key_size == 64) {
      this->key_size = key_size;
      memcpy(this->iv, iv, 32);
      memcpy(this->key, key, 64);
      this->S[0] = this->key[7] ^ this->iv[0];
      this->S[1] = this->key[6];
      this->S[2] = this->key[5];
      this->S[3] = this->key[4] ^ this->iv[1];
      this->S[4] = this->key[3];
      this->S[5] = this->key[2] ^ this->iv[2];
      this->S[6] = this->key[1];
      this->S[7] = ~this->key[0];
      this->S[8] = this->key[4] ^ this->iv[3];
      this->S[9] = ~this->key[6];
      this->S[10] = this->key[5];
      this->S[11] = ~this->key[7];
      this->S[12] = this->key[3];
      this->S[13] = this->key[2];
      this->S[14] = ~this->key[1];
      this->S[15] = this->key[0];
  }
  this->r[0] = 0;
  this->r[1] = 0;

  if(verbose){
    for(uint8_t i = 0; i < 16; i++){
      cout << "S_" << (unsigned)i << ": ";
      stringstream ss;
      ss<< hex << this->S[i];
      string res ( ss.str() );

      cout << res << endl;
    }
    for(uint8_t i = 0; i < 2; i++){
      stringstream ss;
      ss<< hex << this->r[i];
      string res ( ss.str() );

      cout << "r_" << (unsigned)i << ": " << res << endl;
    }
    cout << endl;
  }

  static uint64_t outfrom_fsm, fsmtmp, tmp;
  for(uint8_t i = 0; i < 32; ++i){
    outfrom_fsm = (this->r[0] + this->S[15]) ^ this->r[1];
    tmp = this->a_mul(this->S[0]) ^ this->S[13] ^ this->ainv_mul(this->S[11]) ^ outfrom_fsm;
    fsmtmp = this->r[1] + this->S[13];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    for(uint8_t j = 0; j < 15; ++j){
      this->S[j] = this->S[j + 1];
    }
    this->S[15] = tmp;
    if(big_endian){
      this->z_0 |= (((this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15]))) >> (63 - i));
    }
    else{
      this->z_0 |= ((((this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15]))) & 1) << i);
    }


    if(verbose){
      for(uint8_t j = 0; j < 16; j++){
        cout << "S_" << (unsigned)j << ": ";
        stringstream ss;
        ss<< hex << this->S[j];
        string res ( ss.str() );

        cout << res << endl;
      }
      for(uint8_t j = 0; j < 2; j++){
        stringstream ss;
        ss<< hex << this->r[j];
        string res ( ss.str() );

        cout << "r_" << (unsigned)j << ": " << res << endl;
      }
      cout << endl;
    }
  }
}

uint64_t inline dstu8845::next_stream(){
    uint64_t fsmtmp, tmp, out_stream;

    tmp = this->a_mul(this->S[0]) ^ this->S[13] ^ this->ainv_mul(this->S[11]);
    fsmtmp = this->r[1] + this->S[13];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream = (this->r[0] + tmp) ^ this->r[1] ^ this->S[1];

    for(uint8_t i = 0; i < 15; ++i){
      this->S[i] = this->S[i + 1];
    }

    this->S[15] = tmp;

    return out_stream;
}
