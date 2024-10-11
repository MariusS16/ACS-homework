section .data

section .text
    global bonus

bonus:
    ;; DO NOT MODIFY
    push ebp
    mov ebp, esp
    pusha

    mov eax, [ebp + 8]	; x
    mov ebx, [ebp + 12]	; y
    mov ecx, [ebp + 16] ; board

    ;; DO NOT MODIFY
    ;; FREESTYLE STARTS HERE
    
    ; xor edi, edi
    ; mov edi, 1

    ; cmp eax, 3
    ; jle sus

    ; jmp jos

    ; sus:
    ;     sub eax, 4

    ;     xor edx, edx
    ;     xor esi, esi
    ;     mov esi, 8
    ;     mov edx, eax
    ;     add edx, 1
    ;     push eax
    ;     xor eax, eax
    ;     mov eax, edx
    ;     mul esi
    ;     xor edx, edx
    ;     mov edx, eax
    ;     xor eax, eax
    ;     pop eax
    ;     add edx, ebx
    ;     add edx, 1

        
    ;     bucla:
    ;         cmp edx, 0
    ;         je bend
    ;         shl edi, 1
    ;         dec edx
    ;         jmp bucla
        
    ;     bend:

    ;     add [ecx], edi

    ;     jmp gata

    ; jos:
    ;     xor edx, edx
    ;     xor esi, esi
    ;     mov esi, 8
    ;     mov edx, eax
    ;     add edx, 1
    ;     push eax
    ;     xor eax, eax
    ;     mov eax, edx
    ;     mul esi
    ;     xor edx, edx
    ;     mov edx, eax
    ;     xor eax, eax
    ;     pop eax
    ;     add edx, ebx
    ;     add edx, 1

        
    ;     bucla2:
    ;         cmp edx, 0
    ;         je b2end
    ;         shl edi, 1
    ;         dec edx
    ;         jmp bucla2
        
    ;     b2end:

    ;     add [ecx + 4], edi

    ; gata:

    ;; FREESTYLE ENDS HERE
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY