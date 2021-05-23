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
        ss<< hex << this->S[i]; // int decimal_value
        string res ( ss.str() );

        cout << res << endl;
      }
      for(uint8_t i = 0; i < 2; i++){
        stringstream ss;
        ss<< hex << this->r[i]; // int decimal_value
        string res ( ss.str() );

        cout << "r_" << (unsigned)i << ": " << res << endl;
      }
      cout << endl;
    }

    static uint64_t outfrom_fsm, fsmtmp, tmp;
    for(uint8_t i = 0; i < 64; ++i){
      outfrom_fsm = (this->r[0] + this->S[15]) ^ this->r[1];
      // cout << "outfrom_fsm " << (long unsigned) outfrom_fsm << endl;
      tmp = this->a_mul(this->S[0]) ^ this->S[13] ^ this->ainv_mul(this->S[11]) ^ outfrom_fsm;
      // cout << (unsigned)i << ": " << (long unsigned)tmp << endl;
      // cout << "tmp = " << (unsigned)tmp << endl;
      fsmtmp = this->r[1] + this->S[13];
      this->r[1] = this->T(this->r[0]);
      this->r[0] = fsmtmp;
      for(uint8_t j = 0; j < 15; ++j){
        // cout << "j = " << (unsigned)j << ' ' << (unsigned long)this->S[j] << ' ';
        this->S[j] = this->S[j + 1];
        // cout << (unsigned long)this->S[j] << endl;
      }
      // cout << endl;
      this->S[15] = tmp;

      // this->z_0 = (this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15]));// >> 63;

      // this->z_0 = (this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15]));// >> 63; //this->z_0 |= (this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15])) << i;
      if(big_endian){
        this->z_0 |= (((this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15]))) >> (63 - i));// big_endian// MDM_signature_64bit_all_rounds
      }
      else{
        this->z_0 |= ((((this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15]))) & 1) << i);
      }


      if(verbose){
        for(uint8_t j = 0; j < 16; j++){
          cout << "S_" << (unsigned)j << ": ";
          stringstream ss;
          ss<< hex << this->S[j]; // int decimal_value
          string res ( ss.str() );

          cout << res << endl;
        }
        for(uint8_t j = 0; j < 2; j++){
          stringstream ss;
          ss<< hex << this->r[j]; // int decimal_value
          string res ( ss.str() );

          cout << "r_" << (unsigned)j << ": " << res << endl;
        }
        cout << endl;

        // stringstream ss;
        // ss << hex << this->z_0;
        // string res(ss.str());
        // cout << res << endl;
      }


      // cout << this->S[1] << endl;

    }
//b18832b3


    // stringstream ss;
    // ss<< hex << (this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15])); // int decimal_value
    // string res ( ss.str() );
    //
    // cout << res << endl;

    // cout << endl << (this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15])) << endl;




  //   outfrom_fsm = (this->r[0] + this->S[0]) ^ this->r[1];
  //   this->S[1] = this->a_mul(this->S[1]) ^ this->S[14] ^ this->ainv_mul(this->S[12]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[14];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[1] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[1]) ^ this->r[1] ^ this->S[2]) & 1) << j;
  //   this->z_0 |= ((this->S[1] ^ this->S[2]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[1]) ^ this->r[1];
  //   this->S[2] = this->a_mul(this->S[2]) ^ this->S[15] ^ this->ainv_mul(this->S[13]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[15];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[2] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[2]) ^ this->r[1] ^ this->S[3]) & 1) << j;
  //   this->z_0 |= ((this->S[2] ^ this->S[3]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[2]) ^ this->r[1];
  //   this->S[3] = this->a_mul(this->S[3]) ^ this->S[0] ^ this->ainv_mul(this->S[14]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[0];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[3] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[3]) ^ this->r[1] ^ this->S[4]) & 1) << j;
  //   this->z_0 |= ((this->S[3] ^ this->S[4]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[3]) ^ this->r[1];
  //   this->S[4] = this->a_mul(this->S[4]) ^ this->S[1] ^ this->ainv_mul(this->S[15]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[1];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[4] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[4]) ^ this->r[1] ^ this->S[5]) & 1) << j;
  //   this->z_0 |= ((this->S[4] ^ this->S[5]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[4]) ^ this->r[1];
  //   this->S[5] = this->a_mul(this->S[5]) ^ this->S[2] ^ this->ainv_mul(this->S[0]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[2];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[5] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[5]) ^ this->r[1] ^ this->S[6]) & 1) << j;
  //   this->z_0 |= ((this->S[5] ^ this->S[6]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[5]) ^ this->r[1];
  //   this->S[6] = this->a_mul(this->S[6]) ^ this->S[3] ^ this->ainv_mul(this->S[1]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[3];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[6] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[6]) ^ this->r[1] ^ this->S[7]) & 1) << j;
  //   this->z_0 |= ((this->S[6] ^ this->S[7]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[6]) ^ this->r[1];
  //   this->S[7] = this->a_mul(this->S[7]) ^ this->S[4] ^ this->ainv_mul(this->S[2]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[4];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[7] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[7]) ^ this->r[1] ^ this->S[8]) & 1) << j;
  //   this->z_0 |= ((this->S[7] ^ this->S[8]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[7]) ^ this->r[1];
  //   this->S[8] = this->a_mul(this->S[8]) ^ this->S[5] ^ this->ainv_mul(this->S[3]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[5];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[8] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[8]) ^ this->r[1] ^ this->S[9]) & 1) << j;
  //   this->z_0 |= ((this->S[8] ^ this->S[9]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[8]) ^ this->r[1];
  //   this->S[9] = this->a_mul(this->S[9]) ^ this->S[6] ^ this->ainv_mul(this->S[4]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[6];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[9] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[9]) ^ this->r[1] ^ this->S[10]) & 1) << j;
  //   this->z_0 |= ((this->S[9] ^ this->S[10]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[9]) ^ this->r[1];
  //   this->S[10] = this->a_mul(this->S[10]) ^ this->S[7] ^ this->ainv_mul(this->S[5]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[7];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[10] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[10]) ^ this->r[1] ^ this->S[11]) & 1) << j;
  //   this->z_0 |= ((this->S[10] ^ this->S[11]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[10]) ^ this->r[1];
  //   this->S[11] = this->a_mul(this->S[11]) ^ this->S[8] ^ this->ainv_mul(this->S[6]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[8];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[11] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[11]) ^ this->r[1] ^ this->S[12]) & 1) << j;
  //   this->z_0 |= ((this->S[11] ^ this->S[12]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[11]) ^ this->r[1];
  //   this->S[12] = this->a_mul(this->S[12]) ^ this->S[9] ^ this->ainv_mul(this->S[7]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[9];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[12] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[12]) ^ this->r[1] ^ this->S[13]) & 1) << j;
  //   this->z_0 |= ((this->S[12] ^ this->S[13]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[12]) ^ this->r[1];
  //   this->S[13] = this->a_mul(this->S[13]) ^ this->S[10] ^ this->ainv_mul(this->S[8]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[10];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[13] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[13]) ^ this->r[1] ^ this->S[14]) & 1) << j;
  //   this->z_0 |= ((this->S[13] ^ this->S[14]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[13]) ^ this->r[1];
  //   this->S[14] = this->a_mul(this->S[14]) ^ this->S[11] ^ this->ainv_mul(this->S[9]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[11];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[14] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[14]) ^ this->r[1] ^ this->S[15]) & 1) << j;
  //   this->z_0 |= ((this->S[14] ^ this->S[15]) & 1) << j;
  //   j++;
  //
  //   outfrom_fsm = (this->r[0] + this->S[14]) ^ this->r[1];
  //   this->S[15] = this->a_mul(this->S[15]) ^ this->S[12] ^ this->ainv_mul(this->S[10]) ^ outfrom_fsm;
  //   fsmtmp = this->r[1] + this->S[12];
  //   this->r[1] = this->T(this->r[0]);
  //   this->r[0] = fsmtmp;
  //   // this->z_0 |= (this->S[15] & 1) << j;
  //   // this->z_0 |= (((this->r[0] + this->S[15]) ^ this->r[1] ^ this->S[0]) & 1) << j;
  //   this->z_0 |= ((this->S[15] ^ this->S[0]) & 1) << j;
  //   j++;
  // }
}


bool inline dstu8845::get_first_keystream_bit(){
  uint64_t fsmtmp;

  uint64_t S_0 = this->a_mul(this->S[0]) ^ this->S[13] ^ this->ainv_mul(this->S[11]);
  fsmtmp = this->r[1] + this->S[13];
  uint64_t r_1 = this->T(this->r[0]);
  uint64_t r_0 = fsmtmp;
  return ((r_0 + S_0) ^ r_1 ^ this->S[1]) & 1;
}


// void inline dstu8845::next_stream_test(){
//     static uint64_t outfrom_fsm, fsmtmp, tmp;
//     outfrom_fsm = (this->r[0] + this->S[0]) ^ this->r[1];
//     tmp = this->a_mul(this->S[0]) ^ this->S[13] ^ this->ainv_mul(this->S[11]);
//     fsmtmp = this->r[1] + this->S[13];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     // out_stream[0] =  ^ this->S[1];
//     for(uint8_t i = 0; i < 15; i++){
//       this->S[i] = this->S[i + 1];
//     }
//     this->S[15] = tmp;
//
//     stringstream ss;
//     ss << hex << (unsigned long)(outfrom_fsm ^ this->S[0]);
//     string res(ss.str());
//     cout << res << endl;
// }
//
//
// void dstu8845::dstu8845_crypt(const uint8_t *in, size_t inl, uint8_t *out){
//   uint8_t i;
//   uint64_t keystream[16];
//
//   for (; inl >= 128; inl -= 128, in += 128, out += 128) {
//       this->next_stream_full_crypt((uint64_t *)in, (uint64_t *)out);
//   }
//
//   if (inl > 0) {
//       this->next_stream(keystream);
//
//       for (i = 0; i < inl; i++) {
//           out[i] = in[i] ^ ((uint8_t *)keystream)[i];
//       }
//   }
// }


uint64_t inline dstu8845::next_stream(){
    uint64_t fsmtmp, tmp, out_stream;

    // cout << "NEXT STREAM" << endl;

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


    // for(uint8_t j = 0; j < 16; j++){
    //   cout << "S_" << (unsigned)j << ": ";
    //   stringstream ss;
    //   ss<< hex << this->S[j]; // int decimal_value
    //   string res ( ss.str() );
    //
    //   cout << res << endl;
    // }
    // for(uint8_t j = 0; j < 2; j++){
    //   stringstream ss;
    //   ss<< hex << this->r[j]; // int decimal_value
    //   string res ( ss.str() );
    //
    //   cout << "r_" << (unsigned)j << ": " << res << endl;
    // }
    //
    // stringstream ss;
    // ss<< hex << (unsigned long)out_stream[0]; // int decimal_value
    // string res ( ss.str() );

    // cout << "Z" << ": " << res << endl;

    // this->S[0] = this->a_mul(this->S[0]) ^ this->S[13] ^ this->ainv_mul(this->S[11]);
    // fsmtmp = this->r[1] + this->S[13];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[0] = (this->r[0] + this->S[0]) ^ this->r[1] ^ this->S[1];
    //
    // this->S[1] = this->a_mul(this->S[1]) ^ this->S[14] ^ this->ainv_mul(this->S[12]);
    // fsmtmp = this->r[1] + this->S[14];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[1] = (this->r[0] + this->S[1]) ^ this->r[1] ^ this->S[2];
    //
    // this->S[2] = this->a_mul(this->S[2]) ^ this->S[15] ^ this->ainv_mul(this->S[13]);
    // fsmtmp = this->r[1] + this->S[15];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[2] = (this->r[0] + this->S[2]) ^ this->r[1] ^ this->S[3];
    //
    // this->S[3] = this->a_mul(this->S[3]) ^ this->S[0] ^ this->ainv_mul(this->S[14]);
    // fsmtmp = this->r[1] + this->S[0];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[3] = (this->r[0] + this->S[3]) ^ this->r[1] ^ this->S[4];
    //
    // this->S[4] = this->a_mul(this->S[4]) ^ this->S[1] ^ this->ainv_mul(this->S[15]);
    // fsmtmp = this->r[1] + this->S[1];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[4] = (this->r[0] + this->S[4]) ^ this->r[1] ^ this->S[5];
    //
    // this->S[5] = this->a_mul(this->S[5]) ^ this->S[2] ^ this->ainv_mul(this->S[0]);
    // fsmtmp = this->r[1] + this->S[2];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[5] = (this->r[0] + this->S[5]) ^ this->r[1] ^ this->S[6];
    //
    // this->S[6] = this->a_mul(this->S[6]) ^ this->S[3] ^ this->ainv_mul(this->S[1]);
    // fsmtmp = this->r[1] + this->S[3];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[6] = (this->r[0] + this->S[6]) ^ this->r[1] ^ this->S[7];
    //
    // this->S[7] = this->a_mul(this->S[7]) ^ this->S[4] ^ this->ainv_mul(this->S[2]);
    // fsmtmp = this->r[1] + this->S[4];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[7] = (this->r[0] + this->S[7]) ^ this->r[1] ^ this->S[8];
    //
    // this->S[8] = this->a_mul(this->S[8]) ^ this->S[5] ^ this->ainv_mul(this->S[3]);
    // fsmtmp = this->r[1] + this->S[5];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[8] = (this->r[0] + this->S[8]) ^ this->r[1] ^ this->S[9];
    //
    // this->S[9] = this->a_mul(this->S[9]) ^ this->S[6] ^ this->ainv_mul(this->S[4]);
    // fsmtmp = this->r[1] + this->S[6];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[9] = (this->r[0] + this->S[9]) ^ this->r[1] ^ this->S[10];
    //
    // this->S[10] = this->a_mul(this->S[10]) ^ this->S[7] ^ this->ainv_mul(this->S[5]);
    // fsmtmp = this->r[1] + this->S[7];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[10] = (this->r[0] + this->S[10]) ^ this->r[1] ^ this->S[11];
    //
    // this->S[11] = this->a_mul(this->S[11]) ^ this->S[8] ^ this->ainv_mul(this->S[6]);
    // fsmtmp = this->r[1] + this->S[8];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[11] = (this->r[0] + this->S[11]) ^ this->r[1] ^ this->S[12];
    //
    // this->S[12] = this->a_mul(this->S[12]) ^ this->S[9] ^ this->ainv_mul(this->S[7]);
    // fsmtmp = this->r[1] + this->S[9];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[12] = (this->r[0] + this->S[12]) ^ this->r[1] ^ this->S[13];
    //
    // this->S[13] = this->a_mul(this->S[13]) ^ this->S[10] ^ this->ainv_mul(this->S[8]);
    // fsmtmp = this->r[1] + this->S[10];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[13] = (this->r[0] + this->S[13]) ^ this->r[1] ^ this->S[14];
    //
    // this->S[14] = this->a_mul(this->S[14]) ^ this->S[11] ^ this->ainv_mul(this->S[9]);
    // fsmtmp = this->r[1] + this->S[11];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[14] = (this->r[0] + this->S[14]) ^ this->r[1] ^ this->S[15];
    //
    // this->S[15] = this->a_mul(this->S[15]) ^ this->S[12] ^ this->ainv_mul(this->S[10]);
    // fsmtmp = this->r[1] + this->S[12];
    // this->r[1] = this->T(this->r[0]);
    // this->r[0] = fsmtmp;
    // out_stream[15] = (this->r[0] + this->S[15]) ^ this->r[1] ^ this->S[0];

}

// void inline dstu8845::next_stream_full_crypt(uint64_t *in, uint64_t *out){
//     uint64_t fsmtmp;
//
//     cout << "NEXT STREAM FULL CRYPT" << endl;
//
//     this->S[0] = this->a_mul(this->S[0]) ^ this->S[13] ^ this->ainv_mul(this->S[11]);
//     fsmtmp = this->r[1] + this->S[13];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[0] = in[0] ^ (this->r[0] + this->S[0]) ^ this->r[1] ^ this->S[1];
//
//     this->S[1] = this->a_mul(this->S[1]) ^ this->S[14] ^ this->ainv_mul(this->S[12]);
//     fsmtmp = this->r[1] + this->S[14];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[1] = in[1] ^ (this->r[0] + this->S[1]) ^ this->r[1] ^ this->S[2];
//
//     this->S[2] = this->a_mul(this->S[2]) ^ this->S[15] ^ this->ainv_mul(this->S[13]);
//     fsmtmp = this->r[1] + this->S[15];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[2] = in[2] ^ (this->r[0] + this->S[2]) ^ this->r[1] ^ this->S[3];
//
//     this->S[3] = this->a_mul(this->S[3]) ^ this->S[0] ^ this->ainv_mul(this->S[14]);
//     fsmtmp = this->r[1] + this->S[0];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[3] = in[3] ^ (this->r[0] + this->S[3]) ^ this->r[1] ^ this->S[4];
//
//     this->S[4] = this->a_mul(this->S[4]) ^ this->S[1] ^ this->ainv_mul(this->S[15]);
//     fsmtmp = this->r[1] + this->S[1];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[4] = in[4] ^ (this->r[0] + this->S[4]) ^ this->r[1] ^ this->S[5];
//
//     this->S[5] = this->a_mul(this->S[5]) ^ this->S[2] ^ this->ainv_mul(this->S[0]);
//     fsmtmp = this->r[1] + this->S[2];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[5] = in[5] ^(this->r[0] + this->S[5]) ^ this->r[1] ^ this->S[6];
//
//     this->S[6] = this->a_mul(this->S[6]) ^ this->S[3] ^ this->ainv_mul(this->S[1]);
//     fsmtmp = this->r[1] + this->S[3];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[6] = in[6] ^(this->r[0] + this->S[6]) ^ this->r[1] ^ this->S[7];
//
//     this->S[7] = this->a_mul(this->S[7]) ^ this->S[4] ^ this->ainv_mul(this->S[2]);
//     fsmtmp = this->r[1] + this->S[4];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[7] = in[7] ^(this->r[0] + this->S[7]) ^ this->r[1] ^ this->S[8];
//
//     this->S[8] = this->a_mul(this->S[8]) ^ this->S[5] ^ this->ainv_mul(this->S[3]);
//     fsmtmp = this->r[1] + this->S[5];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[8] = in[8] ^(this->r[0] + this->S[8]) ^ this->r[1] ^ this->S[9];
//
//     this->S[9] = this->a_mul(this->S[9]) ^ this->S[6] ^ this->ainv_mul(this->S[4]);
//     fsmtmp = this->r[1] + this->S[6];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[9] = in[9] ^(this->r[0] + this->S[9]) ^ this->r[1] ^ this->S[10];
//
//     this->S[10] = this->a_mul(this->S[10]) ^ this->S[7] ^ this->ainv_mul(this->S[5]);
//     fsmtmp = this->r[1] + this->S[7];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[10] = in[10] ^(this->r[0] + this->S[10]) ^ this->r[1] ^ this->S[11];
//
//     this->S[11] = this->a_mul(this->S[11]) ^ this->S[8] ^ this->ainv_mul(this->S[6]);
//     fsmtmp = this->r[1] + this->S[8];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[11] = in[11] ^ (this->r[0] + this->S[11]) ^ this->r[1] ^ this->S[12];
//
//     this->S[12] = this->a_mul(this->S[12]) ^ this->S[9] ^ this->ainv_mul(this->S[7]);
//     fsmtmp = this->r[1] + this->S[9];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[12] = in[12] ^ (this->r[0] + this->S[12]) ^ this->r[1] ^ this->S[13];
//
//     this->S[13] = this->a_mul(this->S[13]) ^ this->S[10] ^ this->ainv_mul(this->S[8]);
//     fsmtmp = this->r[1] + this->S[10];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[13] = in[13] ^ (this->r[0] + this->S[13]) ^ this->r[1] ^ this->S[14];
//
//     this->S[14] = this->a_mul(this->S[14]) ^ this->S[11] ^ this->ainv_mul(this->S[9]);
//     fsmtmp = this->r[1] + this->S[11];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[14] = in[14] ^ (this->r[0] + this->S[14]) ^ this->r[1] ^ this->S[15];
//
//     this->S[15] = this->a_mul(this->S[15]) ^ this->S[12] ^ this->ainv_mul(this->S[10]);
//     fsmtmp = this->r[1] + this->S[12];
//     this->r[1] = this->T(this->r[0]);
//     this->r[0] = fsmtmp;
//     out[15] = in[15] ^ (this->r[0] + this->S[15]) ^ this->r[1] ^ this->S[0];
//
// }
