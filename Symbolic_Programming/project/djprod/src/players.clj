(defrecord Player [pos name cards])
(defn ->Player [pos name cards]
  {:pos pos :name name :cards cards})

(defn make-players []
  (for [names {1 "Sue"
               2 "Frank"
               3 "Bob"
               4 "Player"}]
    (->Player (key names) (val names) (core/hand core/deck 13))))
