# Zdalny serwer MySQL <!-- omit in toc -->

- [1. Instalacja](#1-instalacja)
- [2. Uruchomienie serwera](#2-uruchomienie-serwera)
- [3. Uruchomienie klienta](#3-uruchomienie-klienta)
- [4. Uruchomienie testów](#4-uruchomienie-testów)
- [5. Funkcjonalności](#5-funkcjonalności)
- [6. Warunki](#6-warunki)
- [7. Inne](#7-inne)

## 1. Instalacja

* Instalacja Boost
* Instalacja SOCI (Instrukcja na https://github.com/SOCI/soci)

`mkdir build`<br>
`cd buid`<br>
`cmake .. && make -j`

## 2. Uruchomienie serwera

`./server <PORT>`

**Uwaga!** Do uruchomienia serwera wymagane jest:
* Podanie loginu i hasła do lokalnego serwera MySQL
* Podanie nazwy ***ISTNIEJĄCEJ*** bazy na serwerze MySQL
* Podanie nazwy tabeli, w której będą przechowywane rekordy
  * Jeśli tabela już istnieje, musi ona posiadać kolumny `id` oraz `value`
  * Jeśli tabela nie istnieje, zostanie stworzona

## 3. Uruchomienie klienta

`./client <IP_SERWERA> <PORT>`

## 4. Uruchomienie testów

`./serverSQL-ut`

Lub:

`./ctest -V`

## 5. Funkcjonalności

Serwer...

- [x] ... obsługuje dowolną ilość równoległych połączeń (ograniczonych zasobami systemu)
- [x] ... odpowiada możliwie najszybciej na wysyłane zapytania (są one obsługiwane równolegle)
- [ ] ... jest przygotowany na problemy z transmisją
- [x] ... utrzymuje przychodzące połączenia
- [x] ... kończy połączenie z klientem na jego życzenie 
- [ ] ... kończy połączenie przy braku aktywności klienta przez 30s
- [x] ... zapisuje przesłane informacje w bazie danych A
- [x] ... odczytuje informacje z bazy danych A
- [x] ... zapisuej logi (historię komend) w oddzielnej bazie (nazwa oryginalnej tabeli z dopiskiem `_log`)

## 6. Warunki

- [x] Serwer musi być przygotowany na równoległy dostęp (odczyt i zapis) do danych w kilku połączeniach
- [x] Implementacja C++ z użyciem biblioteki Boost (http://www.boost.org/) - Boost.Asio
- [x] Baza danych: mysql
- [x] JSON z użyciem jsoncpp (https://github.com/open-source-parsers/jsoncpp)

## 7. Inne

* Do komunikacji z serwerem MySQL użyto biblioteki SOCI (https://github.com/SOCI/soci) autorstwa Macieja Sobczaka 
* Do zbudowania projektu wykorzystano CMake (https://github.com/Kitware/CMake)
* Testy napisano w GTest (https://github.com/google/googletest)
