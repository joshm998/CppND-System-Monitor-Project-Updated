#include "processor.h"
#include <unistd.h>
#include <iostream>

Processor::Processor() {
  all = LinuxParser::Jiffies();
  idle = LinuxParser::IdleJiffies();
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long allOld = all;
  long idleOld = idle;
  all = LinuxParser::Jiffies();
  idle = LinuxParser::IdleJiffies();
  float totalAll = ((float)all - (float)allOld);
  float totalIdle = ((float)idle - (float)idleOld);
  float value = (((float)totalAll - (float)totalIdle) / (float)totalAll);
  return (value > 0.0) ? value : 0.0;
}