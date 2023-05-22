tc=$(ls ./testcase)

for i in $tc; do
    echo $i
    ./compiler.out ./testcase/$i $i
done