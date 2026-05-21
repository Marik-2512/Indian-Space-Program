# Indian Space Program: Tire Launch

## O projekcie
Prosta gra 2D z interfejsem podobnym do gier ekonomiczno-zręcznościowych. Rozgrywka polega na mini-grze zręcznościowej (Quick-Time Event): gracz ładuje pasek mocy i musi wcisnąć przycisk w idealnym momencie. Wystrzelona opona leci w górę, zbijając przelatujące obiekty (ptaki, drony), za co gracz otrzymuje monety. W sklepie można kupić lepsze silniki, opony i nowe lokacje.

## Skład zespołu
* **Marat Yakunou** - Mechanika, Obiekty, Algorytmy QTE.
* **Daniil Zvanarou** - Interfejs, Sklep, Grafika i Szablony.

## Technologie i Wymagania
* **Język i Biblioteka:** C++ / SFML.
* **Architektura:** Wykorzystanie dziedziczenia (klasa `Equipment` -> `Motor`, `Tire`) oraz polimorfizmu.
* **Szablony:** Klasa kontenerowa `Inventory<T>`.
* **Zapis stanu:** Dane zapisywane do pliku `.txt` / `.json`.

## Assety i Licencje
*Wszystkie materiały graficzne i dźwiękowe użyte w projekcie są licencjonowane do użytku darmowego:*
* Grafika: [Wkrótce dodamy źródła]
* Dźwięk: [Wkrótce dodamy źródła]
