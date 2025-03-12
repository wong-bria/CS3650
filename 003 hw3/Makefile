#JAR_PATH = /home/brianwong1/cs3650/homework/rars1_6.jar

all: hw3.out collatz.out

# Target to produce hw3.out using RARS
hw3.out: hw3.asm rars1_6.jar
	java -jar rars1_6.jar hw3.asm > hw3.out

# Target to produce collatz.out using Python
collatz.out: collatz.py rars1_6.jar
	python3 collatz.py > collatz.out

# compare the outputs of the assembly collatz and Python collatz
check: collatz.out hw3.out
	# skip the first two lines of hw3.out since it was showing RARS copyright
	tail -n +3 hw3.out | diff - collatz.out

# remove generated files
clean:
	rm -f collatz.out hw3.out rars1_6.jar

dist: clean
	dir=`basename $$PWD`; cd ..; tar czvf $$dir.tar.gz ./$$dir
	dir=`basename $$PWD`; ls -l ../$$dir.tar.gz



