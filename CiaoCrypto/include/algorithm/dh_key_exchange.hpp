#include <type_traits>

namespace ciao {

template<class T>
concept integer = std::is_integral_v<T>;

template<integer Int>
class dh_key_exchange;

}

