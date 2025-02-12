//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_SCOPABLE_HPP
#define BOOST_DI_CONCEPTS_SCOPABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace concepts {

template <class...>
struct scope {
  struct is_referable {};
  struct try_create {};
  struct create {};

  template <class...>
  struct requires_ : aux::false_type {};
};

template <class>
struct scope__ {
  template <class...>
  struct scope {
    template <class...>
    using is_referable = aux::true_type;

    template <class T, class, class TProvider>
    T try_create(const TProvider&);

    template <class T, class, class TProvider>
    T create(const TProvider&);
  };
};

template <class>
struct config__ {
  template <class T>
  struct scope_traits {
    using type = scope__<T>;
  };

  template <class T>
  struct memory_traits {
    using type = type_traits::heap;
  };
};

template <class T>
struct provider__ {
  using config = config__<T>;

  template <class TMemory = type_traits::heap>
  aux::conditional_t<aux::is_same<TMemory, type_traits::stack>::value, T, T*> try_get(const TMemory& = {}) const;

  template <class TMemory = type_traits::heap>
  T* get(const TMemory& = {}) const {
    return nullptr;
  }

  config& cfg() const;
};

template <class T>
typename scope<T>::template requires_<typename scope<_, _>::is_referable, typename scope<_, _>::try_create,
                                      typename scope<_, _>::create>
scopable_impl(...);

template <class T>
auto scopable_impl(T&&)
    -> aux::is_valid_expr<typename T::template scope<_, _>::template is_referable<_, config__<_>>,
                          decltype(T::template scope<_, _>::template try_create<_, _>(provider__<_>{})),
                          decltype(aux::declval<typename T::template scope<_, _>>().template create<_, _>(provider__<_>{}))>;

template <class T>
struct scopable__ {
  using type = decltype(scopable_impl<T>(aux::declval<T>()));
};

template <class T>
using scopable = typename scopable__<T>::type;

}  // namespace concepts

#endif
