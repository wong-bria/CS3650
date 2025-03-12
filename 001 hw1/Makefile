# This is the default file for the 'make' command.  cd to this dir first.

# FILE is a variable.  Use '${FILE}' to get its value.
FILE=fork-exec

# The syntax is:
#    <TARGET>: <DEPENDENCY> ...
# 'make <TARGET>' looks for a <DEPENDENCY> that is newer than its <TARGET>,
# and then runs the one or more commands, which must create the <TARGET>
# Each command must begin with the <TAB> character.

# The first target is the default target if you just type 'make'.
run: ${FILE}
	./${FILE}

# If then 
# '-o' for output.  '-g3 -O0' for GDB debugging with symbols
${FILE}: ${FILE}.c
	gcc -g3 -O0 -o ${FILE} ${FILE}.c

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

check: ${FILE}
	cat ${FILE}.in | ./${FILE} | less

clean:
	rm -f a.out ${FILE}

dist: clean
	dir=`basename $$PWD`; cd ..; tar czvf $$dir.tar.gz ./$$dir
	dir=`basename $$PWD`; ls -l ../$$dir.tar.gz   
