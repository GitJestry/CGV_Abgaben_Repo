# CGV Abgaben-Repository

Dieses Repository dient als einfache und einheitliche Grundlage für die Abgaben im Modul **Computergrafik und Visualisierung**.  
Es soll helfen, Projekte strukturiert zu organisieren, Build-Prozesse konsistent zu halten und typische Konfigurationsprobleme früh zu vermeiden.

---

## Wichtige Hinweise zum Kompilieren

Da die Projekte auf **macOS**, **Linux** und **Windows** lauffähig sein sollen, sollte bei OpenGL-Aufgaben bevorzugt die **klassische bindbasierte State-Logik** verwendet werden und nicht die neuere **Direct State Access (DSA)**-API.

Der Grund dafür ist die Plattformkompatibilität:

- **macOS** unterstützt OpenGL nur eingeschränkt und in der Regel nicht die vollständige moderne DSA-Funktionalität.
- Klassische OpenGL-Aufrufe wie `glBindBuffer`, `glBufferData`, `glBindVertexArray` und `glVertexAttribPointer` sind deutlich portabler.
- Dadurch lassen sich plattformabhängige Laufzeitfehler vermeiden, etwa durch nicht verfügbare OpenGL-Funktionen.

### VS Code / IntelliSense

Für jeden Programmordner sollte der passende **`.vscode`-Settings-Ordner** mitgeführt werden.  
Dieser hilft der IDE dabei,

- Include-Pfade korrekt aufzulösen,
- Framework-Headers zu finden,
- falsche IntelliSense-Fehlermeldungen zu reduzieren.

Zusätzlich sollte in jeder `CMakeLists.txt` folgende Zeile enthalten sein:

```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
````

Diese Option erzeugt eine `compile_commands.json`, die von VS Code und anderen Tools verwendet werden kann, um Include-Pfade, Compiler-Flags und Projektstruktur korrekt zu erkennen.

---

## Build-Konfiguration

Es wird empfohlen, den Build-Ordner einheitlich im **Root-Verzeichnis** des Projekts anzulegen und ihn schlicht **`build`** zu nennen.

### CMake konfigurieren und bauen

```bash
cmake -S . -B build && cmake --build build
```

---

## Code-Stil

Um den Code im gesamten Repository konsistent zu halten, gelten folgende Namens- und Formatierungsregeln:

| Element         | Konvention               | Beispiel          |
| --------------- | ------------------------ | ----------------- |
| Einrückung      | 2 Leerzeichen            | `if (...) {`      |
| Variablen       | `var_name`               | `triangle_count`  |
| Membervariablen | `m_var_name`             | `m_shaderProgram` |
| Typen / Klassen | `TypeName`               | `MainApp`         |
| Funktionen      | `getVarName()`           | `loadShader()`    |
| Parameter       | `parName`                | `filePath`        |
| Konstanten      | `CONSTANT_VARIABLE_NAME` | `VERTEX_COUNT`    |

### Zusätzliche Stilhinweise

* Funktionsnamen sollten klar und sprechend sein.
* Klassen sollten nach Möglichkeit genau eine klare Verantwortung haben.
* OpenGL-Setup-Code sollte sauber vom Render-Code getrennt werden.
* Magische Zahlen sollten wenn möglich durch Konstanten ersetzt werden.
* Plattformkritische oder frameworkabhängige Stellen sollten kommentiert werden.
* Coded Performance Optimal, aber auch leicht lesbar. Clean smelling code ist oft sehr langsam.
* Der Code selbst soll kommunizieren. Kommentare nur da einsetzen wo wirklich nötig.
---

## Hinweise zu OpenGL-Code

Für maximale Kompatibilität sollte bevorzugt die klassische OpenGL-Initialisierung verwendet werden, zum Beispiel:

* `glBindBuffer(...)`
* `glBufferData(...)`
* `glBindVertexArray(...)`
* `glEnableVertexAttribArray(...)`
* `glVertexAttribPointer(...)`

Moderne DSA-Funktionen wie

* `glNamedBufferData(...)`
* `glVertexArrayVertexBuffer(...)`
* `glProgramUniform1f(...)`

können nicht genutzt werden da sie zu Segmentation Error führen.
