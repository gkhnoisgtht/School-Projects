(ns cards
  (:use [clojure.core]))

(defrecord Card [suit value])
(defn ->Card [suit value]
  {:suit suit :value value})


(def suits ["hearts" "clubs" "diamonds" "spades"])
(def values (range 2 14))
(def deck (make-deck))


(defn shuffle-deck [deck]
  (shuffle deck))

(defn deal [deck n]
  (take n deck))

(defn heart? [card]
  (= :heart (:suit card)))

(defn print-card [card]
  (str (:value card) "-of-" (:suit card)))

(defn remove-cards [deck n]
  (drop n deck))

(defn hand [deck n]
  (let [cards (shuffle deck)
        hand (deal cards n)
        result (remove-cards cards n)]
    (def deck result) hand))

(defn make-deck []
  (for [suit suits, value values]
            (->Card suit value)))

(defn create-deck []
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

(defn play-card [hand n]
  (nth hand n))

(defn print-hand [hand]
  (for [card hand]
    (print-card card)))


