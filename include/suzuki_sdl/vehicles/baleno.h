/*
 * OBD Address map for Baleno 1.3L G13BB with ECU 33920-65GP.
 */
enum Baleno33920_65GP {
  SDL_FaultCodes1 = 0x00,
  SDL_FaultCodes2 = 0x01,
  SDL_FaultCodes3 = 0x02,
  SDL_FaultCodes4 = 0x03,
  SDL_RpmHigh = 0x04,
  SDL_RpmLow = 0x05,
  SDL_TargetIdle = 0x06,
  SDL_VehicleSpeedSensor = 0x07,
  SDL_EngineCoolantTemperature = 0x08,
  SDL_IntakeAirTemperature = 0x09,
  SDL_TpsAngle = 0x0A,
  SDL_AbsoluteThrottlePosition = 0x0B,
  SDL_InjectorPulseWidthHigh = 0x0D,
  SDL_InjectorPulseWidthLow = 0x0E,
  SDL_IgnitionAdvance = 0x0F,
  SDL_ManifoldAbsolutePressure = 0x10,
  SDL_BarometricPressure = 0x11,
  SDL_IscFlowDuty = 0x12,
  SDL_BatteryVoltage = 0x16,
  SDL_RadiatorFan = 0x19,
  SDL_StatusFlags = 0x1A,
  SDL_FaultCodes5 = 0x20,
  SDL_FaultCodes6 = 0x21,
};
