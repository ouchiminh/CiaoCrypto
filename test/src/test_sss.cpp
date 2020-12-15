#include "../ouchitest/include/ouchitest.hpp"
#include "algorithm/sss.hpp"

OUCHI_TEST_CASE(test_sss3)
{
    using gf256 = ouchi::math::gf256<>;
    ciao::sss<gf256> sss;
    sss.generate_key(3, std::random_device{});
    auto s1 = sss.generate_share(gf256{ 1 }, gf256{ 10 });
    auto s2 = sss.generate_share(gf256{ 2 }, gf256{ 10 });
    auto s3 = sss.generate_share(gf256{ 3 }, gf256{ 10 });
    auto d = sss.decode(ouchi::math::vl_matrix<gf256>{
        {
            gf256{ 1 }, s1,
            gf256{ 2 }, s2,
            gf256{ 3 }, s3
        },
        3, 2
    });
    OUCHI_CHECK_EQUAL((int)d.value, 10);
}

OUCHI_TEST_CASE(test_sss2)
{
    using gf256 = ouchi::math::gf256<>;
    ciao::sss<gf256> sss;
    sss.generate_key(2, std::random_device{});
    auto s1 = sss.generate_share(gf256{ 1 }, gf256{ 10 });
    auto s2 = sss.generate_share(gf256{ 2 }, gf256{ 10 });
    auto d = sss.decode(ouchi::math::vl_matrix<gf256>{
        {
            gf256{ 1 }, s1,
            gf256{ 2 }, s2,
        },
        2, 2
    });
    OUCHI_CHECK_EQUAL((int)d.value, 10);
}

OUCHI_TEST_CASE(test_sss16)
{
    using int_type = ouchi::math::gf2_16<>;
    ciao::sss<int_type> sss;
    sss.generate_key(2, std::random_device{});
    auto s1 = sss.generate_share(int_type{ 1 }, int_type{ 10 });
    auto s2 = sss.generate_share(int_type{ 2 }, int_type{ 10 });
    auto d = sss.decode(ouchi::math::vl_matrix<int_type>{
        {
            int_type{ 1 }, s1,
            int_type{ 2 }, s2,
        },
        2, 2
    });
    OUCHI_CHECK_EQUAL((int)d.value, 10);

}

