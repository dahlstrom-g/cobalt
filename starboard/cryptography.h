// Copyright 2017 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Module Overview: Starboard Cryptography module
//
// Hardware-accelerated cryptography. Platforms should only implement
// this when there are hardware-accelerated cryptography facilities.
// Applications should use CPU-based algorithms if the cipher
// algorithm isn't supported in hardware.

#ifndef STARBOARD_CRYPTOGRAPHY_H_
#define STARBOARD_CRYPTOGRAPHY_H_

#include "starboard/configuration.h"
#include "starboard/export.h"
#include "starboard/types.h"

#if SB_API_VERSION >= 4

#ifdef __cplusplus
extern "C" {
#endif

// String literal for the AES symmetric block cipher.
// https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
#define kSbCryptographyAlgorithmAes "aes"

// String literal for the Triple DES (TDEA) symmetric block cipher.
// https://en.wikipedia.org/wiki/Triple_DES
#define kSbCryptographyAlgorithmTdea "tdea"

// The direction of a cryptographic transformation.
typedef enum SbCryptographyDirection {
  // Cryptographic transformations that encode/encrypt data into a
  // target format.
  kSbCryptographyDirectionEncode,

  // Cryptographic transformations that decode/decrypt data into
  // its original form.
  kSbCryptographyDirectionDecode,
} SbCryptographyDirection;

// The method of chaining encrypted blocks in a sequence.
// https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation
typedef enum SbCryptographyBlockCipherMode {
  // Cipher Block Chaining mode.
  kSbCryptographyBlockCipherModeCbc,

  // Cipher Feedback mode.
  kSbCryptographyBlockCipherModeCfb,

  // Counter mode: A nonce is combined with an increasing counter.
  kSbCryptographyBlockCipherModeCtr,

  // Ciphertext Stealing mode.
  kSbCryptographyBlockCipherModeCts,

  // Electronic Code Book mode: No chaining.
  kSbCryptographyBlockCipherModeEcb,

  // Output Feedback mode.
  kSbCryptographyBlockCipherModeOfb,
} SbCryptographyBlockCipherMode;

// Private structure representing a cryptographic transformer
// configuration and state.
typedef struct SbCryptographyTransformerPrivate
    SbCryptographyTransformerPrivate;

// A handle to a cryptographic transformer.
typedef SbCryptographyTransformerPrivate* SbCryptographyTransformer;

// Well-defined value for an invalid transformer.
#define kSbCryptographyInvalidTransformer ((SbCryptographyTransformer)NULL)

// Returns whether the given transformer handle is valid.
static SB_C_INLINE bool SbCryptographyIsTransformerValid(
    SbCryptographyTransformer transformer) {
  return transformer != kSbCryptographyInvalidTransformer;
}

// Creates an SbCryptographyTransformer with the given initialization
// data. It can then be used to transform a series of data blocks.
// Returns kSbCryptographyInvalidTransformer if the algorithm isn't
// supported, or if the parameters are not compatible with the
// algorithm.
//
// An SbCryptographyTransformer contains all state to start decrypting
// a sequence of cipher blocks according to the cipher block mode. It
// is not thread-safe, but implementations must allow different
// SbCryptographyTransformer instances to operate on different threads.
//
// All parameters must not be assumed to live longer than the call to
// this function. They must be copied by the implementation to be
// retained.
//
// For example, to decrypt AES-128-CTR:
//   SbCryptographyCreateTransformer(SB_CRYPTOGRAPHY_ALGORITHM_AES, 128,
//                                   kSbCryptographyDirectionDecode,
//                                   kSbCryptographyBlockCipherModeCtr,
//                                   ...);
//
// |algorithm|: A string that represents the cipher algorithm.
// |block_size_bits|: The block size variant of the algorithm to use, in bits.
// |direction|: The direction in which to transform the data.
// |mode|: The block cipher mode to use.
// |initialization_vector|: The Initialization Vector (IV) to use.
// |initialization_vector_size|: The size, in bytes, of the IV.
// |key|: The key to use for this transformation.
// |key_size|: The size, in bytes, of the key.
SB_EXPORT SbCryptographyTransformer
SbCryptographyCreateTransformer(const char* algorithm,
                                int block_size_bits,
                                SbCryptographyDirection direction,
                                SbCryptographyBlockCipherMode mode,
                                void* initialization_vector,
                                int initialization_vector_size,
                                const void* key,
                                int key_size);

// Destroys the given |transformer| instance.
SB_EXPORT void SbCryptographyDestroyTransformer(
    SbCryptographyTransformer transformer);

// Transforms one or more |block_size_bits|-sized blocks of |in_data|, with the
// given |transformer|, placing the result in |out_data|.  Returns the number of
// bytes that were written to |out_data|, unless there was an error, in which
// case it will return a negative number.
//
// This function determines success mainly based on whether the combination of
// |algorithm|, |direction|, and |block_size_bits| is supported and whether all
// the sizes passed in are sufficient for the selected parameters. In
// particular, this function cannot verify that the key and IV used were correct
// for the ciphertext, were it to be used in the decode direction. The caller
// must make that verification.
//
// |transformer|: A transformer initialized with an algorithm, IV, cipherkey,
// and so on.
// |in_data|: The data to be transformed.
// |in_data_size|: The size of the data to be transformed, in bytes. If not a
// multiple of the transformer's |block-size_bits|, padding may be applied
// according to the block cipher mode.
// |out_data|: A buffer where the transformed data should be placed.
// |out_data_size|: The capacity of the |out_data| buffer, in bytes. Must at
// least have capacity for |in_data_size|, rounded up to the next block, plus an
// extra block, for potential padding:
// `out_data_size >= ((in_data_size + 7) / 8 + 1) * (block_size_bits / 8)`
// This function will never write past the minimum required capacity.
SB_EXPORT int SbCryptographyTransformBlock(
    SbCryptographyTransformer transformer,
    const void* in_data,
    int in_data_size,
    void* out_data,
    int out_data_size);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // SB_API_VERSION >= 4

#endif  // STARBOARD_CRYPTOGRAPHY_H_
