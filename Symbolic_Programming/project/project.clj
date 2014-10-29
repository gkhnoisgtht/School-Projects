;; Anything you type in here will be executed
;; immediately with the results shown on the
;; right.
(ns project)
(defrecord Card [suit value])
(defrecord Player [pos name cards])
(defn ->Card [suit value]
  {:suit suit :value value})
(defn ->Player [pos name cards]
  {:pos pos :name name :cards cards})


;; Values
(def suits ["clubs" "spades" "diamonds" "hearts"])
(def values (range 2 15))
(def deck (atom nil))
(def game (atom nil))
(def current-trick (atom nil))
(def current (atom 0))
(def plays (atom 0))
(def tricks (atom nil))

;; Card functions
(defn rank [value]
  (if (< value 11)
    (str value)
    (case value
      11 "Jack"
      12 "Queen"
      13 "King"
      14 "Ace")))

(defn create-deck [this]
  (for [suit suits
        value values]
          (->Card suit value)))

(defn remove-card [n]
  (swap! deck (fn [deck] (drop n deck))))

(defn print-card [card]
   (println-str (rank (:value card)) "of" (:suit card)))

(defn heart? [card]
  (= :heart (:suit card)))

(defn shuffle-deck []
  (shuffle @deck))

(defn lead-suit [trick]
  (:suit (first tricks)))

(defn win [trick]
  (let [first-suit (lead-suit trick)
        correct-suit (filter #(= first-suit (:suit %)) trick)
        ordered (sort-by :value > correct-suit)]
    (first ordered)))

(defn sorted [cards]
  (sort-by :suit (sort-by :value cards)))

(defn deal [n new-deck]
  (take n new-deck))

(defn remove-cards [n new-deck]
  (drop n new-deck))

(defn create-hand [n]
  (let [cards (shuffle-deck)
        hand (sorted(deal n cards))
        result (remove-cards n cards)]
    (reset! deck result)
    hand))


;; Helper functions
(defn drop-nth [n coll]
  "helper function to remove cards from hand"
  (->> coll
       (map vector (iterate inc 1))
       (remove #(zero? (mod (first %) n)))
       (map second)))

(defn index-of [e coll]
  (first (keep-indexed #(if (= e %2) %1) coll)))


;; functions to show current players hand
(defn print-hand [hand]
  (for [card hand]
    (print-card card)))

(defn show-cards []
  (let [cards (:cards (nth @game 3))]
    (print-hand cards)))


;; function for hearts leading
(defn play-first-trick? [tricks]
  (< (count (first tricks)) 4))

(defn hearts-broken [tricks]
  (some #(some heart? %) tricks))

(defn can-lead-hearts? [tricks]
  (and ((complement play-first-trick?) tricks)
       (hearts-broken tricks)))


;; Player Functions ---------------------------------------------------------------------------------------------------------
(defn create-players []
  (let [names ["Sue"
               "Frank"
               "Bob"
               "Player"]
        create-p (fn [names pos](->Player pos names (create-hand 13)))]
    (vec (map create-p names (range)))))

;; Game Functions -----------------------------------------------------------------------------------------------------------
(defn show-options[]
  (println "Created Hearts Game!\n
           To look at your hand type (show-cards)\n
           To start the game type (start-game)\n
           To place a card type (play-card n)\n
           - n is the number from 0 of your card\n
           To look at the current score type (score)\n
           To exit type (exit)\n\n")
  )

(defn exit []
  (System/exit 0))

(defn create-game []
  (swap! deck create-deck)
  (let [players  (create-players)]
  (reset! game players))
  (show-options))

(defn first-player [players]
  (let [twoofclubs (->Card "clubs" 2)]
  (some (fn [player]
          (when (some #(= twoofclubs %)
                      (:cards player))
            player))
        players)))

(defn next-player []
  (if (zero? (count @tricks))
    (:pos (first-player @game))
    (:pos (win (last @tricks)))))

(defn play-computer-card [hand n]
  (let [card (nth hand n)]
  (drop-nth n hand)
    card))

(defn play-next-trick? []
  (println "Would you like to play the next hand?\n
           type (play-again) to continue\n"))

(defn add-trick [trick]
  (swap! tricks conj trick))

(defn add-card-to-trick [card]
  (swap! current-trick conj card))

(defn end-trick []
  (for [trick @current-trick]
    (add-trick trick)))

(defn inc-current []
  (if (= @current 3)
    (reset! current 0)
    (swap! current inc))
  (if (< @plays 3)
    (swap! plays inc)
    ((reset! plays 0)
     (end-trick)
     (play-next-trick?))))

(defn print-player-turn []
  (inc-current)
  (println "Your Turn"))

(defn ai-turn []
  (inc-current)
  (println "ai - turn"))

(defn play []
  (if (= @current 3)
    (print-player-turn)
    ((ai-turn)
     (play))))

(defn play-again []
  (next-player)
  (play))

(defn start-game []
  (reset! current (next-player))
  (reset! plays 0)
  (play))

(defn get-player-cards []
  (:cards (nth @game 3)))

;;(defn play-card [n]
;; (let [card (nth (get-player-cards) n)]
;;    (drop-user-card n (get-player-cards))
;;    card))

;; TODO
;;
;; drop cards from @game
;;
;; display trick as it runs
;;
;; AI for computer
;; - get highest card for lead suit
;; - if not play heart, or queen of spades
;; - otherwise random - no heart on first play
;;
;;












