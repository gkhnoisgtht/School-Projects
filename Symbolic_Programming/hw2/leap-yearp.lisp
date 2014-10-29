(defun leap-yearp (year)
	(if (= (mod year 100) 0)
		(if (= (mod year 400) 0)
			t
			nil)
		(if (= (mod year 4) 0)
			t
			nil)))
(leap-yearp 1996)
(leap-yearp 1995)
(leap-yearp 1990)
(leap-yearp 2000)

