#!/bin/sh

set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
repo_root=$(CDPATH= cd -- "$script_dir/../.." && pwd)

binary_path=${MH2C_SAMPLE_H2_GET_BIN:-"$repo_root/build/sample/h2_get/sample_h2_get"}
target_host=${MH2C_SMOKE_HOST:-nghttp2.org}
target_port=${MH2C_SMOKE_PORT:-443}
target_path=${MH2C_SMOKE_PATH:-/httpbin/headers}
target_authority=${MH2C_SMOKE_AUTHORITY:-$target_host}
target_insecure=${MH2C_SMOKE_INSECURE:-0}

if [ ! -x "$binary_path" ]; then
  echo "sample_h2_get binary not found: $binary_path" >&2
  echo "Build it first with:" >&2
  echo "  cmake -B build -DBUILD_SHARED_LIBS=ON -DMH2C_BUILD_SAMPLE=ON" >&2
  echo "  cmake --build build --target sample_h2_get" >&2
  exit 1
fi

set -- \
  "$binary_path" \
  --host "$target_host" \
  --port "$target_port" \
  --authority "$target_authority" \
  --path "$target_path" \
  "$@"

if [ "$target_insecure" = "1" ]; then
  set -- "$@" --insecure
fi

echo "Running smoke check command:"
printf '  %s\n' "$*"

exec "$@"
