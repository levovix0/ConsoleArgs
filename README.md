# ConsoleArgs

маленькая библиотека в один заголовочный файл, упрощающая работу с аргументами командной строки

## Использование

```c++
int main(int argc, char** argv) {
  // инициализация
  ConsoleArgs args{argc, argv};
```

```c++
if (args.has("-h") || args.has("--help")) {
  // вывести помощь
}
```

```c++
// работает как с  `-mode X`  так и с  `-mode=X`
auto _mode = args.getValue("-mode");
auto mode = _mode == nullopt? "standard" : *_mode;
// или короче:
auto mode = args.getValue("-mode", "standard");
```

```c++
auto color = args.get(std::regex("^[0-9a-fA-F]{6}$"));
```

```c++
// получить аргумент, который не был просмотрен
auto just_arg = args.next();
```
