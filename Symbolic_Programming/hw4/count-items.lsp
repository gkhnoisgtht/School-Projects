(defun count-items (deck item)
               (loop for i in deck 
                   count (eq (card-value i) item)))
