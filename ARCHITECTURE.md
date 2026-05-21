\# Architektura Projektu i Struktura Klas



Dokumentacja techniczna przedstawiająca strukturę klas, mechanizmy Programowania Obiektowego (OOP) oraz wykorzystanie szablonów w projekcie "Indian Space Program: Tire Launch".



\## 1. Hierarchia Klas Ekwipunku (Dziedziczenie i Polimorfizm)

Wszystkie przedmioty dostępne w sklepie dziedziczą po wspólnej klasie bazowej, co pozwala na dynamiczne zarządzanie asortymentem i ułatwia rozbudowę gry.



\* \*\*`class Equipment`\*\* (Abstrakcyjna klasa bazowa)

&#x20;   \* \*Pola:\* `std::string name`, `int price`, `bool isUnlocked`

&#x20;   \* \*Metody:\* `virtual float getPowerMultiplier() = 0;` (Czysta funkcja wirtualna zapewniająca polimorfizm).

\* \*\*`class Motor : public Equipment`\*\* (Klasa pochodna)

&#x20;   \* \*Opis:\* Nadpisuje funkcję `getPowerMultiplier()`, zwracając bonus do prędkości początkowej wystrzału opony.

\* \*\*`class Tire : public Equipment`\*\* (Klasa pochodna)

&#x20;   \* \*Opis:\* Nadpisuje funkcję `getPowerMultiplier()`, zwracając bonus do aerodynamiki (zmniejszenie oporu powietrza podczas lotu).



\## 2. Obiekty Gry i Cele (Elementy Wizualne SFML)

Klasy odpowiedzialne за renderowanie elementów na ekranie oraz obsługę ich logiki i przemieszczania się.



\* \*\*`class GameObject`\*\* (Klasa bazowa dla obiektów graficznych)

&#x20;   \* \*Pola:\* `sf::Sprite sprite`, `sf::Vector2f position`

&#x20;   \* \*Metody:\* `virtual void update(float deltaTime)`, `virtual void draw(sf::RenderWindow\& window)`

\* \*\*`class Target : public GameObject`\*\* (Klasa bazowa dla celów/przeszkód)

&#x20;   \* \*Pola:\* `int coinReward` (nagroda w monetach przyznawana graczowi za zestrzelenie obiektu).

\* \*\*`class Bird : public Target`\*\* oraz \*\*`class Drone : public Target`\*\* (Klasy pochodne)

&#x20;   \* \*Opis:\* Definiują unikalne algorytmy zachowania i ruchu (np. lot ptaka po sinusoidzie, zawis drona w stałym miejscu).



\## 3. Szablony i Menedżery Logiki

Spełnienie zaawansowanych wymagań projektowych dotyczących kontenerów szablonowych i mechaniki mini-gier.



\* \*\*`template <typename T> class Inventory`\*\* (Klasa szablonowa)

&#x20;   \* \*Opis:\* Uniwersalny kontener szablonowy przechowujący zakupione przedmioty gracza (np. osobne instancje dla `Inventory<Motor>` oraz `Inventory<Tire>`). Wykorzystuje wewnętrznie strukturę `std::vector<T>`.

\* \*\*`class QTEManager`\*\* (Logika Mini-gry zręcznościowej)

&#x20;   \* \*Opis:\* Odpowiada za mechanikę paska mocy (Power Bar). Metoda `start()` uruchamia wskaźnik, a `stop()` przechwytuje moment wciśnięcia klawisza (np. spacji) i oblicza końcowy mnożnik siły (w przedziale od 0.0 do 1.0).



\## 4. Rdzeń Gry (Game Loop) i Zapis Stanu

\* \*\*`class Game`\*\* (Główna klasa zarządzająca)

&#x20;   \* \*Pola:\* `sf::RenderWindow window`, `GameState currentState` (enum: MENU, SHOP, PLAY, PAUSE), `int coins`

&#x20;   \* \*Metody:\* `run()`, `processEvents()`, `update()`, `render()` (Główna pętla gry implementująca maszyna stanów).

\* \*\*`class SaveManager`\*\* (Obsługa zapisu i odczytu)

&#x20;   \* \*Opis:\* Zawiera statyczne metody odpowiedzialne za utrwalanie stanu gry (balans monet, zakupiony ekwipunek) w lokalnym pliku tekstowym (`.txt`) lub formacie JSON.

