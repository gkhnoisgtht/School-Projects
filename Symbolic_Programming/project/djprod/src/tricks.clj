(defrecord Trick [player pos cards])
(defn ->Tricks [player pos cards]
  {:player player :pos pos :cards cards})

(defn Tricks ())

(defn create-trick [player pos cards]
  (->Tricks player pos cards))

(defn add-trick [player pos cards]
  (let [new-trick (create-trick player pos cards)]
    (swap! conj Tricks)))

