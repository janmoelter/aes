
# Advanced Encryption Standard (AES)

C++ code implementing the Advanced Encryption Standard (AES) block cipher as specified in the FIPS Publication 197 by the NIST.

## Overview

This code implements the Advanced Encryption Standard (AES) block cipher as described in the original FIPS Publication 197 by the NIST. Hence, only the bare block cipher is implemented. This might then be combined with one of the many block modes of operation to obtain a stream cipher.

Note: This implementation comes with no warranty and should not be used in mission critical scenarios.

## Build

```bash
mkdir build
cd build
cmake .. -DEXAMPLES=ON
cmake --build .
```

## References

"Announcing the ADVANCED ENCRYPTION STANDARD (AES)". Federal Information Processing Standards Publication 197. United States National Institute of Standards and Technology (NIST). DOI: [10.6028/NIST.FIPS.197](https://doi.org/10.6028/NIST.FIPS.197).
