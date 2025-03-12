FILE=record
FILTER=filter

run: ${FILE}
	./${FILE}

${FILE}: ${FILE}.c
	gcc -g3 -O0 -o ${FILE} ${FILE}.c

${FILTER}: ${FILTER}.c
	gcc -g3 -O0 -o ${FILTER} ${FILTER}.c

#filter: filter.c
#	gcc -g3 -O0 -o filter filter.c

# IMPORTANT: Learn the 'vi' editor (see 'help' dir).
# Use vim command ':syntax off' if vim messes up the colors on-screen.
vi vim: ${FILE}.c
	vim ${FILE}.c

# IMPORTANT: Python is easy because it's an interpreter.
#            'gdb' is the interpreter for C/C++.  Learn it!
#            (See course web page near bottom.)
# Some environments mess up GDB by using colors.  To disable, do:
# (gdb) set style enabled off
gdb: ${FILE}
	gdb --args ${FILE}

check: ${FILE} ${FILTER}
	./record ls -l /usr/local/bin
	rm -f record1.out
	cp record.out record1.out
	./record -v ls -l /usr/local/bin
	rm -f record2.out
	cp record.out record2.out

clean:
	rm -f a.out ${FILE} ${FILTER} record.out record1.out record2.out

dist: clean
	dir=`basename $$PWD`; cd ..; tar czvf $$dir.tar.gz ./$$dir
	dir=`basename $$PWD`; ls -l ../$$dir.tar.gz   
