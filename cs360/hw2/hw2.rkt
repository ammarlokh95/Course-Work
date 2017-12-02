;;
;; DO NOT REMOVE THESE TWO LINES
;;
#lang racket
(provide (all-defined-out))

;;
;; Problem 1
;;
;; Retruns a function that iterates over a sequence of type (start step end)
;; printing the next element in the sequence on each call
;; 
;; Example: (let ((next (iterator '(0 2 7))))) (repeat next 5)) => '(0 2 4 6 ())
;;          (let ((next (iterator '(3 3 15)))) (repeat next 5)) => '(3 6 9 12 15 ())

(define (iterator rlist)
  (let (
        (start (car rlist))
        (step (cadr rlist))
        (end (caddr rlist))
        (print (car rlist))
        )
     (lambda () (if (> start end)
                    '()
                    (begin
                      (set! print start)
                      (set! start (+ start step))
                           print))))) 
;;
;; Problem 2
;;
;;Stream-seq executes the fuction f given as argument for a particular element of the
;; stream produced from the sequence of type (start step end) from rlist.
;; Helper functions calc and func are used to carry out the procedure.
;;
;; Example: (stream-first (stream-rest (stream-seq square '(0 2 7)))) => 4
;;          (stream-first (stream-rest (stream-rest (stream-seq square '(0 2 7))))) => 16
;;          (stream-first (stream-rest (stream-seq square '(3 3 15)))) => 36



(define (calc start step end)
  (if (> start end)
      empty-stream
      (stream-cons start (calc (+ start step) step end))))

(define (func f l)
  (if (stream-empty? l)
      empty-stream
      (stream-cons (f (stream-first l)) (func f (stream-rest l)))))

(define (stream-seq f rlist)
  (let (
        (start (car rlist))
        (step (cadr rlist))
        (end (caddr rlist))
        )
    (func f (calc start step end))))
          
;;
;; Problem 3
;;
;; (scan f z l) takes a binary function f, a value z and a list l and returns a list of the type
;; z, f(X1,z), f(X2 ,f(X1,z)),..., f(Xn,f(Xn-1,...)) where X1,X2... Xn are elements of the list l
;; reduce is a helper function that takes the same arguments and returns the list to be printed.
;;
;; Examples:
;;> (scan * 1 '(1 2 3 4 5 6)) => (1 1 2 6 24 120 720)
;;> (scan + 0 '(1 2 3 4 5 6)) => (0 1 3 6 10 15 21)
;;> (scan + 0 '()) => (0)

(define (reduce f z l)
  (if (null? l)
      (cons z '())
      (cons z (reduce f (f z (car l)) (cdr l)))))
(define (scan f z l)
  (if (null? l)
      (cons z '())
      (reduce f z l)))

;;
;; Problem 4
;;
;; Stream-scan creates a stream with values of the type z, f(X1,z),..., f(Xn,(Xn-1,...))
;; where X1, X2,..., Xn are elements of list l, f is a binary function and z is a value.
;; stream-reduce is a helper funtion that is used to create the stream.
;;
;; Examples:
;;> (stream-first (stream-rest (stream-scan * 1 '(1 2 3 4 5)))) => 1
;;> (stream-first (stream-rest (stream-rest (stream-scan + 0 '(1 2 3 4 5))))) => 3

(define (stream-reduce f z l)
  (if (stream-empty? l)
      (stream-cons z '())
      (stream-cons z (stream-reduce f (f z (stream-first l)) (stream-rest l)))))
(define (stream-scan f z l)
  (if (stream-empty? l)
      (stream-cons z '())
      (stream-reduce f z l)))

;;
;; Problem 5
;;
;; lookup takes a name and an association list (which is a list of the form:
;;  ((name1 val1) (name2 val2) .... (namen valn))
;; lookup returns the binding pair (name and value) of the element of the alist with
;; the same name as the one passed as argument to the funtion.
;; if not binding is found it returns nil
;;
;; Examples:
;; >(define assocs '((ben "short") (cara "walking") (dan "bald")))
;; > (lookup 'ben assocs) => '(ben "short")
;; > (lookup 'cara assocs) => '(cara "walking")
;; > (lookup 'random assocs) => '()

(define (lookup name alist)
  (cond
    ((null? alist) null)
    ((equal? name (car (car alist))) (car alist))
    (else (lookup name (cdr alist)))))
;;
;; Problem 6
;;
;; lookup-env takes as argument a name to lookup and an env which is a list of association lists.
;; The fuction sequentially goes through the lists in order and returns the first binding pair
;; (name and value) with the same name as the one passed as argument to the funtion.
;; If no binding is found it returns nil.
;;
;; Examples:
;;>(define l1 '((ben "short") (cara "walking") (dan "bald")))
;; (define l2 '((albert "is not") (ski "skinny") (kim "cook") (cara "injured")))
;; (define e (list l1 l2) )
;; (lookup-env 'ski e) => '(ski "skinny")
;; (lookup-env 'cara e) => '(cara "walking")
;; (lookup-env 'benn e) => '()

(define (lookup-env name env)
  (cond
    ((null? env) null)
    ((null? (lookup name (car env))) (lookup-env name (cdr env)))
    (else (lookup name (car env)))))

;;
;; Problem 7: Homework Statistics
;;
;; Problem 1 : 30 minutes
;; Problem 2 : 15 minutes 
;; Problem 3 : 10 minutes
;; Problem 4 : 15 minutes
;; Problem 5 : 10 minutes
;; Problem 6 : 10 minutes