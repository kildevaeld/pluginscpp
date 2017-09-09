#pragma once

namespace utils {

namespace algorithms {

//////////////////
// sort
/////////////////
template <typename Container> inline void sort(Container &c) {
  std::sort(c.begin(), c.end());
}

template <typename Container, typename Predicate>
inline void sort(Container &c, Predicate p) {
  std::sort(c.begin(), c.end(), p);
}

// pointer to member
template <typename Container, typename R, typename S>
inline void sort(Container &c, R S::*member) {
  auto f = std::mem_fn(member);
  using const_ref = typename Container::const_reference;
  std::sort(c.begin(), c.end(),
            [&f](const_ref a, const_ref b) { return f(a) < f(b); });
}

// pointer to member function
template <typename Container, typename R, typename S>
inline void sort(Container &c, R (S::*function)() const) {
  auto f = std::mem_fn(function);
  using const_ref = typename Container::const_reference;
  std::sort(c.begin(), c.end(),
            [&f](const_ref a, const_ref b) { return f(a) < f(b); });
}

//////////////////
// reverseForeach
/////////////////
template <typename Container, typename Op>
inline void reverseForeach(const Container &c, const Op &operation) {
  auto rend = c.rend();
  for (auto it = c.rbegin(); it != rend; ++it)
    operation(*it);
}

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