#lang racket
(require "suffix-tree-stream.rkt")
(require "collection.rkt")

(provide (all-defined-out))

;; Vom prelua toate funcțiile din etapele 1-3 (exceptând
;; longest-common-substring, care nu beneficiază de 
;; reprezentarea ca flux întrucât parcurge tot arborele)
;; și le vom adapta la noua reprezentare a unui ST.
;;
;; Pentru că un ST este construit pornind de la o colecție
;; de sufixe și pentru că ne dorim să nu calculăm toate
;; sufixele decât dacă este nevoie, vom modifica toate
;; funcțiile care prelucrau liste de sufixe pentru a
;; prelucra fluxuri de sufixe.
;;
;; Obs: fără această modificare a listelor de sufixe în
;; fluxuri de sufixe, și presupunând că am manipulat
;; arborii de sufixe doar prin interfața definită în
;; fișierul suffix-tree (respectând astfel bariera de 
;; abstractizare), ar trebui să alterăm doar funcția 
;; suffixes->st care este practic un constructor pentru
;; tipul ST.
;; Din cauza transformării listelor de sufixe în fluxuri,
;; avem mult mai multe implementări de modificat.
;; Puteam evita acest lucru? Da, utilizând conceptul de
;; colecție de sufixe de la început (în loc să presupunem
;; că ele vor fi prelucrate ca liste). În loc de cons,
;; car, cdr, map, filter, etc. am fi folosit de fiecare
;; dată collection-cons, collection-first, ... etc. -
;; aceste funcții fiind definite într-o bibliotecă
;; inițială ca fiind echivalentele lor pe liste, și
;; redefinite ulterior în stream-cons, stream-first,
;; ... etc. Operatorii pe colecții de sufixe ar fi 
;; folosit, desigur, doar funcții de tip collection-.
;;
;; Am ales să nu procedăm astfel pentru că ar fi provocat
;; confuzie la momentul respectiv (când chiar operatorii
;; pe liste erau o noutate) și pentru a vă da ocazia să
;; faceți singuri acest "re-design".


; TODO
; Copiați din etapele anterioare implementările funcțiilor
; de mai jos și modificați-le astfel:
; - Toate funcțiile care lucrează cu liste de sufixe vor
;   lucra cu un nou tip de date Collection, ai cărui
;   constructori și operatori vor fi definiți de voi
;   în fișierul collection.rkt.
; - Pentru toate funcțiile, trebuie să vă asigurați că
;   este respectată bariera de abstractizare (atât în 
;   cazul tipului ST cât și în cazul tipului Collection).
; Obs: cu cât mai multe funcții rămân nemodificate, cu atât
; este mai bine (înseamnă că design-ul inițial a fost bun).

(define (longest-common-prefix w1 w2)
  (longest-common-prefix-helper w1 w2 '()))

(define (longest-common-prefix-helper w1 w2 acc)
  (cond
    ((or (null? w1) (null? w2)) (list acc w1 w2))
    ((equal? (car w1) (car w2)) (longest-common-prefix-helper (cdr w1) (cdr w2) (append acc (list (car w1)))))
    (else (list acc w1 w2))))


; am schimbat, în numele funcției, cuvântul list în
; cuvântul collection
(define (longest-common-prefix-of-collection words)
  (if (collection-empty? words)
      empty-stream
      (if (= (collection-length words) 1)
          (collection-stream-first words)
          (collection-stream-first (longest-common-prefix (collection-stream-first words) (longest-common-prefix-of-collection (collection-stream-rest words)))))))


(define (match-pattern-with-label st pattern)
  'your-code-here)


(define (st-has-pattern? st pattern)
 
  (define match (match-pattern-with-label st pattern))
 
  (cond ((equal? match #t) #t)
        ((equal? (first match) #f) #f)
        (else (st-has-pattern? (third match) (second match)))))


(define (get-suffixes text)
  (if (collection-empty? text)
    empty-stream
    (collection-cons text (get-suffixes (collection-stream-rest text)))))

(define (get-ch-words words ch)
  (collection-filter (lambda (word) (and (not (collection-empty? word)) (equal? (collection-stream-first word) ch))) words))


(define (ast-func suffixes)
  (cons (list (collection-stream-first (collection-stream-first suffixes))) (collection-map (lambda (suffix) (collection-stream-rest suffix)) suffixes))) ;cons intre prima litera si restul 

(define (cst-func suffixes)
  (let ((common-prf (longest-common-prefix-of-collection suffixes)))
    (cons common-prf (collection-map (lambda (suffix) (drop suffix (length common-prf))) suffixes)))) ;cons intre prefixul comun si restul

; considerați că și parametrul alphabet este un flux
; (desigur, și suffixes este un flux, fiind o colecție
; de sufixe)



(define (suffixes->st labeling-func suffixes alphabet)
  (letrec ((func (lambda (ch)
      (let ((ch-words (get-ch-words suffixes ch)))
        (if (not (collection-empty? ch-words))
            (let ((new-suf (labeling-func ch-words)))
              (cons (car new-suf) 
                          (suffixes->st labeling-func (cdr new-suf) alphabet)))
            empty-stream)))))
    (collection-filter (lambda (f) (pair? f)) (collection-map func alphabet))))



; nu uitați să convertiți alfabetul într-un flux
(define (list->stream L)
  (if (null? L)
      empty-stream
      (stream-cons (car L) (list->stream (cdr L)))))

(define (text->st labeling-func)
  (lambda (text)
    (let ((n-s (get-suffixes (append text (list #\$))))) ;prima - obtinem suf si adaugam $ la final
      (suffixes->st labeling-func n-s (list->stream (remove-duplicates (sort (append text (list #\$)) char<?)))))))

(define text->ast
  (text->st ast-func))


(define text->cst
  (text->st cst-func))


; dacă ați respectat bariera de abstractizare,
; această funcție va rămâne nemodificată.
(define (substring? text pattern) 
  'your-code-here)


; dacă ați respectat bariera de abstractizare,
; această funcție va rămâne nemodificată.
(define (repeated-substring-of-given-length text len)
  (let loop ((st (text->cst text)) (need-len len) (result '()))
    (cond ((collection-empty? st) #f)
          ((<= need-len 0) (take result len))
          (else
           (let* ((branch (first-branch st)) (label (get-branch-label branch)) (subtree (get-branch-subtree branch)))
             (or (loop subtree (- need-len (length label)) (append result label))
                 (loop (other-branches st) need-len result)))))))
 