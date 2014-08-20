//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_SCOPE_HPP
#define BOOST_DI_BINDINGS_SCOPE_HPP

#include "boost/di/bindings/dependency.hpp"

namespace boost {
namespace di {
namespace bindings {

BOOST_DI_HAS_MEMBER_TYPE(given);

template<typename TScope>
class scope {
    template<typename T>
    using is_dependency = has_given<T>;

    template<typename T, typename = void>
    struct get_binding
        : dependency<TScope, T, T>
    { };

    template<typename T>
    struct get_binding<T, typename std::enable_if<is_dependency<T>::value>::type>
        : T::template rebind<TScope>::other
    { };

    template<typename... Ts>
    struct bind_impl
        : type_list<typename get_binding<Ts>::type...>
    { };

public:
    template<typename... Ts>
    using bind = typename bind_impl<Ts...>::type;
};

} // namespace bindings
} // namespace di
} // namespace boost

#endif

