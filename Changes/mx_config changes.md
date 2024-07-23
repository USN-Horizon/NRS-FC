# Changes in CubeMX

System Clock from 50Hz to 168MHz

Prescaler from 15 to 168. Calculated as Prescaler = System Clock / Timer Clock. 168 = 168MHz / 1 MHz, where we solve for 1MHZ as the Timer Clock

Counter period is set to 20000 from 65535. This gives a 20ms period

Pulse Width is calculated as (Desired angle / 180) * (2000 - 1000) + 1000. Minimum pulse width (0 degrees) = 1ms = 1000 counts. Maximum pulse width (180 degrees) = 2ms = 2000 counts.

