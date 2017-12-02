;;
;; DO NOT REMOVE THESE TWO LINES
;;
#lang racket
(provide (all-defined-out))

;;
;; Problem 1
;;
(define (atom? l)
  (not (or (pair? l) (null? l))))

(define (deep-count l)
  (cond 
     ((null? l) 0)
     ((atom? (car l)) (+ 1 (deep-count (cdr l))))
     (else (+ (deep-count (car l)) (deep-count (cdr l))))
))

;;
;; Problem 2
;;

(define (rev-help l x)
  (if (null? l)
      x
      (rev-help (cdr l) (cons (car l) x))
      ))

(define (reverse l)
  (rev-help l '()))

;;
;; Problem 3
;;
(define (calc start step end)
  (if (> start end)
      null
      (cons start (calc (+ start step) step end))))

(define (range rlist)
  (let (
        (start (car rlist))
        (step (cadr rlist))
        (end (caddr rlist))
        )
      (calc start step end)))

;;
;; Problem 4
;;
(define (func f l)
  (if (null? l)
      null
      (cons (f (car l)) (func f (cdr l)))))
(define (seq f rlist)
  ( let (
         (start (car rlist))
         (step (cadr rlist))
         (end (caddr rlist))
         )
     (func f (calc start step end))))
  
;;
;; Problem 5: Homework Statistics
;;
;;problem 1:5 mins
;;problem 2:30-45 mins
;;problem 3: 15 mins
;;problem 4: 10 mins
