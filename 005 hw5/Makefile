hw5: hw5.c
	gcc -g3 -o hw5 hw5.c

check: hw5
	echo "Testing hw5"
	cat hw5.in | ./hw5 -a | diff --ignore-all-space - hw5.out-raw-fully-assoc
	cat hw5.in | ./hw5 -d | diff --ignore-all-space - hw5.out-raw-direct-mapped

clean:
	rm -f hw5

dist: clean
	dir=`basename $$PWD`; cd ..; tar czvf $$dir.tar.gz ./$$dir
	dir=`basename $$PWD`; ls -l ../$$dir.tar.gz   
