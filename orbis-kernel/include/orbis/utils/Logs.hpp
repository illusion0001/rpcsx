#pragma once
#include <atomic>
#include <span>
#include <string>

namespace orbis {
inline namespace logs {
enum class LogLevel : unsigned char {
  Always,
  Fatal,
  Error,
  Todo,
  Success,
  Warning,
  Notice,
  Trace
};

// Currently enabled log level
inline std::atomic<LogLevel> logs_level = LogLevel::Notice;

template <typename T, typename = void> struct log_class_string {
  static const T &get_object(const void *arg) {
    return *static_cast<const T *>(arg);
  }
  static void format(std::string &out, const void *arg);
};

template <> struct log_class_string<void *, void> {
  static void format(std::string &out, const void *arg);
};

template <typename T>
struct log_class_string<T *, void> : log_class_string<void *, void> {};

template <> struct log_class_string<char *, void> {
  static void format(std::string &out, const void *arg);
};

template <std::size_t N> struct log_class_string<char[N]> {
  static void format(std::string &out, const void *arg) {
    out += reinterpret_cast<const char *>(arg);
  }
};

template <>
struct log_class_string<char8_t *, void> : log_class_string<char *> {};

template <typename... Args>
using log_args_t = const void *(&&)[sizeof...(Args) + 1];

struct log_type_info {
  decltype(&log_class_string<int>::format) log_string;

  template <typename T> static constexpr log_type_info make() {
    return log_type_info{
        &log_class_string<T>::format,
    };
  }
};

namespace detail {
template <typename T> struct remove_const_data {
  using type = T;
};
template <typename T> struct remove_const_data<const T *> {
  using type = T *;
};

template <typename T>
using remove_const_data_t = typename remove_const_data<T>::type;
} // namespace detail

template <typename... Args>
constexpr const log_type_info type_info_v[sizeof...(Args) + 1]{
    log_type_info::make<
        detail::remove_const_data_t<std::remove_cvref_t<Args>>>()...};

void _orbis_log_print(LogLevel lvl, const char *msg, std::string_view names,
                      const log_type_info *sup, ...);

template <typename... Args>
void _orbis_log_impl(LogLevel lvl, const char *msg, const char *names,
                     const Args &...args) {
  // Fast filtering
  if (logs_level.load(std::memory_order::relaxed) < lvl)
    return;

  _orbis_log_print(lvl, msg, names, type_info_v<Args...>,
                   static_cast<const void *>(&args)...);
}

} // namespace logs
} // namespace orbis

#define ORBIS_LOG_FATAL(msg, ...)                                              \
  ::orbis::_orbis_log_impl(::orbis::LogLevel::Fatal, (msg), #__VA_ARGS__,      \
                           ##__VA_ARGS__)
#define ORBIS_LOG_ERROR(msg, ...)                                              \
  ::orbis::_orbis_log_impl(::orbis::LogLevel::Error, (msg), #__VA_ARGS__,      \
                           ##__VA_ARGS__)
#define ORBIS_LOG_TODO(msg, ...)                                               \
  ::orbis::_orbis_log_impl(::orbis::LogLevel::Todo, (msg), #__VA_ARGS__,       \
                           ##__VA_ARGS__)
#define ORBIS_LOG_SUCCESS(msg, ...)                                            \
  ::orbis::_orbis_log_impl(::orbis::LogLevel::Success, (msg), #__VA_ARGS__,    \
                           ##__VA_ARGS__)
#define ORBIS_LOG_WARNING(msg, ...)                                            \
  ::orbis::_orbis_log_impl(::orbis::LogLevel::Warning, (msg), #__VA_ARGS__,    \
                           ##__VA_ARGS__)
#define ORBIS_LOG_NOTICE(msg, ...)                                             \
  ::orbis::_orbis_log_impl(::orbis::LogLevel::Notice, (msg), #__VA_ARGS__,     \
                           ##__VA_ARGS__)
#define ORBIS_LOG_TRACE(msg, ...)                                              \
  ::orbis::_orbis_log_impl(::orbis::LogLevel::Trace, (msg), #__VA_ARGS__,      \
                           ##__VA_ARGS__)
