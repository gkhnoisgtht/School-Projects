(defun points-per-quarter (&rest args)
  (/ (apply #' + args) (length args)))

(points-per-quarter 0 9 9 0 1 2)

(points-per-quarter 0 14 13 7 3 3 0)