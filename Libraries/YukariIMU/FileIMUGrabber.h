/** @file */

#pragma once

#include "IIMUGrabber.h"

#include <chrono>
#include <memory>

namespace Yukari
{
namespace IMU
{
  class FIleIMUGrabber : public IIMUGrabber
  {
  public:
    FIleIMUGrabber();

    virtual void open() override;
    virtual void close() override;
    virtual bool isOpen() const override;

    virtual IMUFrame::Ptr grabFrame() override;

  protected:
    bool m_open;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastFrameTime;
  };
}
}
