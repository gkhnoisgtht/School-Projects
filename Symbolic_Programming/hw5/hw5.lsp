;suits and values for use with other functions
(defparameter *suits* '(spades diamonds hearts clubs))
(defparameter *values* '(Ace King Queen Jack 10 9 8 7 6 5 4 3 2))

(defclass card ()
  ((suit :initarg :suit :accessor card-suit)
   (value :initarg :value :accessor card-value))
  (:documentation "A Simple Card Class"))

(defclass deck ()
  ((cards :initarg :cards :accessor cards))
  (:documentation "A Simple Deck Class"))

(defun make-deck ()
	"Makes a deck returning all 52 cards"
    (mapcan #'(lambda (suit)
                (mapcar #'(lambda (value)
                            (make-card suit value))
                  *values*))
      *suits*))
				
(defun add-card (card deck)
	"pushes new card into deck"
	(push card (cards deck)))

(defun make-card (suit value)
	"Makes a new card with suit and value"
  (make-instance 'card :suit suit :value value))

(defun print-card (card)
	"Prints out card in format Value of Suit"
  (format t "~%~A of ~A" (card-value card)(card-suit card)))

(defun count-values (value hand)
	"Counts the specific values in the deck or hand"
  (loop for card in hand
        count (eq (card-value card) value)))

(defun prob-for-value (value hand deck)
	"calculates the probability that the next card drawn will be of the specific value"
  (float (/ (- 4 (count-values value hand)) 
            (length (cards deck)))))

(defun prob-for-suit (hand suit deck)
	"calculates the probability that the next card drawn will be of the specific suit"
  (float (/ (- 13 (count-values suit hand)) 
            (length (cards deck)))))

(defun deal-cards (deck &optional (num 1))
	"Deals the cards from provided deck, num is given to output the size of the hand"
  (cond  ((zerop num) nil)
		 (t (cons (pop (cards deck))
		       (deal-cards deck (1- num))))))