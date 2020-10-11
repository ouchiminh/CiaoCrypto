#include <type_traits>

namespace ciao {

template<class Int, class = void>
class dh_key_exchange;

template<class Int>
class dh_key_exchange<Int, std::enable_if_t<std::is_integral_v<Int>>>;

}

