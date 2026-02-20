#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="${1:-build}"
EXAMPLES_DIR="${2:-examples}"
LOOM_ARGS="${LOOM_ARGS:-}"

# find loom executable (support .exe)
find_exe() {
  local name="$1"
  find "$BUILD_DIR" -type f \( -iname "${name}" -o -iname "${name}.exe" -o -iname "${name}*.exe" \) 2>/dev/null | head -n1
}

LOOM_BIN="$(find_exe loom)"
if [[ -z "$LOOM_BIN" ]]; then
  echo "No loom binary found under $BUILD_DIR"
  exit 1
fi

# runner for .exe (use wine if needed)
runner_for() {
  local bin="$1"
  if [[ "$bin" == *.exe ]] && [[ ! -x "$bin" ]]; then
    if command -v wine >/dev/null 2>&1; then
      echo "wine"
    else
      echo ""
    fi
  else
    echo ""
  fi
}
LOOM_RUNNER="$(runner_for "$LOOM_BIN")"

# gather json files
mapfile -t JSON_FILES < <(find "$EXAMPLES_DIR" -type f -name '*.json' | sort)
if [[ ${#JSON_FILES[@]} -eq 0 ]]; then
  echo "No .json files found under $EXAMPLES_DIR"
  exit 1
fi

mkdir -p test/logs test/outputs
failed=0

run_loom() {
  local json="$1"
  local base="$(basename "$json" .json)"
  local out="test/outputs/${base}.out"
  local log="test/logs/loom__${base}.log"

  echo "RUN: cat $json | ${LOOM_RUNNER:+$LOOM_RUNNER }$LOOM_BIN $LOOM_ARGS > $out 2> $log"
  if ! ( cat "$json" | ${LOOM_RUNNER:+$LOOM_RUNNER } "$LOOM_BIN" $LOOM_ARGS > "$out" ) 2> "$log"; then
    echo "FAIL: loom with $json (see $log)"
    failed=1
  else
    echo "OK: loom produced $out (stderr log: $log)"
  fi
}

for json in "${JSON_FILES[@]}"; do
  echo "=== TESTING with $json ==="
  run_loom "$json"
done

if [[ $failed -ne 0 ]]; then
  echo "Some tests failed"
  exit 1
fi
echo "All tests passed"
```// filepath: test/run_tests.sh
#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="${1:-build}"
EXAMPLES_DIR="${2:-examples}"
LOOM_ARGS="${LOOM_ARGS:-}"

# find loom executable (support .exe)
find_exe() {
  local name="$1"
  find "$BUILD_DIR" -type f \( -iname "${name}" -o -iname "${name}.exe" -o -iname "${name}*.exe" \) 2>/dev/null | head -n1
}

LOOM_BIN="$(find_exe loom)"
if [[ -z "$LOOM_BIN" ]]; then
  echo "No loom binary found under $BUILD_DIR"
  exit 1
fi

# runner for .exe (use wine if needed)
runner_for() {
  local bin="$1"
  if [[ "$bin" == *.exe ]] && [[ ! -x "$bin" ]]; then
    if command -v wine >/dev/null 2>&1; then
      echo "wine"
    else
      echo ""
    fi
  else
    echo ""
  fi
}
LOOM_RUNNER="$(runner_for "$LOOM_BIN")"

# gather json files
mapfile -t JSON_FILES < <(find "$EXAMPLES_DIR" -type f -name '*.json' | sort)
if [[ ${#JSON_FILES[@]} -eq 0 ]]; then
  echo "No .json files found under $EXAMPLES_DIR"
  exit 1
fi

mkdir -p test/logs test/outputs
failed=0

run_loom() {
  local json="$1"
  local base="$(basename "$json" .json)"
  local out="test/outputs/${base}.out"
  local log="test/logs/loom__${base}.log"

  echo "RUN: cat $json | ${LOOM_RUNNER:+$LOOM_RUNNER }$LOOM_BIN $LOOM_ARGS > $out 2> $log"
  if ! ( cat "$json" | ${LOOM_RUNNER:+$LOOM_RUNNER } "$LOOM_BIN" $LOOM_ARGS > "$out" ) 2> "$log"; then
    echo "FAIL: loom with $json (see $log)"
    failed=1
  else
    echo "OK: loom produced $out (stderr log: $log)"
  fi
}

for json in "${JSON_FILES[@]}"; do
  echo "=== TESTING with $json ==="
  run_loom "$json"
done

if [[ $failed -ne 0 ]]; then
  echo "Some tests failed"
  exit 1
fi
echo "All tests passed"