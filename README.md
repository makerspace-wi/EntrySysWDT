# Entry_System WDT
### Watch dog timer for Entry System
##### Prozessor Typ: ATTiny25 5 Volt
##### Components:
- Controls signal form Entry System with 1500ms LOW and 500ms HIGH.
- white flash after signal change from high to low: signal is in tolerance
- Genarates a CPU reset if signal differs greater then 500ms HIGH or LOW
- If HIGH signal is greater then 10000ms, a POWER Reset is genarated.

##### Output Signals:
- white LED (short: signal ok; long: sinal error, reset )
- red LED left RESET Mini
- red LED right above POWER Reset

##### Input Signals:
- signal for MINI
- Serial input (not used in the moment)
