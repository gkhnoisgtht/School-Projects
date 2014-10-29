;; Anything you type in here will be executed
;; immediately with the results shown on the
;; right.

(defrecord Card [suit value])
(defrecord Player [pos name cards])
(defn ->Card [suit value]
  {:suit suit :value value})
(defn ->Player [pos name cards]
  {:pos pos :name name :cards cards})



(def suits ["hearts" "clubs" "diamonds" "spades"])
(def values (range 2 15))
(def deck (make-deck))
(def game {})


(defn shuffle-deck [deck]
  (shuffle deck))

(defn deal [deck n]
  (take n deck))

(defn heart? [card]
  (= :heart (:suit card)))

(defn print-card [card]
   (println-str (:value card) "of" (:suit card)))

(defn remove-cards [deck n]
  (drop n deck))

(defn hand [deck n]
  (let [cards (shuffle deck)
        hand (sorted(deal cards n))
        result (remove-cards cards n)]
    (def deck result) hand))

(defn make-deck []
  (for [suit suits
        value values]
            (->Card suit value)))

(defn create-deck []
  "Not the best approach but it is functional"
  (def deck (make-deck)))

(defn lead-suit [trick]
  (:suit (first trick)))

(defn win [trick]
  (let [first-suit (lead-suit trick)
        correct-suit (filter #(= first-suit (:suit %)) trick)
        ordered (sort-by :value > correct-suit)]
    (first ordered)))

(defn play-first-trick? [tricks]
  (< (count (first tricks)) 4))

(defn hearts-broken [tricks]
  (some #(some heart? %) tricks))

(defn can-lead-hearts? [tricks]
  (and ((complement play-first-trick?) tricks)
       (hearts-broken tricks)))

(defn print-hand [hand]
  (for [card hand]
    (print-card card)))

(defn play-computer-card [hand n]
  (let [card (nth hand n)]
  (drop-nth hand n)
    card))

(defn play-card [n]
  (let [card(nth (players-cards) n)]
    (drop-nth (players-cards) n)
    card))

(defn players-cards []
  (:cards (nth (:players game) 3)))

(defn make-players []
  (for [names {0 "Sue"
               1 "Frank"
               2 "Bob"
               3 "Player"}]
    (->Player (key names) (val names) (hand deck 13))))

(defn sorted [cards]
  (sort-by :suit (sort-by :value cards)))

(defn make-game []
  (make-deck)
  (def game {:players (make-players)
             :tricks []})
  (show-options))

(defn first-player [players]
  (let [twoofclubs (->Card "clubs" 2)]
  (some (fn [player]
          (when (some #(= twoofclubs %)
                      (:cards player))
            player))
        players)))

(defn last-trick-count [tricks]
  (count (last tricks)))

(defn current-trick [tricks]
  (let [played (last-trick-count tricks)]
    (when
      (and (> played 0)
           (< played 4))
        (last tricks))))


(defn next-player []
  (let [tricks (:tricks game)
        current (current-trick tricks)]
    (if (zero? (count tricks))
      (:pos (first-player (:players game)))
      (if (current)
        (mod (inc (:pos (last current))) 4)
        (:pos (win (last current)))))))


(defn show-cards []
  (let [cards (:cards (nth (:players game) 3))]
    (print-hand cards)))

(defn show-options[]
  (println "Created Hearts Game!\n
           To look at your hand type (show-cards)\n
           To start the game type (start-game)\n
           To place a card type (play-card n)\n
           - n is the number from 0 of your card\n
           To look at the current score type (score)\n
           To exit type (exit)\n\n")
  (show-cards))

(defn exit []
  (System/exit 0))

(defn rank [value]
  (if (< value 11)
    (str value)
    (case value
      11 "Jack"
      12 "Queen"
      13 "King"
      14 "Ace")))


(count (:cards (nth (:players game) 0)))
(defn print-player-turn []
  (println "Since you have the 2 of Spades you go first\n
           Please play the 2 of Spades as your first card"))


(defn start-game []
  (let [first (next-player)]
    (if (= first 3)
      (print-player-turn)
      (ai-turn (:cards (nth (:players game) first))))))

(make-game)
(start-game)

(defn ai-turn [player]
  (if (= (count (:tricks game)) 0)
    (play-computer-card (:cards player) 0)
    (play-computer-card (:cards player)
                        (rand
                         (count
                          (:cards player))))
    ))

(defn drop-nth
  [n coll]
  (->> coll
       (map vector (iterate inc 1))
       (remove #(zero? (mod (first %) n)))
       (map second)))
(show-cards)

(play-card 0)

(show-cards)
(def cards (nth(:players game)3))
(def new-cards (drop 1 (:cards cards)))
(defn newdrop [col]
  (drop 1 col))

(update-in cards [:cards] newdrop)

(defn indexes-of [e coll] (keep-indexed #(if (= e %2) %1) coll))

(defn index-of [e coll] (first (keep-indexed #(if (= e %2) %1) coll)))


(indexes-of (->Card "clubs" 2) deck)

(def decks (atom nil))

(defn make-decks [deck]
  (make-deck))
(swap! decks make-decks)


(swap! decks (fn [deck] (drop 5 deck) ))

