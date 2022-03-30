/**
 * Common interface for factory methods that translate to and from protocol buffers. There are two overloads for each
 * method. The overload that takes an output pointer does not "zero-out" that parameter. This allows these factory
 * methods to be used in more situations, such as accumulating results, with the caveat that the caller is responsible
 * for the initial state of the output parameter. Generally, this is not an issue as most callers will instantiate a new
 * instance of the output immediately before calling the factory method.
 *
 * Protocol buffer arenas are supported via the output pointer overloads. That is, it is possible to instantiate a
 * protocol buffer via an arena and then have it filled in with these methods.
 */
#pragma once

namespace pack::component {

template <typename T, typename MessageT>
void to_proto(const T& t, MessageT* msg);

template <typename T, typename MessageT>
MessageT to_proto(const T& t) {
  MessageT result{};
  to_proto(t, &result);
  return result;
}

template <typename T, typename MessageT>
void from_proto(const MessageT& msg, T* t);

template <typename T, typename MessageT>
T from_proto(const MessageT& msg) {
  T result{};
  from_proto(msg, &result);
  return result;
}

}  // namespace pack::component
