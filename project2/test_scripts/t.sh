make
array=("01" "02" "03" "04" "05" "06" "07" "08" "09" "10" "11" "12" "13" "14" "15")
for i in ${array[@]}; do
    echo "${i}"
    bin/splc test/test_2_r"${i}".spl > out/"${i}".out
    diff --side-by-side out/"${i}".out test/test_2_r"${i}".out
done
