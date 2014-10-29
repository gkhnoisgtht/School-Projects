(defun primep (a)
	"call helper function"
	(primep-2 a 2))

(defun primep-2 ( val iter)
	"helper function to recursively look to disprove prime number"
	(cond 
		((> iter (/ val 2)) t)
		((eq (mod val iter) 0) nil)
		(t (primep-2 val (1+ iter)))))