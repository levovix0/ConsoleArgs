#pragma once

#include <vector>
#include <string>
#include <regex>
#include <optional>

namespace calib
{
  struct ConsoleArgs
  {
    std::vector<std::string> args;
    bool blockArgs = true;

    ConsoleArgs() = default;
    ConsoleArgs(int argc, char** argv);
    ConsoleArgs(ConsoleArgs const& copy) = default;
    ConsoleArgs(ConsoleArgs&& move) = default;

    ConsoleArgs& operator=(ConsoleArgs const& copy) = default;
    ConsoleArgs& operator=(ConsoleArgs&& move) = default;

    [[nodiscard]] size_t count() const;
    [[nodiscard]] std::string_view operator[](size_t n) const;

    [[nodiscard]] bool has(std::string_view arg);
    [[nodiscard]] bool has(std::regex regex_s);

    [[nodiscard]] std::optional<std::string> get(std::string_view arg);
    [[nodiscard]] std::optional<std::string> get(std::regex regex_s);

    // берёт значение у параметра: -i ~/text.txt  ->  ~/text.txt,  std=c++17  ->  c++17
    [[nodiscard]] std::optional<std::string> getValue(std::string_view arg);

    // Возвращает следующий аргумент, который не является настройкой
    [[nodiscard]] std::optional<std::string> next();

  private:

    std::vector<bool> blockedArgs;

    void blockArg(size_t n);
  };



  inline ConsoleArgs::ConsoleArgs(int argc, char** argv) {
    for (int i = 1; i < argc; ++i)
    args.emplace_back(argv[i]);
   }

  inline size_t ConsoleArgs::count() const { return args.size(); }
  inline std::string_view ConsoleArgs::operator[](size_t n) const { return args[n]; }

  inline bool ConsoleArgs::has(std::string_view arg) {
    auto a = std::find(args.begin(), args.end(), arg);
    if (a != args.end()) { blockArg(a - args.begin()); return true; }
    return false;
   }
  inline bool ConsoleArgs::has(std::regex regex_s) {
    auto a = std::find_if(args.begin(), args.end(), [&regex_s](std::string& s) {
      return std::regex_search(s, regex_s);
     });
    if (a != args.end()) { blockArg(a - args.begin()); return true; }
    return false;
   }

  inline std::optional<std::string> ConsoleArgs::get(std::string_view arg) {
    auto it = std::find(args.begin(), args.end(), arg);
    if (it == args.end()) return std::nullopt;
    blockArg(it - args.begin());
    return *it;
   }
  inline std::optional<std::string> ConsoleArgs::get(std::regex regex_s) {
    auto it = std::find_if(args.begin(), args.end(), [&regex_s](std::string& s) {
      return std::regex_search(s, regex_s);
     });
    if (it == args.end()) return std::nullopt;
    blockArg(it - args.begin());
    return *it;
   }

  inline std::optional<std::string> ConsoleArgs::getValue(std::string_view arg) {
    auto it = std::find_if(args.begin(), args.end(), [arg](std::string& s) {
      return (s.size() == arg.size() && s == arg) || (s.size() > arg.size() && s.substr(0, arg.size()) == arg && s[arg.size()] == '=');
     });
    if (it == args.end()) return std::nullopt;
    auto itpos = it - args.begin();
    auto&& sv = *it;
    if (sv.size() > arg.size()) {
      blockArg(itpos);
      return sv.substr(arg.size() + 1, sv.size() - arg.size() - 1);
     }
    if (++it == args.end()) return std::nullopt;
    blockArg(itpos);
    blockArg(itpos + 1);
    return *it;
   }

  inline std::optional<std::string> ConsoleArgs::next() {
    auto it = std::find(blockedArgs.begin(), blockedArgs.end(), false);
    if ((it - blockedArgs.begin()) >= args.size()) return std::nullopt;
    return args[it - blockedArgs.begin()];
   }

  inline void ConsoleArgs::blockArg(size_t n) {
    if (!blockArgs) return;
    for (size_t i = blockedArgs.size(); i <= n; ++i)
    blockedArgs.emplace_back(false);
    blockedArgs[n] = true;
   }
}
