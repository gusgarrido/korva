#!/usr/bin/env bash

set -e

make build
make repl
./build/target/repl
