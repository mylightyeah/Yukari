/** @file */

#pragma once

#include <memory>
#include <vector>

#include <YukariCloudCapture/ICloudGrabber.h>
#include <YukariCommon/LoggingService.h>
#include <YukariIMU/IIMUGrabber.h>
#include <YukariProcessing/IFrameProcessingTask.h>
#include <YukariTriggers/ITrigger.h>

#include "Types.h"

namespace Yukari
{
namespace CaptureApp
{
  class CaptureController
  {
  public:
    typedef std::shared_ptr<CaptureController> Ptr;

  public:
    CaptureController();

    int run();

    bool start();
    bool stop();

    inline bool isRunning() const
    {
      return m_isRunning;
    }

    inline void setCloudGrabber(CloudGrabberPtr grabber)
    {
      m_cloudGrabber = grabber;
    }

    inline void setIMUGrabber(IMU::IIMUGrabber::Ptr grabber)
    {
      m_imuGrabber = grabber;
    }

    inline void addPostCaptureTask(ProcessingTaskPtr task)
    {
      m_postCaptureOperations.push_back(task);
    }

    void addCaptureTrigger(Triggers::ITrigger::Ptr trigger);

  private:
    void triggerCapture();

    friend std::ostream &operator<<(std::ostream &s, const CaptureController &o);

  private:
    Common::LoggingService::Logger m_logger;

    bool m_isRunning;
    size_t m_currentFrameCount;

    CloudGrabberPtr m_cloudGrabber;
    IMU::IIMUGrabber::Ptr m_imuGrabber;

    std::vector<Triggers::ITrigger::Ptr> m_captureTriggers;

    std::vector<ProcessingTaskPtr> m_postCaptureOperations;
  };
}
}