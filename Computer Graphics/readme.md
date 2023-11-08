# User manual

## Kompilacja
Używana jest biblioteka ImGui, glm, glfw, oraz glew.
biblioteka ImGui oraz glm rozpakowana jest do folderu src/vendor,
natomiast GLFW oraz GLEW dodane są do dependencji projektu.

## Aktywny wielokąt
W trakcie korzystania z programu aktywny wielokąt jest wybierany na podstawie aktywnej karty wielokątów w menu (dodawanie i usuwanie wielokątów też jest zrobione na ich podstawie)

## Tryby wielokątów
Każdy wielokąt ma 2 możliwe tryby, tryb edytowania odpowiada za wszystkie możliwe operacje, natomiast tryb dodawania pozwala jedynie na dodawanie wierzchołków.

Tryb wielokąta można zmienić w menu głównym.

### Tryb dodawania
W trybie dodawania jedyną możliwą operacją na płótnie jest operacja dodawania wierzchołków za pomocą kliknięcia lewego przycisku myszy.

### Tryb edycji
W trybie edycji możemy:
- Dodawać nowy wierzchołek (od aktywnego) podwójnym kliknięciem lewego przycisku myszy
- Aktywować wierzchołek klikając na niego lewym przyciskiem myszy
- Przesuwać wierzchołki i linie przytrzymując lewy przycisk myszy
- Przesuwać aktywny wielokąt przytrzymując lewy przycisk myszy oraz klawisz shift
- Dodawać relację do krawędzi przy pomocy menu kontekstowego kilkając prawy przycisk myszy na wybranej krawędzi