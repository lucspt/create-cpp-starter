#!/usr/bin/env bash

set -euo pipefail

__wrap__() {
  TMP_DIR="$(mktemp -d)"
  INSTALL_PREFIX="$HOME/.create-cpp-starter"

  cleanup() {
    rm -rf "$TMP_DIR"
  }

  catch_err() {
    cleanup
    rm -rf "$INSTALL_PREFIX"
  }

  # trap catch_err ERR
  trap cleanup EXIT

  GIT_CLONE_URL=https://github.com/lucspt/create-cpp-starter.git

  echo "This script will install create-cpp-starter for you."
  cd "$TMP_DIR"
  git clone --quiet "$GIT_CLONE_URL" .

  cd create-cpp-starter

  mkdir build && cd build

  cmake .. -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" >/dev/null
  cmake --build . >/dev/null
  cmake --install . >/dev/null

  {
    echo 'case ":${PATH}:" in'
    echo '*:"$HOME/.create-cpp-starter/bin":*) ;;'
    echo "*)"
    echo '  export PATH="$HOME/.create-cpp-starter/bin:$PATH"'
    echo "  ;;"
    echo "esac"
  } >>"$INSTALL_PREFIX/env"

}
__wrap__
