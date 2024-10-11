global get_words
global compare_func
global sort

extern strtok
extern strcmp
extern qsort
extern strcat
extern strlen

section .data
    tokens db " ,.", 10, 0
section .text

compare:
    enter 0, 0

    push ebx
    xor ebx, ebx
    
    mov ebx, dword[ebp + 8]
    mov ecx, dword[ebp + 12]

    mov ebx, dword [ebx]
    mov ecx, dword [ecx]

    push ecx
    push edx

    push ebx
    call strlen
    add esp, 4

    pop edx
    pop ecx

    mov edx, eax
    push ecx
    push edx
    
    push ecx
    call strlen
    add esp, 4

    pop edx
    pop ecx

    cmp edx, eax
    jg LUNG
    jl SCURT
    je EGAL

LUNG:
    mov eax, 1
    jmp END_COMPARE

SCURT:
    mov eax, -1
    jmp END_COMPARE

EGAL:
    push ecx
    push ebx
    call strcmp
    add esp, 8

END_COMPARE:
    pop ebx
    leave
    ret


;; sort(char **words, int number_of_words, int size)
;  functia va trebui sa apeleze qsort pentru soratrea cuvintelor 
;  dupa lungime si apoi lexicografix
sort:
    enter 0, 0
    pushad

    mov eax, dword[ebp + 8]
    mov ebx, dword[ebp + 12]
    mov ecx, dword[ebp + 16]

    push compare
    push ecx
    push ebx
    push eax
    call qsort
    add esp, 16

    popad
    leave
    ret



;; get_words(char *s, char **words, int number_of_words)
;  separa stringul s in cuvinte si salveaza cuvintele in words
;  number_of_words reprezinta numarul de cuvinte


get_words:
    enter 0, 0
    pushad

    mov esi, dword[ebp + 8]
    mov edi, dword[ebp + 12]
    mov ecx, dword[ebp + 16]

    xor edx, edx
    xor eax, eax

    push ecx
    push edx

    push tokens
    push esi
    call strtok
    add esp, 8
    
    pop edx
    pop ecx
WHILE:
    cmp eax, 0 
    je END
    mov dword[edi + 4 * edx], eax

    push edx
    push ecx

    push tokens
    push 0
    call strtok
    add esp, 8

    pop ecx
    pop edx
    
    add edx, 1
    
    jmp WHILE

END:
    popad
    leave
    ret
