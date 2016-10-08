#Test the image files, note their compression and compression time etc.

RES=$1
TYPE=$2

echo "Running $TYPE tests from $RES"
mkdir -p "${RES}/decoded"
mkdir -p "${RES}/encoded"

OUT=""
for file in ${RES}/*.${TYPE}
do
	file=${file##*/}

	# compress, then decompress, collecting output each time
	T_COMPRESS_START=$(date +"%s.%N")
	OUT+=$(./huff -r "${RES}/$file" "${RES}/encoded/${file}.hff")
	T_COMPRESS_END=$(date +"%s.%N")
	OUT+='\n'
	T_DECOMPRESS_START=$(date +"%s.%N")
	OUT+=$(./huff --puff -r "${RES}/encoded/${file}.hff" "${RES}/decoded/$file")
	T_DECOMPRESS_END=$(date +"%s.%N")
	OUT+='\n'

	#Calculate time taken to compress and decompress, also the total time
	T_COMPRESS_TOTAL=$(echo "$T_COMPRESS_END - $T_COMPRESS_START" | bc)
	T_DECOMPRESS_TOTAL=$(echo "$T_DECOMPRESS_END - $T_COMPRESS_START" | bc)
	T_TOTAL=$(echo "$T_COMPRESS_TOTAL + $T_DECOMPRESS_TOTAL" | bc)

	# check if the decompressed output is the same as the initial input
	DIFF=$(diff "${RES}/$file" "${RES}/decoded/$file")
	if [ "$DIFF" != "" ]
	then
		echo "fail ${file} in ${T_TOTAL}:"
		cmp "${RES}/$file" "${RES}/decoded/$file"
	else
		echo "pass $file | compressed in ${T_COMPRESS_TOTAL}, decompressed in ${T_DECOMPRESS_TOTAL}"
	fi
done

echo -e "---------------------------\n" >> "${RES}/${TYPE}_test.log"
echo -e "$OUT" >> "${RES}/${TYPE}_test.log"
