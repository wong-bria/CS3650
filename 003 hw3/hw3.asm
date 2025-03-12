	.data
answer:	.asciz "collatz("
answer2: .asciz "): "     
answer3: .asciz "\n"     

	.text
	
main:
	li t0, 1 # i = 1
loop:
	# prolog
	addi sp, sp, -16 # PUSH1: allocate 16 Bytes on the stack 
	sw ra, 8(sp)     # PUSH2: save ra into first allocated word 
	
	li t1, 100      # t1 = 100
	beq t0, t1, end # if i == 100, exit loop
	
	mv a0, t0       # n = i
	jal ra, collatz # call collatz(n) <-- function call, so need prolog/epilog here
	
	mv a1, t5 # a1 = result of collatz(n)
	
	li a7, 4 # PrintString
	la a0, answer
	ecall
	
	li a7, 1 # PrintInt
	mv a0, t0
	ecall
	
	li a7, 4 # PrintString
	la a0, answer2
	ecall
	
	li a7, 1 # PrintInt
	mv a0, a1 # result from collatz(n)
	ecall
	
	li a7, 4 # PrintString
	la a0, answer3
	ecall
	
	addi t0, t0, 1 # i = i + 1
	j loop
	
	# epilog
	lw ra, 8(sp)           # Restore return address
	addi sp, sp, 16        # Restore stack pointer (deallocate 16 bytes)
end:
	li a7, 10 # exit
	ecall

# collatz(n) which calls f(n, 1)
collatz: # Prolog of 'collatz' function: Create call frame
	addi sp, sp, -16 # PUSH1: allocate 16 Bytes on the stack 
	sw ra, 8(sp)     # PUSH2: save ra into first allocated word 
	
	# call f(n,1)
	li a1, 1 # x = 1
	jal ra, f # jump to f(n,x)
	
	mv t5, a1
	
	# Epilog of collatz function
    	lw ra, 8(sp)         # POP2: restore ra from stack 
    	addi sp, sp, 16      # POP1: dealloc the 16 B that I had allocated 
    	jr ra                # Return to caller
    	
# f(n,x)
f: # Prolog of f(n,x) function: Create call frame
	addi sp, sp, -16 # PUSH1: allocate 16 Bytes on the stack 
	sw ra, 8(sp)     # PUSH2: save ra into first allocated word 
	
	# Base case: if n == 1, return x
	li t2, 1
	beq a0, t2, f_base # if n == 1, go to base case
	
	# check if n is even
	li t2, 2
	rem t3, a0, t2       # t3 = remainder (n / 2)
	beq t3, zero, f_even # if n % 2 == 0, go to even case
	
	# check if n is odd
	bnez t3, f_odd # if n is odd, go to odd case
	
	# epilog will happen when f reaches base case, goes to f_base
	# then goes to f_exit which contains the epilog

# Odd case: f(3n + 1, x + 1)
f_odd:
	li t2, 1
	bgt a0, t2, f_odd_case # If n > 1, go to f_odd_case
	
f_odd_case:
	addi sp, sp, -16 # PUSH1: allocate 16 Bytes on the stack 
	sw ra, 8(sp)     # PUSH2: save ra into first allocated word 
	
	li t4, 3
	mul a0, a0, t4       # n = n * 3
	addi a0, a0, 1       # n = n + 1
	
	addi a1, a1, 1       # x = x + 1
	mv a1, a1
	jal ra, f            # Recursive call to f <- function call so need prolog/epilog
	mv t6, a1
	
	# epilog
	lw ra, 8(sp)           # Restore return address
	addi sp, sp, 16        # Restore stack pointer (deallocate 16 bytes)
	
	j f_exit              # epilog at f_exit

f_even:# Even case: f(n / 2, x + 1)
	addi sp, sp, -16 # PUSH1: allocate 16 Bytes on the stack 
	sw ra, 8(sp)     # PUSH2: save ra into first allocated word 

	li t4, 2
	div a0, a0, t4 # n = n / 2
	
	addi a1, a1, 1         # x = x + 1
	jal ra, f              # Recursive call to f <- function call so need prolog/epilog
	mv t6, a1
	
	lw ra, 8(sp)           # Restore return address
	addi sp, sp, 16        # Restore stack pointer (deallocate 16 bytes)
	
	j f_exit               # epilog at f_exit
    
f_base:
	mv t5, a1             # return t5 = x
	j f_exit              # epilog at f_exit
	
f_exit: # Epilog
	lw ra, 8(sp)           # Restore return address
	addi sp, sp, 16        # Restore stack pointer (deallocate 16 bytes)
	jr ra                  # Return to caller

