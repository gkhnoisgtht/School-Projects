;; Anything you type in here will be executed
;; immediately with the results shown on the
;; right.
;; Anything you type in here will be executed
;; immediately with the results shown on the
;; right.
(ns project
  (:require [clojure.core]))
;; Card and Player setup
(defrecord Card [suit value])
(defrecord Player [pos name cards])
(defn ->Card [suit value]
  {:suit suit :value value})
(defn ->Player [pos name cards]
  {:pos pos :name name :cards cards})


;; Atoms for keeping state through transactions
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
  "Pretty display of the rank"
  (if (< value 11)
    (str value)
    (case value
      11 "Jack"
      12 "Queen"
      13 "King"
      14 "Ace")))

(defn create-deck [this]
  "Creates the deck"
  (for [suit suits
        value values]
          (->Card suit value)))

(defn remove-card [n]
  "Removes card from deck"
  (swap! deck (fn [deck] (drop n deck))))

(defn print-card [card]
  "Prints the card in a pretty format"
   (println-str (rank (:value card)) "of" (:suit card)))

(defn heart? [card]
  "Returns true if the card is a heart"
  (= "hearts" (:suit card)))

(defn shuffle-deck []
  "Shuffles the deck"
  (shuffle @deck))

(defn lead-suit [trick]
  "Returns the lead suit of the trick"
  (:suit (first tricks)))

(defn win [trick]
  "Displays the winner of the trick"
  (let [first-suit (lead-suit trick)
        correct-suit (filter #(= first-suit (:suit %)) trick)
        ordered (sort-by :value > correct-suit)]
    (first ordered)))

(defn sorted [cards]
  "Sorts the deck displayed to the user"
  (sort-by :suit (sort-by :value cards)))

(defn deal [n new-deck]
  "Takes n cards from the deck"
  (take n new-deck))

(defn remove-cards [n new-deck]
  "Removes the cards from the given deck"
  (drop n new-deck))

(defn create-hand [n]
  "Creates the players hand"
  (let [cards (shuffle-deck)
        hand (sorted(deal n cards))
        result (remove-cards n cards)]
    (reset! deck result)
    hand))

(defn two-of-clubs? [card]
  "Checks to see if the given card is the two of clubs"
  (let [two (->Card "clubs" 2)]
  (=  two card)))

(defn drop-nth [n coll]
  "helper function to remove cards from hand"
  (->> coll
       (map vector (iterate inc 1))
       (remove #(zero? (mod (first %) n)))
       (map second)))

(defn index-of [e coll]
  "Returns the index of the value in a vector"
  (first (keep-indexed #(if (= e %2) %1) coll)))


(defn print-hand [hand]
  "Cycles through all cards in the hand and prints them out"
  (for [card hand]
    (print-card card)))

(defn show-cards []
  "Shows the current players cards"
  (let [cards (:cards (nth @game @current))]
    (print-hand cards)))


(defn first-trick? []
  "Returns true if this is the first trick in the set"
  (= (count (first @tricks)) 0))

(defn hearts-broken []
  "Returns true if hearts have been broken in this set of tricks"
  (some #(some heart? %) @tricks))

(defn can-lead-hearts? []
  "Returns true if hearts have been broken and it is not the first trick in the set"
  (and ((complement first-trick?))
       (hearts-broken)))

(defn begining-of-trick? []
  "Returns true if this is the begining of the trick"
  (= (count @current-trick) 0))


(defn create-players []
  "Creates players and places them in a vector"
  (let [names ["Player 1"
               "Player 2"
               "Player 3"
               "Player 4"]
        create-p (fn [names pos](->Player pos names (create-hand 13)))]
    (vec (map create-p names (range)))))

;; Game Functions
(defn show-options[]
  "Shows options to the user"
  (println "Created Hearts Game!\n
           To look at your hand type (show-cards)\n
           To start the game type (start-game)\n
           To place a card type (play-card n)\n
           - n is the number from 0 of your card\n
           To see this list again type (show-options)\n
           To exit type (exit)\n\n"))

(defn exit []
  "Exits the game"
  (System/exit 0))

(defn create-game []
  "creates the game for the user to play"
  (swap! deck create-deck)
  (let [players  (create-players)]
  (reset! game players))
  (show-options))

(defn first-player [players]
  "Finds the first person to play in a trick"
  (let [twoofclubs (->Card "clubs" 2)]
  (some (fn [player]
          (when (some #(= twoofclubs %)
                      (:cards player))
            player))
        players)))

(defn next-player []
  "returns the next player in the trick set"
  (if (zero? (count @tricks))
    (:pos (first-player @game))
    (:pos (win (last @tricks)))))

(defn play-next-trick? []
  "Asks if the user would like to play the next trick in the set"
  (println "Would you like to play the next hand?\n
           type (play-again) to continue\n"))

(defn add-trick [trick]
  "Adds the trick to the total number of tricks"
  (swap! tricks conj trick))

(defn display-trick []
  "displays the cards in the trick"
  (print-hand @current-trick))

(defn add-card-to-trick [card]
  "Adds card to the current trick"
  (swap! current-trick conj card)
  (display-trick))

(defn end-trick []
  "Cleans up the tricks"
  (for [trick @current-trick]
    (add-trick trick)))

(defn inc-current []
  "Increment the current player in the trick"
  (if (= @current 3)
    (reset! current 0)
    (swap! current inc))
  (if (< @plays 3)
    (swap! plays inc)
    ((reset! plays 0)
     (end-trick))))

(defn print-player-turn []
  "Prints the turn of the current player"
  (println (str (:name (nth @game @current)) " Turn")))

(defn play []
  "function to simulate a round within a trick"
  (print-player-turn))

(defn play-again []
  "Plays next trick"
  (next-player)
  (play))

(defn start-game []
  "Actually starts the game after the game has been created"
  (reset! current (next-player))
  (reset! plays 0)
  (play))

(defn get-player-cards []
  "Displays the current players cards"
  (:cards (nth @game @current)))

(defn update-game [n]
  "Updates the cards for each player after they have drawn a card"
  (let [cards (nth @game @current)
        final-cards (update-in cards [:cards] (fn [cards] (drop-nth n (:cards cards))))]
    (swap! game (fn [game] (assoc game @current final-cards)))))

(defn card-valid [card]
  "Validates the drawn card"
  (if (first-trick?)
    (if (begining-of-trick?)
      (if (two-of-clubs? card)
        true
        false)
      (if (heart? card)
        false
        true))
    (if (and (can-lead-hearts?)
             (heart? card))
      true
      false)))

(defn winning-player [player]
  (println (str player " won this round!")))

;; play next trick
(defn display-winner []
  "Returns winning player name"
  (let [winning-card (win (last @tricks))
        index (index-of winning-card (last @tricks))
        player @current
        init (+ player index)]
    (if (> init 3)
      (winning-player (:name (nth @game (- init 4))))
      (winning-player (:name (nth @game init))))))


(defn play-card [n]
  "Plays the card selected"
 (let [card (nth (get-player-cards) n)]
    (update-game n)
   (if (card-valid card)
    ((add-card-to-trick card)
      (inc-current)
      (if (not= @plays 0)
        (play)
        (display-winner)))
     (println "Card not valid, try again"))))




