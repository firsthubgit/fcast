#include "adapter.h"

#include "tcd/tcd_engine_def.h"
#include "tcd/tcd_engine_sender.h"

int foo() {
  tcd::TCDEngineSender* sender_ = tcd::GetTCDSenderEngine();
  return 42;
}
