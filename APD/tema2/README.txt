In aceast proiect, am avut de implementat protocolul BitTorrent care este un protocol de comunicatie pentru partajarea de fisiere. 
Pentru realizarea acestuia, am implementat functii pentru tracker, peer si upload_thread_func, download_thread_func.

Tracker:
Primeste informatii despre fisierele detinute de clienti.
Trimite lista de segmente si clienti relevanti atunci cand un client cere un fisier.
Verifica finalizarea operatiilor folosind un vector de stare.

Peer:
Trimite informatiile despre fisierele detinute catre tracker.
Utilizeaza functia download_thread_func pentru a descarca segmente si upload_thread_func pentru a incarca segmente catre alti clienti.
Am folosit un algoritm Round Robin pentru a selecta clientii de la care se descarca segmentele, echilibrand sarcina intre acestia. Tot procesul este gestionat cu mesaje MPI pentru sincronizare si comunicare intre procese.