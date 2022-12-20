push 0
push rax
add
pop rax ; Shifting top the var register
push rax
push 0
add
pop rbx ; set "i" pos

push 0
pop [ rbx ] ; set "i"

push 1
push rax
add
pop rax ; Shifting top the var register

while001:
push rax
push -1
add
pop rbx ; set "i" pos

push [ rbx ] ; push "i"
push 5
sub
push 0
je :endWhile001

push rax
push 0
add
pop rbx ; set "lokal" pos

push 11
pop [ rbx ] ; set "lokal"

push rax
push -1
add
pop rbx ; set "i" pos

push [ rbx ] ; push "i"
out
push 1
push rax
add
pop rax ; Shifting top the var register
push rax
push -2
add
pop rbx ; set "i" pos

push [ rbx ] ; push "i"
push 2
sub
push 0
je :endif001

push rax
push 0
add
pop rbx ; set "opa" pos

push 500
pop [ rbx ] ; set "opa"

push rax
push 0
add
pop rbx ; set "opa" pos

push [ rbx ] ; push "opa"
out

endif001:

push -1
push rax
add
pop rax ; Shifting down the var register
push rax
push -1
add
pop rbx ; set "i" pos

push [ rbx ] ; push "i"
push 1
add
push rax
push -1
add
pop rbx ; set "i" pos

pop [ rbx ] ; set "i"


jmp :while001
endWhile001:

push -1
push rax
add
pop rax ; Shifting down the var register
push 0
push rax
add
pop rax ; Shifting down the var register

hlt
