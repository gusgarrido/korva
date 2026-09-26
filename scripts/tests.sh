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
    local failed=0

    while IFS= read -r -d '' test; do
        echo -n -e "\t\e[1;30mTesting \e[0;33m$test\e[30m... " >&2

        if output=$("$test" 2>&1); then
            echo -e "\e[1;32mPassed\e[0m" >&2
        else
            status=$?
            ((++failed))
            echo -e "\e[1;31mFailed (exit $status)\e[0m" >&2
            echo -e "\t\t$output\e[0m"
        fi
    done < <(
        find "build/tests/$1" -type f -name 'test_*' -executable -print0
    )
    

    if (( failed == 0 )); then
        echo -e "\e[42mAll tests passed successfully.\e[0m\n"
    else
        echo -e "\e[41mEncountered $failed tests that did not pass.\e[0m\n"
    fi

    return "$failed"
}

function check_leaks {
    if ! command -v valgrind >/dev/null 2>&1; then
        return 0
    fi

    echo -e "\e[44m[*]\e[0m Checking for memory leaks..."

    while IFS= read -r -d '' test; do
        echo -n -e "\t\e[1;30mAnalyzing \e[0;33m$test\e[30m..." >&2

        set +e
        output=$(valgrind -q --leak-check=summary --error-exitcode=1 "./$test" 2>&1)
        status=$?
        set -e

        if (( status == 0 )); then
            echo -e " \e[1;32mAll bytes accounted for\e[0m" >&2
        else
            echo -e " \e[1;41mPossible leakage detected\e[0m" >&2
            echo -e "\t\t\e[30mRun \e[34m'valgrind --track-origins=yes --leak-check=full ./$test'\e[0m \e[30mfor further debugging.\e[0m"
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

if ! command -v valgrind >/dev/null 2>&1; then
    echo -e "\n\e[1;30;43mWARNING:\e[0m valgrind not found. Memory leak checks will \e[1;4mNOT\e[0m run.\n"
fi

exitcode=0

for module in "$@"; do
    echo -e "\e[44m[*]\e[0m Running module \e[1;34m$module\e[0m:"
    if ! run_module "$module"; then
        exitcode=1
        continue
    fi
    check_leaks "$module"
done

echo -e "\e[1mFinished performing all tasks.\e[0m"

exit "$exitcode"
