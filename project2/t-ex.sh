make
array=("1" "2")
for i in ${array[@]}; do
    echo "${i}"
    bin/splc test-ex/test_"${i}".spl > out/"${i}".out
    diff out/"${i}".out test-ex/test_"${i}".out
done
