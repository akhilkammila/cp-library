#! /bin/bash
for((i = 0; i < 20; i++))
do
    python3 gen.py > test.in
    python3 Naive.py < test.in > a.out
    ./WhyDidCowCrossRoad < test.in > b.out

    if ! diff -w a.out b.out; then
        echo "Breaking at iteration $i"
        break
    fi
done