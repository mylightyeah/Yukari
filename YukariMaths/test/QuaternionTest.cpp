/** @file */

#ifndef DOXYGEN_SKIP

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "QuaternionTest"

#include <boost/test/unit_test.hpp>

#include <YukariMaths/Quaternion.h>

#define TOL 0.000011f

namespace Yukari
{
namespace Maths
{
  namespace Test
  {
    BOOST_AUTO_TEST_CASE(Quaternion_Init_Empty)
    {
      Quaternion q;
      /* TODO */
    }

    /* TODO */
  }
}
}

#endif
