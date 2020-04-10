// Copyright (c) 2013 The Wandoujia Authors. All rights reserved.
#ifndef WANDOUJIARSA_SIGNATURE_VERIFIER_H_
#define WANDOUJIARSA_SIGNATURE_VERIFIER_H_
#include <windows.h>
#include <vector>

typedef unsigned char      uint8;

namespace crypto {
class SignatureVerifier {
 public:
  SignatureVerifier();
  ~SignatureVerifier();

  bool VerifyInit(const uint8* signature,
      int signature_len,
      const uint8* public_key_info,
      int public_key_info_len);

  void VerifyUpdate(const uint8* data_part, int data_part_len);
  bool VerifyFinal();
 private:
  void Reset();
 private:
  std::vector<uint8> signature_;
  HCRYPTPROV provider_;
  HCRYPTHASH hash_object_;
  HCRYPTKEY public_key_;
};

}  // namespace crypto
#endif  // WANDOUJIARSA_SIGNATURE_VERIFIER_H_
