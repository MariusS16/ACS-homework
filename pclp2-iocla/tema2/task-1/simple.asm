%include "../include/io.mac"

section .text
    global simple
    extern printf

simple:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     ecx, [ebp + 8]  ; len
    mov     esi, [ebp + 12] ; plain
    mov     edi, [ebp + 16] ; enc_string
    mov     edx, [ebp + 20] ; step

    ;; DO NOT MODIFY
   
    ;; Your code starts here

    xor eax, eax
    shiftare_dreapta:
        
        mov bl, byte [esi + eax]
        add bl, dl  ;am adaugat step ul

        cmp bl, 'Z' ;vedem daca a depasit litera Z
        jle mutare

        sub bl, 'Z' - 'A' + 1 ;daca a depasit litera Z scadem diferenta
        jmp mutare

    mutare:
        mov byte [edi + eax], bl 
        add eax, 1
        cmp eax, ecx
        jne shiftare_dreapta


    ;; Your code ends here
    
    ;; DO NOT MODIFY

    popa
    leave
    ret
    
    ;; DO NOT MODIFY
