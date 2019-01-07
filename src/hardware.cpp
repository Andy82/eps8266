#include <hardware.h>

#define MAX_FREE_MEM 46224

void Hardware_init() {
  logger.log("SDK version:     " + String(ESP.getSdkVersion()));
  logger.log("Boot version:    " + String(ESP.getBootVersion()));
  logger.log("Boot mode:       " + String(ESP.getBootMode()));
  logger.log("Reset info:      " + String(ESP.getResetInfo()));
  logger.log("Reset reason:    " + String(ESP.getResetReason()));
  logger.log("CPU freq:        " + String(ESP.getCpuFreqMHz()) + " MHz");
  logger.log("Free memory:     " + String(ESP.getFreeHeap()) + " (" +  String(ESP.getFreeHeap() / (MAX_FREE_MEM / 100)) + " %)");
  logger.log("Chip ID:         " + String(ESP.getChipId()));
  logger.log("Flash ID:        " + String(ESP.getFlashChipId()));

  logger.log("Flash speed:     " + String(ESP.getFlashChipSpeed() / 1000000) + " MHz");
  logger.log("Flash mode:      " + String (ESP.getFlashChipMode()));
  logger.log("Flash size:      " + String (ESP.getFlashChipSize()));
  if (ESP.getFlashChipSize() != ESP.getFlashChipRealSize()) {
    logger.log("Flash real size: " + String (ESP.getFlashChipRealSize()));
  }
  logger.log("SPIFFS size:     " + String(ESP.getFlashChipRealSize() - (ESP.getSketchSize() + ESP.getFreeSketchSpace())));
  logger.log("Program size:    " + String(ESP.getSketchSize() + ESP.getFreeSketchSpace()));
  logger.log("Sketch size:     " + String(ESP.getSketchSize()));
  logger.log("Free for sketch: " + String(ESP.getFreeSketchSpace()));
  
}