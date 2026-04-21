# Repository Guidelines

## Project Intent
This repository is not only an RFC-compliant HTTP/2 library. It is also a
manual HTTP/2 client for reproducing real-world and intentionally misbehaving
client behavior when debugging servers. Preserve the ability to build, send,
parse, and inspect unusual or malformed frames when that supports the manual
client use case.

## Project Structure & Module Organization
Core library sources live under `mh2c/`. The main areas are `frame/` for HTTP/2 frame types and parsing, `hpack/` for header compression, `ssl/` for OpenSSL wrappers, `util/` for low-level helpers, and `common/` for shared types. Public headers are exposed from `mh2c/` and selected subdirectories during install. Unit tests mirror the library layout under `test/` (for example `test/frame/*` and `test/hpack/*`). Sample usage lives in `sample/h2_get/`. Treat `build/` as generated output; do not hand-edit or commit it unless the task explicitly requires generated files.

## Build, Test, and Development Commands
Configure with CMake and point it at OpenSSL when needed:
```sh
cmake -B build -DOPENSSL_ROOT_DIR=/path/to/openssl -DBUILD_SHARED_LIBS=ON
```
Build everything with:
```sh
cmake --build build
```
Run unit tests with:
```sh
cmake --build build --target test
ctest --test-dir build --output-on-failure
```
Format sources with:
```sh
cmake --build build --target format
```
Run lint checks with:
```sh
cmake --build build --target lint
```
`format` requires `clang-format`; `lint` requires `cpplint`; tests require GoogleTest.

## Coding Style & Naming Conventions
Use C++17. Formatting is enforced by `.clang-format`: 2-space indentation, no tabs, 80-column limit, attached braces, and Google-style include sorting. Prefer matching the existing naming scheme: lowercase file names with underscores (`frame_builder.cpp`), class/type names in PascalCase, and test files ending in `_test.cpp`. Keep headers and implementations paired (`foo.h`, `foo.cpp`, optional `foo.ipp`).

## Testing Guidelines
Add or update tests in the matching `test/<area>/` directory when behavior changes. Follow the existing file pattern `*_test.cpp` and keep coverage close to the touched module. New frame, HPACK, or utility behavior should ship with a focused unit test before broader sample changes.
Do not assume stricter RFC rejection is automatically correct. When behavior is
intentionally permissive for the manual client, prefer tests that catch unsafe
failure modes such as crashes, overreads, silent normalization, or raw payload
loss instead of forcing rejection of every malformed input.

## Commit & Pull Request Guidelines
Recent history uses short Conventional Commit-style subjects such as `fix: ...` and `chore: ...`, with occasional `[WIP]` prefixes for in-progress work. Keep commits small and behavior-focused. Pull requests should explain the change, note any OpenSSL or build assumptions, link related issues, and include the commands you ran (`cmake --build build`, `ctest`, `format`, `lint`). Include sample output only when it clarifies a protocol or API change.
