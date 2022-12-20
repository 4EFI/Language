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
push 0
add
pop rbx ; set "a" pos

push [ rbx ] ; push "a"
out
push rax
push 1
add
pop rbx ; set "b" pos

push [ rbx ] ; push "b"
out
push rax
push 2
add
pop rbx ; set "ts" pos

push [ rbx ] ; push "ts"
out

hlt
