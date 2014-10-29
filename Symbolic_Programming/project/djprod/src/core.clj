(defn make-game []
  (make-deck)
  (def game {:players (make-players)
             :tricks []}))

(defn first? [players]
  (let [twoofclubs (->Card "clubs" 2)]
  (some (fn [player]
          (when (some #(= twoofclubs %)
                      (:cards player))
            player))
        players)))
