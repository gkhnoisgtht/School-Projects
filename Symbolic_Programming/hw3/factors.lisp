
(defun factors (a)
	"call helper function"
	(factors-2 a 2))

(defun factors-2 ( val iter)
	"helper function to find factors for a given number"
	(cond 
		((> (/ val 2) iter) nil)
	    ((eq (mod val iter) 0) 
			(cons iter (factors-2 val (1+ iter))))
	    (t (factors-2 val (1+ iter)))))