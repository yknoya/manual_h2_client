# manual_h2_client
manual_h2_client is a simple HTTP/2 client library which assembles the HTTP/2 frames manually.  
The purpose of the library is to debug a HTTP/2 server.

## Requirement
* C++17 or later
* [CMake](https://cmake.org/download/) 3.13 or later
* [OpenSSL](https://www.openssl.org/source/) 1.1.1g or later
* [GoogleTest](https://github.com/google/googletest) 1.10 or later (optional)
* [clang-format](https://clang.llvm.org/docs/ClangFormat.html) 10.0.1 or later (optional)
* [cpplint](https://github.com/cpplint/cpplint) 1.5.4 or later(optional)

## Usage
### How to build
```
$ cmake -B build \
  -DOPENSSL_ROOT_DIR=/path/to/openssl \
  -DBUILD_SHARED_LIBS=ON
$ cmake --build build
```

The default configuration builds only the library. Tests and sample code are
optional.

Enable and run unit tests with:
```
$ cmake -B build \
  -DOPENSSL_ROOT_DIR=/path/to/openssl \
  -DBUILD_SHARED_LIBS=ON \
  -DBUILD_TESTING=ON
$ cmake --build build
$ ctest --test-dir build --output-on-failure
```

Enable the sample program with:
```
$ cmake -B build \
  -DOPENSSL_ROOT_DIR=/path/to/openssl \
  -DBUILD_SHARED_LIBS=ON \
  -DMH2C_BUILD_SAMPLE=ON
$ cmake --build build
```

If the required tools are installed, you can also run formatting and linting:

```
$ cmake --build build --target format
$ cmake --build build --target lint
```

### How to use
See [the sample code](https://github.com/yknoya/manual_h2_client/blob/master/sample).  
You can execute the sample code in the following command after building with
`-DMH2C_BUILD_SAMPLE=ON`.
```
$ ./build/sample/h2_get/sample_h2_get
```
