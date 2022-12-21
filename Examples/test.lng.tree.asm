jmp :Mein
Petrovich:
push 0
push rax
add
pop rax ; Shifting top the var register
push rax
push 0
add
pop rbx ; set "a" pos

pop [ rbx ]

push rax
push 1
add
pop rbx ; set "b" pos

pop [ rbx ]

push rax
push 0
add
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
push rax
push 1
add
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
add
out
push rax
push 0
add
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
push rax
push 1
add
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
add
push rax
push 0
add
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
push rax
push 1
add
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
add
push 0
push rax
add
pop rax ; Shifting down the var register
ret
endPetrovich:
Mein:
push 0
push rax
add
pop rax ; Shifting top the var register
push 10
push 2
push rbx
 pop rcx ; save pos before call
call :Petrovich
push rcx
 pop rcx ; save pos before call
push rax
push 0
add
pop rbx ; set "i" pos

pop [ rbx ] ; set "i"

push rax
push 0
add
pop rbx ; set "i" pos

push [ rbx ] ; push "i"
out
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
is_bt
push 0
je :endWhile001

push 11
push rax
push 0
add
pop rbx ; set "lokal" pos

pop [ rbx ] ; set "lokal"

push rax
push 0
add
pop rbx ; set "lokal" pos

push [ rbx ] ; push "lokal"
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
push 0
is_ee
push 0
je :endif001

push 100
push rax
push 0
add
pop rbx ; set "opa" pos

pop [ rbx ] ; set "opa"

push rax
push 0
add
pop rbx ; set "opa" pos

push [ rbx ] ; push "opa"
sqrt
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
ret
endMein:

hlt
