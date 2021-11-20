make
array=("1" "2" "3" "4" "5" "6" "7" "8" "9" "10" )
for i in ${array[@]}; do
    echo "${i}"
    bin/splc test_/test_11910934_"${i}".spl > out/"${i}".out
    diff out/"${i}".out test_/test_11910934_"${i}".out
done
