(defun return-items (deck item)
	(loop for i in deck 
		when (eq item (card-suit i))
			collect i))