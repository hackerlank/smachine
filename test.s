CATE #0x0

ldr r0, #0x12345678
mov r1, r0
mov r1, r0
ldr r0, [r1]
str r0, [r1]
push r0
pop  r0
call #0x0
ret
add r0, r0, r1
div r0, r0, r1
sub r0, r0, r1
or  r0, r0, r1
xor r0, r0, r1
jmp r0
jmpn r0
jmpz r0
jmpo r0 
jmpnn r0
jmpnz r0
jmpno r0

