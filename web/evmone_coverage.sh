#!/bin/bash
# Produce lcov html coverage report on evmone for the tests in folder BASE and PATCH
# Make a diff between BASE and PATCH highlighting what new coverage is done by PATCH compared to BASE
# And what coverage is now missing by PATCH compared to BASE
# Results are stored in RESULT variable
# Tests TYPE =  BlockchainTests | GeneralStateTests

BASE=$1
PATCH=$2
RESULT=$3
TYPE=$4
BUILD="/evmone/build"

retesteth -t $TYPE -- --testfile $BASE --clients evmone
lcov --capture --directory $BUILD --output-file $BUILD/coverage.lcov --ignore-errors mismatch  --exclude="$HOME/.hunter/*" --exclude="$PWD/_deps/*"  --exclude="11"
lcov --zerocounters --directory $BUILD
genhtml $BUILD/coverage.lcov --output-directory $RESULT/BASE --title BASE_COVERAGE
cp $BUILD/coverage.lcov $RESULT/coverage_base.lcov

retesteth -t $TYPE -- --testfile $PATCH --clients evmone
lcov --capture --directory $BUILD --output-file $BUILD/coverage.lcov --ignore-errors mismatch  --exclude="$HOME/.hunter/*" --exclude="$PWD/_deps/*"  --exclude="11"
lcov --zerocounters --directory $BUILD
genhtml $BUILD/coverage.lcov --output-directory $RESULT/PATCH --title PATCH_COVERAGE
cp $BUILD/coverage.lcov $RESULT/coverage_patch.lcov

genhtml $RESULT/coverage_patch.lcov --baseline-file $RESULT/coverage_base.lcov --output-directory $RESULT/DIFF --title DIFF_COVERAGE

#find . -name "*.gcda" -o -name "*.gcno" -exec rm -f {} +
