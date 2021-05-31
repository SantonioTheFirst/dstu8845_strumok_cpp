#include "strumok.cpp"
#include <ctime>
#include <cmath>
#include <vector>
// #include <iterator>
#include <parallel/algorithm>
#include <cstdlib>
#include <fstream>
#include <omp.h>
// #include <sstream>


void print_MDM_signature(uint64_t number){
  for(uint8_t i = 0; i < 64; i++){
    cout << ((number >> i) & 1);
  }
  cout << '\n';
}


void inline get_iv_from_bitset(uint64_t number, vector<uint16_t> bitset, uint64_t *out_iv){
  uint8_t bitset_size = bitset.size();
  for(uint8_t i = 0; i < bitset_size; i++){
    uint8_t c = 0;
    uint8_t temp = bitset[i];
    while(bitset[i] >= 64){
      bitset[i] -= 64;
      c++;
    }
    out_iv[c] |= ((number >> i) & 1) << (temp % 64);
  }
}


void inline get_iv_and_key_from_bitset(uint64_t number, vector<uint16_t> bitset, uint64_t *out_iv, uint64_t *out_key){
  uint16_t bitset_size = bitset.size();
  for(uint16_t i = 0; i < bitset_size; i++){
    if(bitset[i] > 255){
      bitset[i] -= 256;
      uint8_t c = 0;
      uint16_t temp = bitset[i];
      while(bitset[i] >= 64){
        bitset[i] -= 64;
        c++;
      }
      out_key[c] |= ((number >> i) & 1) << (temp % 64);
    }
    else{
      uint8_t c = 0;
      uint16_t temp = bitset[i];
      while(bitset[i] >= 64){
        bitset[i] -= 64;
        c++;
      }
      out_iv[c] |= ((number >> i) & 1) << (temp % 64);
    }
  }
}


uint8_t inline get_count_of_zeros(uint64_t number){
  uint8_t c = 0;
  while(number % 2 == 0){
    number /= 2;
    c++;
  }
  return c;
}


uint8_t inline MDM_test(vector<uint16_t> bitset, uint8_t key_size, bool verbose, bool allow_key, bool big_endian){
  uint64_t MDM_signature = 0;
  uint16_t bitset_size = bitset.size();
  // #pragma omp parallel for
  for(uint32_t i = 1; i < pow(2, bitset_size); i++){
    uint64_t iv[4] = {0};
    uint64_t *key = new uint64_t[key_size / 8];
    for(uint8_t j = 0; j < key_size / 8; j++){
      key[j] = 0;
    }
    if(!allow_key){
      get_iv_from_bitset((uint64_t) i, bitset, iv);
    }
    else{
      get_iv_and_key_from_bitset((uint64_t) i, bitset, iv, key);
    }
    dstu8845 cipher(key, key_size, iv, false, big_endian); ///////////////////
    uint64_t z = cipher.z_0;
    MDM_signature ^= z;
    // cout << "MDM_signature: " << MDM_signature;
    delete [] key;
  }
  if(verbose){
    print_MDM_signature(MDM_signature);
  }
  return get_count_of_zeros(MDM_signature);
}


/*vector<uint16_t>*/ uint16_t greedy_add_one_bit(vector<uint16_t> B, vector<uint16_t> S, ofstream& output, uint16_t key_size, bool big_endian){
  uint16_t best_bit;
  int8_t max = -1;
  // uint16_t B_size = B.size();
  for(uint16_t i = 0; i < B.size(); ++i){
    if(count(S.begin(), S.end(), B[i]) == 0){
      vector<uint16_t> temp = S;
      temp.emplace_back(B[i]);
      int8_t zr = MDM_test(temp, key_size, false, true, big_endian);
      if(zr > max){
        max = zr;
        best_bit = B[i];
      }
    }
    else{
      continue;
    }
  }
  // S.emplace_back(best_bit);
  cout << "Best bit: " << (unsigned)best_bit << " Zeros: " << (unsigned)max << '\n';
  output << (unsigned)best_bit << ',' << (unsigned)max << '\n';
  // return S;
  return best_bit;
}


vector<uint16_t> greedy_MDM_Stankovski(uint8_t n, vector<uint16_t> S, uint16_t key_size, bool big_endian){
  // vector<uint16_t> S;
  uint8_t S_size = S.size();
  vector<uint16_t> B;
  ofstream output;
  output.open("result.csv");

  if(key_size == 32){
    for(uint16_t i = 0; i < 512; i++){
      B.emplace_back(i);
    }
  }
  else{
    for(uint16_t i = 0; i < 768; i++){
      B.emplace_back(i);
    }
  }
  for(uint8_t i = 0; i < n - S_size; i++){
    S.emplace_back(greedy_add_one_bit(B, S, output, key_size, big_endian));
  }
  output.close();
  return S;
}


void print_2d(vector<vector<uint16_t>> v){
  for(uint16_t i = 0; i < v.size(); ++i){
    for(uint8_t j = 0; j < v[i].size(); ++j){
      cout << (unsigned)v[i][j] << ' ';
    }
    cout << '\n';
  }
}


void print_2d_with_pairs(vector<pair<vector<uint16_t>, int8_t>> v){
  for(uint32_t i = 0; i < v.size(); ++i){
    cout << "({ ";
    for(uint8_t j = 0; j < v[i].first.size(); ++j){
      cout << v[i].first[j] << ' ';
    }
    cout << "}, " << (unsigned)v[i].second << ')' << '\n';
  }
}


bool sortbysecdesc(const pair<vector<uint16_t>, int8_t> &a, const pair<vector<uint16_t>, int8_t> &b){
       return a.second > b.second;
}


vector<pair<vector<uint16_t>, int8_t>> find_best(vector<uint16_t> B, vector<uint16_t> C, uint8_t k, uint8_t n, uint8_t key_size, bool big_endian){
  vector<pair<vector<uint16_t>, int8_t>> S;
  int8_t * max_values = new int8_t[(unsigned)k];
  for(uint8_t i = 0; i < k; ++i){
    max_values[i] = -1;
  }
  for(uint16_t i = 0; i < B.size(); ++i){
    if(count(C.begin(), C.end(), B[i]) == 0){
      pair<vector<uint16_t>, int8_t> temp;
      temp.first.insert(temp.first.end(), C.begin(), C.end());
      temp.first.emplace_back(B[i]);
      temp.second = MDM_test(temp.first, key_size, false, true, big_endian);
      for(uint8_t j = 0; j < k; ++j){
        if(temp.second >= max_values[j]){
          max_values[j] = temp.second;
          S.emplace_back(temp);
          sort(S.begin(), S.end(), sortbysecdesc);
          if(S.size() > k){
            S.erase(S.begin() + k, S.end());
          }

          break;
        }
      }
    }
  }
  // cout << "Before delete k = " << (unsigned)k << '\n';
  delete[] max_values;
  return S;
}


void slightly_greedy(uint8_t key_size, uint8_t m, vector<uint8_t> k, vector<uint8_t> n, vector<double> alpha, bool big_endian, ofstream &output, ofstream &bitset){
  // cout << "BEFORE DEF B\n";
  vector<uint16_t> B;
  for(uint16_t i = 0; i < 256 + key_size * 8; ++i){
    B.emplace_back(i);
  }
  // cout << "START\n";
  vector<pair<vector<uint16_t>, int8_t>> S;
  S.emplace_back();
  for(uint8_t i = 0; i < m; ++i){
    cout << "Iteration: " << (unsigned)(i + 1) << '\n';
    vector<pair<vector<uint16_t>, int8_t>> temp;


    uint32_t S_size = S.size();
    // cout << "PARALLEL FOR LOOP\n";
    vector<pair<vector<uint16_t>, int8_t>> L_c;
    uint32_t j;
    // #pragma omp parallel for private(j, L_c) shared(i, S_size, temp, S, k, n, key_size, big_endian)
    for(j = 0; j < S_size; ++j){
      // if(j % 1000 == 0){
      //   cout << "J = " << (unsigned)j << " PROCESS_ID = " << omp_get_thread_num() << '\n';
      // }
      L_c  = find_best(B, S[j].first, k[i], n[i], key_size, big_endian);
      // #pragma omp critical
      // {
        temp.insert(temp.end(), L_c.begin(), L_c.end());
      // }
    }


    // #pragma omp parallel for
    // for(uint32_t j = 0; j < S.size(); ++j){
    //   vector<vector<pair<vector<uint16_t>, int8_t>>> L_c;
    //   L_c.emplace_back(find_best(B, S[j].first, k[i], n[i], key_size, big_endian));
    //   cout << "L_C size = " << (unsigned)L_c.size() << '\n';
    //   for(uint32_t t = 0; t < L_c.size(); ++t){
    //     for(uint32_t r = 0; r < L_c[t].size(); ++r){
    //       temp.emplace_back(L_c[t][r]);
    //     }
    //   }
    //
    // }
    // cout << "BEFORE clear\n";
    S.clear();
    S.insert(S.end(), temp.begin(), temp.end());
    // temp.clear();
    // clock_t begin = clock();
    sort(S.begin(), S.end(), sortbysecdesc);
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // cout << "Sort: " << time_spent << " seconds\n";
    uint32_t max_size = (uint32_t)((alpha[i] * S.size()) + 0.5);
    cout << "Max size: " << (unsigned)max_size << '\n';
    if(max_size < 50){
      max_size = 50;
    }
    cout << "Number of bitsets: " << (unsigned)S.size() << '\n';
    if(S.size() > max_size){
      // cout << "ERASE" << '\n';
      S.erase(S.begin() + max_size, S.end());
    }
    cout << "Number of bitsets after erase: " << (unsigned)S.size() << '\n';
    output << (unsigned)S[0].second << '\n';
    sort(S[0].first.begin(), S[0].first.end());
    bitset << "({ ";
    for(uint8_t j = 0; j < S[0].first.size(); ++j){
      bitset << S[0].first[j] << ' ';
    }
    bitset << "}, " << (unsigned)S[0].second << ')' << '\n';
  }
  // print_2d_with_pairs(S);
}


int main(){

    clock_t begin = clock();
    vector<uint8_t> k =    {50, 50, 5, 5, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    vector<uint8_t> n =    {4, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
    vector<double> alpha = {1, 0.1, 0.3, 0.2, 0.2, 0.2, 0.2, 0.1, 0.2, 0.3, 0.3, 0.3, 0.2, 0.2, 0.3, 0.2, 0.2, 0.2, 0.2, 0.1};
    unsigned short m;
    cout << "Max size: ";
    cin >> m;
    cout << '\n';

    // vector<uint16_t> test = {307, 402, 440, 308, 392, 442, 432, 433, 411, 410};
    // sort(test.begin(), test.end());
    // cout << (unsigned)MDM_test(test, 64, false, true, big_endian) << '\n';
    bool big_endian = false;
    vector<vector<uint8_t>> ks;
    vector<vector<double>> alphas;
    uint8_t key_size[2] = {64, 32};
    ks.resize(4);
    alphas.resize(4);
    ks[0] = k;
    ks[1] = {50, 30, 10, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    ks[2] = {50, 30, 5, 5, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    ks[3] = {200, 150, 2, 2, 2, 2, 2, 3, 3, 2, 2, 2, 2, 2, 2, 2};
    alphas[0] = alpha;
    alphas[1] = {1, 1, 0.3, 0.01, 0.3, 0.5, 0.5, 0.5, 0.1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
    alphas[2] = {1, 1, 0.5, 0.5, 0.2, 0.2, 0.15, 0.01, 0.5, 0.5, 0.3, 0.3, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.1};
    alphas[3] = {1, 1, 0.5, 0.5, 0.5, 0.01, 0.1, 0.3, 0.3, 0.3, 1, 0.5, 0.5, 0.1, 0.5, 0.5};
    for(uint8_t i = 0; i < 1; ++i){
      for(uint8_t j = 3; j < 4; ++j){
        ofstream output;
        output.open("results/result_slightly_greedy_" + to_string(key_size[i]) + '_' + to_string(j) + ".txt");
        ofstream bitset;
        bitset.open("results/slightly_greedy_bitset_" + to_string(key_size[i]) + '_' + to_string(j) + ".txt");

        slightly_greedy(key_size[i], m, ks[j], n, alphas[j], big_endian, output, bitset);

        output.close();
        bitset.close();
      }
    }
    // cout << "BEFORE ofstream\n";
    // ofstream output;
    // output.open("results/result_slightly_greedy_3.txt");
    // ofstream bitset;
    // bitset.open("results/slightly_greedy_bitset_3.txt");
    // // cout << "BEFORE CALL\n";
    // slightly_greedy(64, m, ks[3], n, alphas[3], big_endian, output, bitset);

    // output.close();
    // bitset.close();

    // slightly_greedy(64, m, k, n, alpha, big_endian);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << "Time: " << time_spent << " seconds";


    // greedy_MDM_Stankovski

    // clock_t begin = clock();
    // unsigned short n;
    // uint8_t key_size = 64;
    // bool big_endian = false;//true;//false;
    // vector<uint16_t> bitset = {};// {296, 297, 303, 304, 305, 308, 310, 317, 318, 319, 384, 386, 397, 398, 401, 414, 418, 422, 435, 446};
    // cout << "Bitset size now: " << bitset.size() << '\n';
    // cout << "Bitset size: ";
    // cin >> n;
    // // cout << (unsigned)n << '\n';
    // vector<uint16_t> result = greedy_MDM_Stankovski((uint8_t)n, bitset, key_size, big_endian);
    // // vector<uint16_t> result = {14};
    // cout << (unsigned)MDM_test(result, key_size, true, true, big_endian) << '\n';
    // sort(result.begin(), result.end());
    // for(uint8_t i = 0; i < result.size(); i++){
    //   cout << (unsigned)result[i] << ' ';
    // }
    //
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // cout << "Time: " << time_spent << " seconds";

    //greedy_MDM_Stankovski end

    cout << '\n';

    return 0;
}
