# GuessNumberX — Cross-Platform «Угадай число»

Кроссплатформенное Qt 6 / QML приложение «Угадай число» на базе `lab7/lab7_task4_yarmosh`,
расширенное под четыре платформы: **Android, iOS, Linux, Web (WebAssembly)**.

## Платформенные UI

| Платформа | Стиль | Навигация | Особенности |
|-----------|-------|-----------|-------------|
| **Android** | Material 3 | `Drawer` слева + `TabBar` (Bottom Navigation) с 3 кнопками | Большие сенсорные зоны (56dp), snackbar для ошибок, скругления |
| **iOS** | Apple Music | `TabBar` снизу (TextUnderIcon, акцент `#FF2D55`) | Плавные кросс-фейды между вкладками, плоские кнопки без теней, inline-баннеры вместо модальных диалогов |
| **Linux** | Fusion / classic | `MenuBar` сверху + `ToolBar` + `TabBar` | Drag-and-drop файла → seed для игры, hot-keys (Ctrl+N/Q/1/2/3, F1), статус-бар внизу |
| **Web (wasm)** | Адаптив | На ширине <720px — Android-скин, иначе — Linux-скин | Управление с клавиатуры (Enter — submit, Ctrl+R — новая игра, F1 — подсказка) |

Платформа определяется в `src/platforminfo.cpp` через `Q_OS_*` макросы и поднимается в QML через `platformInfo` context-property; `qml/Main.qml` — это `Loader`, переключающий конкретный UI.

## Функциональность

- Загадывается число от 0 до 99, до 10 (настраиваемо) попыток.
- Подсказки «больше / меньше» с анимациями: bounce, rotate, fadeIn (см. `qml/shared/GameAnimations.qml`).
- Локальная статистика: число запусков, игр, всего попыток / очков, лучший результат, UUID приложения.
- История игр (JSON в `QSettings`), отображается в виде `ListView` на всех платформах.
- Drag-and-drop файла на Linux: цифры из имени файла используются как seed для новой игры.
- Live-переключение языка: **EN / RU / BE** через `Localizer` + `QQmlEngine::retranslate()`.
- Анимации: bounce при подсказке, rotate при победе, fadeIn при новой игре, shake при ошибке ввода, smooth-fade при смене скина на Web, snackbar fade-in на Android, slide-in настроек на Linux.
- Все исключения логируются в консоль через `qWarning()` / `qCritical()`, в UI выводится локализованное сообщение.

## Структура

```
lab9-task3/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── gamemodel.{h,cpp}        # core (seed-able RNG)
│   ├── gameviewmodel.{h,cpp}    # QML-facing VM
│   ├── settingsmanager.{h,cpp}  # QSettings + история
│   ├── platforminfo.{h,cpp}     # детектор платформы
│   ├── localizer.{h,cpp}        # runtime language switching
│   └── drophandler.{h,cpp}      # обработка drag-and-drop
├── qml/
│   ├── Main.qml                 # роутер платформ
│   ├── shared/                  # GameCore, StatsCard, GameAnimations, …
│   ├── android/                 # AndroidUI + Material страницы
│   ├── ios/                     # IosUI + Apple Music страницы
│   ├── linux/                   # LinuxUI с MenuBar/Toolbar/DropArea
│   └── web/                     # WebUI обёртка (адаптивный режим)
├── i18n/
│   ├── lab9_en.ts
│   ├── lab9_ru.ts
│   └── lab9_be.ts
├── icons/                       # SVG иконки
├── tests/
│   ├── unit/tst_unit.cpp                # модульные тесты (8)
│   ├── integration/tst_integration.cpp  # интеграционные (7)
│   └── widget/                          # QQuickTest QML-тесты
│       ├── tst_widget.cpp
│       └── qml/tst_gamecore.qml         # 5 + 4 ≈ 9 UI-тестов
└── .github/workflows/build.yml          # CI на все 4 платформы
```

## Сборка локально (Linux)

```bash
sudo apt install qt6-base-dev qt6-declarative-dev qt6-tools-dev \
                 qt6-quickcontrols2-dev qt6-svg-dev qt6-multimedia-dev \
                 cmake ninja-build

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/appGuessNumberX
```

### Запуск тестов

```bash
cd build
ctest --output-on-failure --verbose
# отдельно:
./tst_unit
./tst_integration
./tst_widget -platform offscreen
```

## Тесты (по требованию: 3-7 каждого вида)

| Вид                  | Файл                                | Тестов |
|----------------------|-------------------------------------|:------:|
| Модульные (Unit)     | `tests/unit/tst_unit.cpp`           | 8      |
| Интеграционные       | `tests/integration/tst_integration.cpp` | 7  |
| UI / Widget (QML)    | `tests/widget/qml/tst_*.qml` + setup на C++ | 5 + 4 = 9 |

Все три набора регистрируются через `enable_testing()` в `CMakeLists.txt` и автоматически выполняются в CI на Linux-джобе (см. `.github/workflows/build.yml`).

## CI / GitHub Actions

Workflow `.github/workflows/build.yml` собирает все четыре таргета:

| Job        | Что делает |
|------------|-----------|
| `linux`    | Устанавливает Qt 6.7.3 desktop, конфигурирует CMake, собирает, **запускает `ctest`** (unit + widget + integration), публикует artifact `guessnumberx-linux`. |
| `android`  | Ставит host-Qt + Android-Qt + NDK 25, кросс-собирает APK для `arm64-v8a`, публикует `guessnumberx-android`. |
| `web`      | Ставит host-Qt + emsdk 3.1.50 + Qt for WebAssembly, собирает `.wasm`/`.js`/`.html`, публикует bundle. |
| `deploy-pages` | Деплоит web-сборку на GitHub Pages (зависит от `web`). |
| `ios`      | На `macos-14`: host-Qt + iOS-Qt, генерирует Xcode-проект, собирает `.app` под iPhone Simulator (arm64). |

Чтобы Pages заработал — в настройках репозитория: `Settings → Pages → Source: GitHub Actions`.

## Подключение как submodule

Этот репозиторий публичен и подключается в основной репозиторий лабораторной работы (для документации):

```bash
git submodule add https://github.com/<user>/<this-repo>.git lab9-task3
git submodule update --init --recursive
```

## Соответствие требованиям

| Требование | Где реализовано |
|------------|-----------------|
| Кроссплатформенность Android/iOS/Linux/Web | `src/platforminfo.*`, `qml/Main.qml`, поддиректории `qml/<platform>/` |
| Material + Drawer + BottomNav (Android) | `qml/android/AndroidUI.qml` |
| TabBar + Apple Music стиль (iOS) | `qml/ios/IosUI.qml`, `IosGamePage.qml` |
| MenuBar + ToolBar + Drag&Drop (Linux) | `qml/linux/LinuxUI.qml` + `src/drophandler.cpp` |
| Web адаптив + клавиатура | `qml/web/WebUI.qml`, `Keys.onPressed` в `qml/shared/GameCore.qml` |
| Локализация на 3 языка | `i18n/lab9_{en,ru,be}.ts` + `src/localizer.cpp` (runtime switch) |
| Анимация элементов UI | `qml/shared/GameAnimations.qml` (bounce/rotate/fade), shake/slide/snackbar |
| Обработка исключений + лог в консоль | try/catch в `GameModel`, `GameViewModel`, `SettingsManager`, `DropHandler` + `qWarning/qCritical` |
| Тесты unit/widget/integration | `tests/`, выполняются в `ctest` в CI |
| GitHub Actions сборка всех платформ | `.github/workflows/build.yml` |
