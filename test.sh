N=$1

for i in `seq 1 $N`;
do
	echo "running test sequence ${i}"
	echo "running tar tests"
	#unbuffer ./testset.sh tar tar 2>&1 | tee  "logs/run${i}.log"
	echo "running txt tests"
	unbuffer ./testset.sh txt txt 2>&1 | tee "logs/run${i}.log"
	echo "running img tests"
	#unbuffer ./testset.sh img bmp 2>&1 | tee "logs/run${i}.log"
done
