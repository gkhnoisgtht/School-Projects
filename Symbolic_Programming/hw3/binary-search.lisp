(defun binary-search (item lis)
	"Search through binary tree, return t on finding the element, nil otherwise"
	(cond 
		((null lis) nil)
		((eq item (car lis)) t)
		((atom (car lis)) (binary-search item (cdr lis)))
		(t (or 	
			(binary-search item (car lis))
			(binary-search item (cdr lis))))))