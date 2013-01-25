Sat Mar 30 10:08:39 1996
Projekt "Advanced Logic" - filtr do przetwarzania strumieni tekstu.


Init - zwraca liczbę zużytych argumentów lub -1 w przypadku błędu.
Init dostaje jako parametry wszystkie parametry od wystąpienia słowa
kluczowego, które wybrało dany obiekt, do końca linii. Podana jest także
liczba tych słów kluczowych.

Sposób działania procesu - funkcji Work():
1. Czytaj dane z wejściowego buforu.
2. Jeśli pozytywne, to zapisz przetworzone dane na wyjście - to się
zawsze powodzi.
3. Jeśli jest 0 bajtów, to wychodzimy z funkcji.
4. Jeśli jest EOF, to znaczy, że już nie ma danych - trzeba
wywołać SignalEndOfData() - zawiadom następne bufory, że już nic nie dostaną.

Ograniczenie wielkości bufora:
Amiga przy odczycie fread() ze standardowego wejścia przy ilości ponad
512 elementów o rozmiarze 1 bajtu ma kłopoty - sygnalizuje koniec
strumienia danych zaraz po wprowadzeniu pierwszego Entera.
