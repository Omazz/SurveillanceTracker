# SurveillanceTracker
__SurveillanceTracker__ представляет собой программный комплекс, состоящий из следующих частей:
* __Radar__: ПО для имитации работы антенной системы РЛС. При работе выдаёт информацию на __Tracker__.
* __Tracker__: основное ПО, использующееся для завязки и сопровождения траекторий;
* __Indicator__: ПО для отображения информации, поступающей с выхода __Tracker__;
* __Settings__: ПО для настройки __Tracker__, а именно: выбор параметров захвата траектории, указание времени сканирования и т.д.;
* __Analyzer__: ПО для моделирования работы фильтров при различных входных данных, в том числе при различных траекториях движения;

Далее будет приведено более подробное описание каждого программного модуля.
## Radar
### Описание:
Приложение имитирует работу РЛС. Программа имеет интерфейс, позволяющий воспроизвести в реальном времени раннее записанную информацию. Формат выдаваемой информации - __Asterix__ категорий __34__ и __48__, используемых в авиационной индустрии для передачи информации о воздушных судах, а также о текущем состоянии РЛС.

## Tracker
### Описание:
Приложение, выполняющее траекторную обработку радиолокационных данных. Данное ПО формирует траекторию ВС, сопровождает его и предсказывает местоположение. Выход ПО должен подаваться на вход программы __Indicator__ для отображения диспетчеру. Фильтр, его параметры, количество экстраполяций до сброса траектории, время сканирования, минимальную и максимальную скорости объектов - всё это можно указать в __Settings__. При выборе фильтра и его параметров следует пользоваться программой __Analyzer__.

## Indicator
### Описание:
Приложение, предназначенное для отображения информации, поступающей с __Tracker__.  Индикатор отображает информацию о траекториях и состоянии антенной системы в реальном времени, что позволяет операторам контролировать и оптимизировать работу системы. Это позволяет скорректировать движение воздушных судов, а также может использоваться для устранения возможных проблем в работе системы, то есть для её отладки.

## Settings
### Описание:
Приложение, предназначенное для настройки работы __Tracker__. В нём можно выбрать следующую информацию: период сканирования антенной системы, число пропусков для сброса траектории, максимальную и минимальную скорости цели, правило завязки траектории, максимальный угол отклонения от курса ВС, использующийся фильтр, время ожидания информации о траектории до выдачи экстраполяции, пороговые значения по скорости и углу для определения манёвра и др.

## Analyzer
### Описание:
Приложение, которое позволяет __специалисту__ смоделировать интересующую траекторию и сравнить работу фильтров, применяемых к ней. Каждому фильтру можно задать входные параметры через интерфейс ПО. В качестве оценки выступает как визуальный результат работы ПО, то есть построенная отфильтрованная траектория, так и графики СКО, которые можно построить также через интерфейс. На данный момент реализованы следующие траектории: прямолинейное движение, движение с поворотом по дуге, движение по окружности. Предложение предназначено исключительно для __тестирования алгоритмов__ и __оптимизации дальнейшей разработки__.

# Общая архитектура проекта
В корневой папке проекта есть 7 директорий, рассмотрим каждую из них:
1) __.github__: содержит внутри директорию с файлом, где находится код для автотестирования проекта, то есть реализует технологию __CI/CD__.
2) __ApplicationIcons__: содержит внутри икнокни для приложений, входящих в проект.
3) __PriFiles__: содержит подключаемые модули, каждый из которых выполняет свою конкретную небольшую задачу, будь то шифрование, парсинг протокола, фильтрация или же что-то другое. Модули из этой директории используются в полноценных программных модулях, содержащихся в директории __Programs__.
4) __ProgramBuilds__: содержит внутри скомпилированные программные модули.
5) __Programs__: содержит внутри все программные модули, а также директорию __SurveillanceTrackerProgram__, в которой находится __pro__-файл, включающий в себя все эти же программные модули в виде подпроектов.
6) __Tests__: содержит внутри все тесты, а также директорию __SurveillanceTrackerTests__, в которой находится __pro__-файл, включающий в себя все эти же тесты в виде подпроектов.
7) __Records__: содержит записи, которые могут транслироваться в реальном времени программой __Radar__.

# Компиляция
Для сборки любого ПО из комплекса потребуется выполнить следующие шаги:
1) Установить __Qt__:
>sudo apt install -y qtcreator qtbase5-dev qt5-qmake cmake

2) Установить библиотеку __libpcap__:
> sudo apt-get install libpcap-dev

Для того, чтобы скомпилировать программные модули требуется сделать следующие действия:
1) Перейти в директорию __Programs/SurveillanceTrackerProgram/__.
2) Создать __make__-файл, воспользовавшись консольной командой:
> qmake SurveillanceTrackerProgram.pro
3) Скомпилировать программные модули, воспользовавшись простой командой:
> make

При этом, можно компилировать не все модули, а указывать конкретную цель:

> make sub----Radar

> make sub----Tracker

> make sub----Indicator

> make sub----Settings

> make sub----Analyzer

# Запуск
Для запуска любого ПО из комплекса потребуется выполнить следующие шаги:
1) Перейти в директорию __Programs/SurveillanceTrackerProgram/__.
2) Запустить интересующий модуль, воспользовавшись __make__-файлом __RunApps__.

Сделать это можно, воспользовавшись соответствующей командой:

> make Radar -f RunApps

> make Tracker -f RunApps

> make Indicator -f RunApps

> make Settings -f RunApps

> make Analyzer -f RunApps

Таким образом, для полноценного запуска комлпекса ПО требуется выполнить следующую последовательность действий:
1) Запустить __Radar__.
2) Запустить __Tracker__.
3) Запустить __Indicator__.
4) В __Radar__ уже изначально выстален порт, на котором функционирует __Tracker__. Требуется выбрать запись, нажав на соответствующую кнопку в интерфейсе ПО. После выбора записи она начнёт транслироваться в режиме реального времени в соответствии с теми временными интервалами, которые в ней определены.
5) Наблюдать выход работы __Tracker__ можно на программном модуле __Indicator__.

Примечание:
Для более точной настройки __Tracker__ следует воспользоваться ПО __Settings__. При этом, в ходе дальнейшей разработки может меняться алгоритм фильтрации траектории ВС. Инструментом анализа фильтрации является программный модуль __Analyzer__, который позволяет проанализировать работу фильтра при различных траекториях движения ВС, а также при различных шумах его измерений.

# Тестирование
Стоит отметить, что помимо ручного запуска тестов, при разработке проекта была применена технология __CI/CD__ в целях автоматизации тестирования. Результаты тестирования можно наблюдать в разделе репозитория __Actions__. Для того, чтобы скомпилировать тесты требуется сделать следующие действия:
1) Перейти в директорию __Tests/SurveillanceTrackerTests/__.
2) Создать __make__-файл, воспользовавшись консольной командой:
> qmake SurveillanceTrackerTests.pro
3) Скомпилировать все тесты можно, воспользовавшись простой командой:
> make

При этом, можно компилировать не все модули, а указывать конкретную цель:

> make sub----TestAnalyzer

> make sub----TestCipher

> make sub----TestTracker

Для запуска тестов необходимо выполнить следующие действия:
1) Перейти в директорию __Tests/SurveillanceTrackerTests/__.
2) Запустить интересующие тесты, воспользовавшись __make__-файлом __RunTests__.

Сделать это можно, воспользовавшись соответствующей командой:

> make TestAnalyzer -f RunTests

> make TestCipher -f RunTests

> make TestTracker -f RunTests
