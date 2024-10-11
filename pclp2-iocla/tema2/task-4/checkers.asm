
section .data

section .text
	global checkers

checkers:
    ;; DO NOT MODIFY
    push ebp
    mov ebp, esp
    pusha

    mov eax, [ebp + 8]	; x
    mov ebx, [ebp + 12]	; y
    mov ecx, [ebp + 16] ; table

    ;; DO NOT MODIFY
    ;; FREESTYLE STARTS HERE

    cmp eax, 7
    je jos ;trec la label-ul "jos" care imi pune 1 numai jos sub linia respectiva

    dreapta_sus:

        cmp ebx, 7
        je stanga_sus ;trec la label-ul stanga-sus care pune 1 pe diagonala din stanga sus

        xor edx, edx
        xor esi, esi
        mov esi, 8
        mov edx, eax
        add edx, 1
        push eax
        xor eax, eax
        mov eax, edx
        mul esi
        xor edx, edx
        mov edx, eax
        xor eax, eax
        pop eax
        add edx, ebx
        add edx, 1
        add byte[ecx + edx], 1


    stanga_sus:

        cmp ebx, 0
        je jos ;trec la label-ul "jos" care imi pune 1 numai jos sub linia respectiva

        xor edx, edx
        xor esi, esi
        mov esi, 8
        mov edx, eax
        add edx, 1
        push eax
        xor eax, eax
        mov eax, edx
        mul esi
        xor edx, edx
        mov edx, eax
        xor eax, eax
        pop eax
        add edx, ebx
        sub edx, 1
        add byte[ecx + edx], 1

    jos:

    cmp eax, 0
    je gata ;trec la label-ul "gata" care nu face nimic->se termina
    
    dreapta_jos:

        cmp ebx, 7
        je stanga_jos ;trec la label-ul stanga-jos care pune 1 pe diagonala din stanga jos

        xor edx, edx
        xor esi, esi
        mov esi, 8
        mov edx, eax
        sub edx, 1
        push eax
        xor eax, eax
        mov eax, edx
        mul esi
        xor edx, edx
        mov edx, eax
        xor eax, eax
        pop eax
        add edx, ebx
        add edx, 1
        add byte[ecx + edx], 1

    stanga_jos:

        cmp ebx, 0
        je gata ;trec la label-ul "gata" care nu face nimic->se termina

        xor edx, edx
        xor esi, esi
        mov esi, 8
        mov edx, eax
        sub edx, 1
        push eax
        xor eax, eax
        mov eax, edx
        mul esi
        xor edx, edx
        mov edx, eax
        xor eax, eax
        pop eax
        add edx, ebx
        sub edx, 1
        add byte[ecx + edx], 1

    gata: ;acesta nu face nimic ->se termina 

    ;; FREESTYLE ENDS HERE
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY