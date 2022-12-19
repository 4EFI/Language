push rax
push 0
add
pop rbx ; set "x" pos

push 1
pop [ rbx ] ; set "x"

push rax
push 1
add
pop rbx ; set "y" pos

push 10
pop [ rbx ] ; set "y"


while001:
push rax
push 0
add
pop rbx ; set "x" pos

push [ rbx ] ; push "x"
push 0
je :endWhile001

push 1000
out
push rax
push 0
add
pop rbx ; set "x" pos

push [ rbx ] ; push "x"
push 1
sub
push rax
push 0
add
pop rbx ; set "x" pos

pop [ rbx ] ; set "x"


jmp :while001
endWhile001:

out

hlt
