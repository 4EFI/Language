push rax
push 0
add
pop rbx ; set "a" pos

push 0
pop [ rbx ] ; set "a"

push rax
push 1
add
pop rbx ; set "b" pos

push 0
pop [ rbx ] ; set "b"

push rax
push 2
add
pop rbx ; set "ts" pos

push 0
pop [ rbx ] ; set "ts"

in
push rax
push 0
add
pop rbx ; set "a" pos

pop [ rbx ]
in
push rax
push 1
add
pop rbx ; set "b" pos

pop [ rbx ]
in
push rax
push 2
add
pop rbx ; set "ts" pos

pop [ rbx ]
push rax
push 3
add
pop rbx ; set "i" pos

push 0
pop [ rbx ] ; set "i"


while001:
push rax
push 3
add
pop rbx ; set "i" pos

push [ rbx ] ; push "i"
push 5
sub
push 0
je :endWhile001

push rax
push 2
add
pop rbx ; set "ts" pos

push [ rbx ] ; push "ts"
out
push rax
push 2
add
pop rbx ; set "ts" pos

push [ rbx ] ; push "ts"
push 2
pow
push rax
push 2
add
pop rbx ; set "ts" pos

pop [ rbx ] ; set "ts"

push rax
push 3
add
pop rbx ; set "i" pos

push [ rbx ] ; push "i"
push 1
add
push rax
push 3
add
pop rbx ; set "i" pos

pop [ rbx ] ; set "i"


jmp :while001
endWhile001:


hlt
