(defun remove-player (fouls minutes)
	(cond ((= fouls 5) t)
		((and (= fouls 4)(>= minutes 5)) t)
		((and (>= fouls 3)(>= minutes 15)) t)
		((and (>= fouls 2)(>= minutes 28)) t)))

(remove-player 4 2)                        

(remove-player 3 30)
 
(remove-player 2 20)

(remove-player 2 30)

(remove-player 3 20)
