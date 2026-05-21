# Architektura Projektu i Struktura Klas

Dokumentacja techniczna przedstawiająca strukturę klas, mechanizmy Programowania Obiektowego (OOP) oraz wykorzystanie szablonów w projekcie **"Indian Space Program: Tire Launch"**.

---

## 1. Hierarchia Klas Ekwipunku (Dziedziczenie i Polimorfizm)

Klasa bazowa pozwala na elastyczne zarządzanie asortymentem w sklepie i łatwe dodawanie nowych przedmiotów.

### `class Equipment` (Abstrakcyjna klasa bazowa)
- **Pola wspólne:**
  - `std::string name` — nazwa przedmiotu
  - `int price` — cena zakupu
  - `bool isUnlocked` — status odblokowania
- **Metody kluczowe:**
  - `virtual float getPowerMultiplier() const = 0;` — czysta funkcja wirtualna zapewniająca polimorfizm.

### `class Motor : public Equipment`
- **Opis:** Klasa pochodna reprezentująca silniki.
- **Logika:** Nadpisuje funkcję wirtualną, zwracając bonus do prędkości początkowej opony.

### `class Tire : public Equipment`
- **Opis:** Klasa pochodna reprezentująca opony.
- **Logika:** Nadpisuje funkcję wirtualną, zwracając bonus do aerodynamiki (zmniejszenie oporu powietrza).

---

## 2. Obiekty Gry i Cele (Elementy Wizualne SFML)

### `class GameObject` (Klasa bazowa obiektów graficznych)
- **Pola:** `sf::Sprite sprite`, `sf::Vector2f position`
- **Metody:** `virtual void update(float deltaTime)`, `virtual void draw(sf::RenderWindow& window)`

### `class Target : public GameObject` (Klasa bazowa dla celów)
- **Pola:** `int coinReward` — nagroda в monetach za zestrzelenie obiektu.

### `class Bird` oraz `class Drone` (Klasy pochodne)
- **Opis:** Definiują unikalne algorytmy ruchu (np. lot ptaka po sinusoidzie lub jednostajny ruch drona).

---

## 3. Szablony i Menedżery Logiki

### `template <typename T> class Inventory` (Klasa szablonowa)
- **Opis:** Uniwersalny kontener szablonowy przechowujący odblokowane przedmioty gracza przy użyciu `std::vector<T>`. Pozwala na tworzenie osobnych list, np. `Inventory<Motor>` i `Inventory<Tire>`.

### `class QTEManager` (Logika paska mocy)
- **Opis:** Obsługuje mini-grę zręcznościową. Odpowiada za ruch wskaźnika na pasku mocy i oblicza końcowy mnożnik siły wystrzału (od 0.0 do 1.0).

---

## 4. Rdzeń Gry (Game Loop) i Zapis Stanu

### `class Game` (Główna klasa zarządzająca)
- **Pola:** `sf::RenderWindow window`, `GameState currentState` (MENU, SHOP, PLAY, PAUSE), `int coins`
- **Metody:** `run()`, `processEvents()`, `update()`, `render()`

### `class SaveManager` (Obsługa plików)
- **Opis:** Zawiera statyczne metody do zapisu i odczytu stanu gry (stan konta, zakupione przedmioty) z lokalnego pliku tekstowego lub JSON.
