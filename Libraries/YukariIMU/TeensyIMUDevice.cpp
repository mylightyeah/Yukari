/** @file */

#include "TeensyIMUDevice.h"

#include <YukariMSP/MSPParsers.h>

using namespace Yukari::Common;
using namespace Yukari::Maths;
using namespace Yukari::MSP;

namespace Yukari
{
namespace IMU
{
  TeensyIMUDevice::TeensyIMUDevice(const std::string &port, unsigned int baud)
      : IMSPGrabber(port, baud)
      , m_logger(LoggingService::Instance().getLogger("TeensyIMUDevice"))
  {
    m_mspPayloadQuat.reserve(6);
  }

  TeensyIMUDevice::~TeensyIMUDevice()
  {
  }

  void TeensyIMUDevice::setDispacement(const Eigen::Vector3f &displacement)
  {
    // TODO
  }

  IMUFrame::Ptr TeensyIMUDevice::grabFrame()
  {
    m_mspPayloadQuat.clear();

    /* MSP data */
    bool ok = m_client.requestData(MSPClient::Y_ORIENTATION, m_mspPayloadQuat);

    if (!ok)
    {
      m_logger->error("Failed to get all MSP data");
      return nullptr;
    }

    /* Calculate timestep */
    auto timeNow = std::chrono::high_resolution_clock::now();
    auto frameDuration = timeNow - m_lastFrameTime;
    m_lastFrameTime = timeNow;

    auto retVal = std::make_shared<IMUFrame>(frameDuration);

    /* Set orientation */
    Eigen::Quaternionf q;
    MSPParsers::ParseQuaternion(m_mspPayloadQuat, q);
    retVal->orientation() = m_transform.orientation() * q;

    return retVal;
  }
}
}
