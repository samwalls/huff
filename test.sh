N=$1

for i in `seq 1 $N`;
do
	echo "running test sequence ${i}"
	./testset.sh tar tar 2>&1 | tee  --append "logs/run${i}.log"
	./testset.sh txt txt 2>&1 | tee --append "logs/run${i}.log"
	./testset.sh img bmp 2>&1 | tee --append "logs/run${i}.log"
done
