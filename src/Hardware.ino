#define MAX_FREE_MEM 46224

void Hardware_init() {
  Serial.print(F("SDK version:     ")); Serial.println(ESP.getSdkVersion());
  Serial.print(F("Boot version:    ")); Serial.println(ESP.getBootVersion());
  Serial.print(F("Boot mode:       ")); Serial.println(ESP.getBootMode());
  Serial.print(F("Reset info:      ")); Serial.println(ESP.getResetInfo());
  Serial.print(F("CPU freq:        ")); Serial.print(ESP.getCpuFreqMHz()); Serial.println(" MHz");
  Serial.print(F("Free memory:     "));
  Serial.print(ESP.getFreeHeap());
  Serial.print(F(" ("));
  printFreeMemP();
  Serial.println(F("%)"));

  Serial.printf("Chip ID:         %08X\n", ESP.getChipId());
  Serial.printf("Flash ID:        %08X\n", ESP.getFlashChipId());

  Serial.print(F("Flash speed:     ")); Serial.print(ESP.getFlashChipSpeed() / 1000000); Serial.println(" MHz");
  Serial.print(F("Flash mode:      ")); Serial.println(ESP.getFlashChipMode());
  Serial.print(F("Flash size:      ")); Serial.println(ESP.getFlashChipSize());
  if (ESP.getFlashChipSize() != ESP.getFlashChipRealSize()) {
    Serial.print(F("Flash real size: ")); Serial.println(ESP.getFlashChipRealSize());
  }
  Serial.print(F("SPIFFS size:     ")); Serial.println(ESP.getFlashChipRealSize() - (ESP.getSketchSize() + ESP.getFreeSketchSpace()));
  Serial.print(F("Program size:    ")); Serial.println(ESP.getSketchSize() + ESP.getFreeSketchSpace());
  Serial.print(F("Sketch size:     ")); Serial.println(ESP.getSketchSize());
  Serial.print(F("Free for sketch: ")); Serial.println(ESP.getFreeSketchSpace());
}

void printFreeMemP() {
  Serial.print(ESP.getFreeHeap() / (MAX_FREE_MEM / 100));
}
