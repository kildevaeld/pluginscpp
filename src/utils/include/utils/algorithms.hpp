#pragma once

namespace utils {

namespace algorithms {

//////////////////
// findOr
/////////////////
template <typename T, typename F>
typename T::value_type findOr(const T &container, typename T::value_type other,
                              F function) {
  typename T::const_iterator end = container.end();
  typename T::const_iterator begin = container.begin();

  typename T::const_iterator it = std::find_if(begin, end, function);
  if (it == end)
    return other;
  return *it;
}

template <typename T, typename R, typename S>
typename T::value_type findOr(const T &container, typename T::value_type other,
                              R (S::*function)() const) {
  return findOr(container, other, std::mem_fn(function));
}

template <typename T, typename F> int indexOf(const T &container, F function) {
  typename T::const_iterator end = container.end();
  typename T::const_iterator begin = container.begin();

  typename T::const_iterator it = std::find_if(begin, end, function);
  if (it == end)
    return -1;
  return it - begin;
}

template <typename T, typename F>
typename T::value_type findOrDefault(const T &container, F function) {
  return findOr(container, typename T::value_type(), function);
}

template <typename T, typename R, typename S>
typename T::value_type findOrDefault(const T &container,
                                     R (S::*function)() const) {
  return findOr(container, typename T::value_type(), function);
}
	
}


}