call :Mein
hlt
Faktorial:
push rax + 0
pop rbx ; set "n" pos

pop [ rbx ]

push rax + 0
pop rbx ; set "n" pos

push [ rbx ] ; push "n"
out
push 1 + rax
pop rax ; Shifting top the var register
push rax + -1
pop rbx ; set "n" pos

push [ rbx ] ; push "n"
push 1
is_be
push 0
je :endif001

push 1
ret

endif001:

push -1 + rax
pop rax ; Shifting down the var register
push rax + 0
pop rbx ; set "n" pos

push [ rbx ] ; push "n"
push rax + 0
pop rbx ; set "n" pos

push [ rbx ] ; push "n"
push 1
sub
push 1 + rax
pop rax ; Shifting top the var register
call :Faktorial
push -1 + rax
pop rax ; Shifting down the var register
mul
ret
ret
endFaktorial:
Mein:
push 5
push 1 + rax
pop rax ; Shifting top the var register
call :Faktorial
push -1 + rax
pop rax ; Shifting down the var register
out
push 0
ret
ret
endMein:
