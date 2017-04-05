/** @file */

#include "MSPParsers.h"

#include <YukariCommon/LoggingService.h>

using namespace Yukari::Common;
using namespace Yukari::Maths;

namespace Yukari
{
namespace MSP
{
  int16_t MSPParsers::Read16(MSPClient::Payload::const_iterator it)
  {
    union {
      int16_t v;
      uint8_t b[2];
    } s;

    s.b[0] = *(it++);
    s.b[1] = *it;

    return s.v;
  }

  bool MSPParsers::ParseRawIMUPayload(const MSPClient::Payload &payload, int16_t *gyro,
                                      int16_t *acc, int16_t *mag)
  {
    if (payload.size() != 18)
      return false;

    auto data = payload.begin();

    size_t i;

    for (i = 0; i < 3; i++)
    {
      acc[i] = Read16(data);
      data += 2;
    }

    for (i = 0; i < 3; i++)
    {
      gyro[i] = Read16(data);
      data += 2;
    }

    for (i = 0; i < 3; i++)
    {
      mag[i] = Read16(data);
      data += 2;
    }

    return true;
  }

  bool MSPParsers::ParseAttitudePayload(const MSPClient::Payload &payload, float *att)
  {
    if (payload.size() != 6)
      return false;

    auto data = payload.begin();

    /* X axis */
    att[0] = Read16(data) / 10.0f;
    data += 2;

    /* Y axis */
    att[1] = Read16(data) / 10.0f;
    data += 2;

    /* Heading */
    att[2] = Read16(data);

    return true;
  }

  bool MSPParsers::ParseQuaternion(const MSPClient::Payload &payload, Quaternion &quat)
  {
    static const size_t DATA_LENGTH = 8;

    if (payload.size() != DATA_LENGTH)
      return false;

    union {
      struct
      {
        int16_t w;
        int16_t i;
        int16_t j;
        int16_t k;
      } q;

      uint8_t data[DATA_LENGTH];
    } u;

    std::copy(payload.begin(), payload.end(), u.data);

    quat = Quaternion((float)u.q.w / 1000.0f, (float)u.q.i / 1000.0f, (float)u.q.j / 1000.0f,
                      (float)u.q.k / 1000.0f);

    return true;
  }
}
}
