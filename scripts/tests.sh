#!/usr/bin/env bash

set -e

for module in $@; do
    if [ ! -d "build/tests/$module" ]; then
        echo -e "\e[31m[error]: Test module '$module' does not exist.\e[0m"
        exit 1
    fi
done

make build 1> /dev/null
make tests 1> /dev/null

function run_module {
    local output
    local status=0

    output=$(
        while IFS= read -r -d '' test; do
            echo -n -e "   \e[1;35mTesting \e[0;33m$test\e[30m..." >&2
            "$test"
            status=$?
            if (( status == 0 )); then
                echo -e " \e[1;32mPassed\e[0m" >&2
                continue
            fi
            echo -e " \e[1;31mFailed\e[0m" >&2
            exit $status
            
        done < <(
            find "build/tests/$1" -type f -name 'test_*' -executable -print0
        )
    ) || status=$?

    if (( status == 0 )); then
        echo -e "\e[42mAll tests passed successfully.\e[0m"
    else
        echo -e "\n$output"
    fi

    return "$status"
}

function check_leaks {
    if ! command -v valgrind >/dev/null 2>&1; then
        return 0
    fi

    echo -e "\e[44m[*]\e[0m Checking for memory leaks..."

    while IFS= read -r -d '' test; do
        echo -n -e "   \e[1;35mAnalyzing \e[0;33m$test\e[30m..." >&2

        set +e
        output=$(valgrind -q --leak-check=summary --error-exitcode=1 "./$test" 2>&1)
        status=$?
        set -e

        if (( status == 0 )); then
            echo -e " \e[1;32mAll bytes accounted for\e[0m" >&2
        else
            echo -e " \e[1;41mPossible leakage detected\e[0m" >&2
            echo -e "      \e[30mRun \e[34m'valgrind --track-origins=yes --leak-check=full ./$test'\e[0m \e[30mfor further debugging.\e[0m"
        fi
        
    done < <(
        find "build/tests/$1" -type f -name 'test_*' -executable -print0
    )
}

if [ $# -eq 0 ]; then
    echo -e "\e[1mExecuting tests for all modules.\e[0m"
    set -- ./
else
    echo -e "\e[1mExecuting tests for modules\e[0m $*\e[1m.\e[0m"
fi

if [ ! command -v valgrind >/dev/null 2>&1 ]; then
    echo -e "\e[1;43mWARNING: valgrind is not installed. Memory leaks will not be checked.\e[0m"
fi

for module in "$@"; do
    echo -e "\e[44m[*]\e[0m Running module \e[1;34m$module\e[0m:"
    run_module "$module"
    if [ $? -eq 0 ]; then
        check_leaks "$module"
    fi
done

echo -e "\e[1mFinished performing all tasks.\e[0m"
