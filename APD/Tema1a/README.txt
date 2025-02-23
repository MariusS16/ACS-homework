Tema1a APD:
In aceast proiect am 5 functii pentru rezolvarea aceasteia(map, reduce, word_processing, thread_function, main). 
Incepand din functia main se citesc patametrii, iar apoi se citesc fisierele care trebuie procesate, creez M + R threaduri care primesc fiecare argumentele(id, fileID, files, mutex etc.) si se apeleaza functia de thread.
In functia de thread, in functie de id-ul threadului separ thread-urile map de cele reduce si apelez functiile necesare(map/reduce), iar cu ajuorul unei bariere fac ca thread-urile sa astepte.
Functia map se ocupa cu "procesarea" fiecarui fisier, adica citeste din fiecare fisier asociat si apeleaza functia word_processing care transforma fiecare cuvant doar in litere mici). 
Functia reduce preia toate rezultatele obtinute de la mapperi, facand o sortare pe acestea si afisandu-le in cate un fiser "litera.txt".