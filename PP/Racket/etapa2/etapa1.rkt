#lang racket
(require "suffix-tree.rkt")

(provide (all-defined-out))

; TODO 2
; Implementați o funcție care primește două cuvinte (liste
; de caractere) w1 și w2 și calculează cel mai lung prefix
; comun al acestora, împreună cu restul celor două cuvinte
; după eliminarea prefixului comun.
; ex:
; (longest-common-prefix '(#\w #\h #\y) '(#\w #\h #\e #\n))
; => '((#\w #\h) (#\y) (#\e #\n))
; Folosiți recursivitate pe coadă.
(define (longest-common-prefix w1 w2)
  (longest-common-prefix-helper w1 w2 '()))

(define (longest-common-prefix-helper w1 w2 acc)
  (cond
    ((or (null? w1) (null? w2)) (list acc w1 w2))
    ((equal? (car w1) (car w2)) (longest-common-prefix-helper (cdr w1) (cdr w2) (append acc (list (car w1)))))
    (else (list acc w1 w2))))


; TODO 3
; Implementați recursiv o funcție care primește o listă nevidă 
; de cuvinte care încep cu același caracter și calculează cel 
; mai lung prefix comun al acestora.
; Opriți căutarea (parcurgerea) în momentul în care aveți garanția 
; că prefixul comun curent este prefixul comun final.
(define (longest-common-prefix-of-list words)
  (if (null? words)
      '()
      (if (= (length words) 1)
          (car words)
          (car (longest-common-prefix (car words) (longest-common-prefix-of-list (cdr words)))))))

;; Următoarele două funcții sunt utile căutării unui șablon
;; (pattern) într-un text cu ajutorul arborelui de sufixe.
;; Ideea de căutare este următoarea:
;; - dacă șablonul există în text, atunci există un sufix care
;;   începe cu acest șablon, deci există o cale care începe din
;;   rădăcina arborelui care se potrivește cu șablonul
;; - vom căuta ramura a cărei etichetă începe cu prima literă
;;   din șablon
;; - dacă nu găsim această ramură, șablonul nu apare în text
;; - dacă șablonul este conținut integral în eticheta ramurii,
;;   atunci el apare în text
;; - dacă șablonul se potrivește cu eticheta dar nu este conținut
;;   în ea (de exemplu șablonul "nana$" se potrivește cu eticheta
;;   "na"), atunci continuăm căutarea în subarborele ramurii
;; - dacă șablonul nu se potrivește cu eticheta (de exemplu
;;   șablonul "numai" nu se potrivește cu eticheta "na"), atunci
;;   el nu apare în text (altfel, eticheta ar fi fost "n", nu
;;   "na", pentru că eticheta este cel mai lung prefix comun al
;;   sufixelor din subarborele său)


; TODO 4
; Implementați funcția match-pattern-with-label care primește un
; arbore de sufixe și un șablon nevid și realizează un singur pas 
; din procesul prezentat mai sus - identifică ramura arborelui a
; cărei etichetă începe cu prima literă din șablon, apoi
; determină cât de bine se potrivește șablonul cu eticheta,
; întorcând ca rezultat:
; - true, dacă șablonul este conținut integral în etichetă
; - lista (etichetă, nou pattern, subarbore), dacă șablonul se
;   potrivește cu eticheta dar nu este conținut în ea
;   (ex: ("na", "na$", subarborele de sub eticheta "na")
;   pentru șablonul inițial "nana$" și eticheta "na")
; - lista (false, cel mai lung prefix comun între etichetă și
;   șablon), dacă șablonul nu s-a potrivit cu eticheta sau nu
;   s-a găsit din start o etichetă care începe cu litera dorită
;   (ex1: (false, "n") pentru șablonul "numai" și eticheta "na")
;   (ex2: (false, "") pentru etichetă negăsită)
; Obs: deși exemplele folosesc stringuri pentru claritate, vă
; reamintim că în realitate lucrăm cu liste de caractere.
(define (match-pattern-with-label st pattern)
  (cond 
    ((null? pattern) (list #f '()))
    ((empty? st) (list #f '()))
    ((equal? (get-branch-label (first-branch st)) pattern) #t) ;;verific daca eticheta este egala cu patternul
    ((equal? (car (longest-common-prefix (get-branch-label (first-branch st)) pattern)) pattern) #t) ;;verific daca patternul este prefixul comun
    ((and (not (null? (cauta (get-branch-label (first-branch st)) pattern '()))) (not(equal? (cauta (get-branch-label (first-branch st)) pattern '()) (get-branch-label (first-branch st))))) (list #f (cauta (get-branch-label (first-branch st)) pattern '()))) ;;verific daca patternul este prefixul comun dar nu este continut in eticheta (#f (b a))
    ((equal? (cauta (get-branch-label (first-branch st)) pattern '()) (get-branch-label (first-branch st))) (list (get-branch-label (first-branch st)) (sufix (get-branch-label (first-branch st)) pattern pattern) (cdr (first-branch st))))  ;;verific daca patternul este continut in eticheta ((a) (a n) (#$))
    ((not (equal? (get-branch-label (first-branch st)) pattern)) (match-pattern-with-label (other-branches st) pattern)))) ;;verific daca patternul nu este continut in eticheta


(define (cauta st pattern copy-pattern)
  (if (null? st)
    copy-pattern
    (if (null? pattern)
      copy-pattern
      (if (equal? (car st) (car pattern))
        (cauta (cdr st) (cdr pattern) (append copy-pattern (list (car pattern))))
        copy-pattern))))


(define (sufix st pattern suf)
  (if (null? st)
    suf
    (if (null? pattern)
      suf
      (if (equal? (car st) (car pattern))
        (sufix (cdr st) (cdr pattern) (cdr suf))
        suf))))
  



; TODO 5
; Implementați funcția st-has-pattern? care primește un
; arbore de sufixe și un șablon și întoarce true dacă șablonul
; apare în arbore, respectiv false în caz contrar.
(define (st-has-pattern? st pattern)
(cond
  ((null? pattern) #f)
  ((null? st) #f)
  ((equal? (get-branch-label (first-branch st)) pattern) #t) ;;verific daca eticheta este egala cu patternul
  ((equal? (car (longest-common-prefix (get-branch-label (first-branch st)) pattern)) pattern) #t) ;;verific daca patternul este prefixul comun
  ; ((equal? (cauta (get-branch-label (first-branch st)) pattern '()) (get-branch-label (first-branch st))) #t) ;; verific daca patternul este continut in eticheta
  ; ((> (length (get-branch-label (first-branch st))) (length pattern)) (cauta-pattern (get-branch-label (first-branch st)) pattern)) ;;verific daca patternul este prefixul comun
  ; ((and (not (null? (cauta (get-branch-label (first-branch st)) pattern '()))) (not(equal? (cauta (get-branch-label (first-branch st)) pattern '()) (get-branch-label (first-branch st))))) #t)
  ; ((equal? (cauta (get-branch-label (first-branch st)) pattern '()) pattern) #t) ;;verific daca patternul este continut in eticheta
  (else (st-has-pattern? (other-branches st) pattern)))
  )

(define (cauta-pattern st pattern copy-pattern)
  (if (null? st)
    copy-pattern
    (if (null? pattern)
      copy-pattern
      (if (equal? (car st) (car pattern))
        (cauta-pattern (cdr st) (cdr pattern) (append copy-pattern (list (car pattern))))
        (cauta-pattern (cdr st) pattern copy-pattern)))))




  