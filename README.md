# manual_h2_client
manual_h2_client is a simple HTTP/2 client library which assembles the HTTP/2 frames manually.  
The purpose of the library is to debug a HTTP/2 server.

## Requirement
* C++17 or later
* [CMake](https://cmake.org/download/) 3.13 or later
* [OpenSSL](https://www.openssl.org/source/) 1.1.1g or later
* [GoogleTest](https://github.com/google/googletest) 1.10 or later (optional)
* [clang-format](https://clang.llvm.org/docs/ClangFormat.html) 10.0.1 or later (optional)

## Usage
### How to build
Build only the library:
```
$ cmake -B build \
  -DOPENSSL_ROOT_DIR=/path/to/openssl \
  -DBUILD_SHARED_LIBS=ON
$ cmake --build build
```

The default configuration builds only the library.

Build with unit tests enabled:
```
$ cmake -B build \
  -DOPENSSL_ROOT_DIR=/path/to/openssl \
  -DBUILD_SHARED_LIBS=ON \
  -DBUILD_TESTING=ON
$ cmake --build build
$ ctest --test-dir build --output-on-failure
```

Build with sample executables enabled:
```
$ cmake -B build \
  -DOPENSSL_ROOT_DIR=/path/to/openssl \
  -DBUILD_SHARED_LIBS=ON \
  -DMH2C_BUILD_SAMPLE=ON
$ cmake --build build
```

If the required tools are installed, you can also run formatting:

```
$ cmake --build build --target format
```

### How to use
See [the sample code](https://github.com/yknoya/manual_h2_client/blob/master/sample).  
You can execute the sample code in the following command after building with
`-DMH2C_BUILD_SAMPLE=ON`.
```
$ ./build/sample/h2_get/sample_h2_get \
  --host nghttp2.org \
  --path /httpbin/headers
$ ./build/sample/h2_post/sample_h2_post \
  --host nghttp2.org \
  --path /httpbin/post
$ ./build/sample/h2_post_chunked_data/sample_h2_post_chunked_data \
  --host nghttp2.org \
  --path /httpbin/post
$ ./build/sample/h2_custom_headers/sample_h2_custom_headers \
  --host nghttp2.org \
  --path /httpbin/headers
$ ./build/sample/h2_ping/sample_h2_ping \
  --host nghttp2.org
$ ./build/sample/h2_window_update/sample_h2_window_update \
  --host nghttp2.org \
  --path /httpbin/headers
```

Shared sample options:

* `--host HOST`: target host name (default: `nghttp2.org`)
* `--port PORT`: target port (default: `443`)
* `--authority AUTHORITY`: `:authority` pseudo-header value (default: host)
* `--path PATH`: request path for request/response samples
* `--insecure`: disable TLS certificate verification for manual debugging
* `--help`: print usage

### Sample smoke check
For a quick end-to-end sample validation, build `sample_h2_get` and run the
smoke-check helper:

```
$ cmake -B build \
  -DBUILD_SHARED_LIBS=ON \
  -DMH2C_BUILD_SAMPLE=ON
$ cmake --build build --target sample_h2_get
$ ./sample/h2_get/smoke_check.sh
```

The helper executes `sample_h2_get` directly and prints the exact command
before running it. You can override the target endpoint without changing the
sample itself:

```
$ MH2C_SMOKE_HOST=example.com \
  MH2C_SMOKE_PATH=/ \
  ./sample/h2_get/smoke_check.sh
```

Available environment variables:

* `MH2C_SMOKE_HOST`
* `MH2C_SMOKE_PORT`
* `MH2C_SMOKE_PATH`
* `MH2C_SMOKE_AUTHORITY`
* `MH2C_SMOKE_INSECURE=1`
* `MH2C_SAMPLE_H2_GET_BIN`
