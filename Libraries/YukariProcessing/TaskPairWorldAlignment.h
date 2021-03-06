/** @file */

#pragma once

#include "ITaskWorldAlignment.h"

#include <YukariCommon/LoggingService.h>

namespace Yukari
{
namespace Processing
{
  class TaskPairWorldAlignment : public ITaskWorldAlignment
  {
  public:
    TaskPairWorldAlignment(const boost::filesystem::path &path,
                           std::map<std::string, std::string> &params);

  protected:
    virtual void doAlignment(Task t) override;

  private:
    Common::LoggingService::Logger m_logger;
  };
}
}
