Problema 1: Numarare

Am citit cele doua numere si ceele doua grafuri, iar apoi m-am gandit sa folosesc programare dinamica. Am creat un nou adj3 in care am adaugat vecinii comuni. Pentru fiecare nod se actualizează numărul de drumuri pentru vecinii săi din adj3 (modulo 1000000007). Complexitatea algoritmului este O(N * M).


Problema 2: Trenuri

Am citit cele doua orase si apoi cele M curse directe. M-am gandit sa folosesc un algoritm DFS, iar apoi am ultilizat sortarea topolologica pentru a gasi cel mai lung drum de la start la destinatie. Complexitatea algoritmului este O(N + M).


Problema 3: Drumuri Obligatorii

Problema Drumuri Obligatorii cere suma minima astfel încât sa existe cel
putin un drum care pleaca din nodul x si cel putin un drum care pleaca din nodul y, ambele ajungând în nodul z. Am citit nodurile si muchiile: 
M-am gandit la algoritmul lui Dijkstra pentru a gasi distantele minime de la x, y si z la celelalte noduri(am aplicat de 3 ori algoritmul). Am aflat distanta finale minima prin calcularea distantelor print-un alt nod (suma celor 3 distante). Complexitatea algoritmului este O((N + M) * log(N)).