.text
.global main

.set LED_RGB, 0x43C00000 // base address for RGB LED.

.set ONE_MS, 0x036447D // One milisecond on blackboard.
.set TEN_MS, 0x021ECE5 // Ten miliseconds on blackboard.
.set ONE_S,  0xD408994 // One second on blackboard.

main:
	LDR r0,=LED_RGB // Load the base address into r0
	MOV r1, #1 // This is the variable that will help with us adding or
               // subtracting from the current width of the LED. This is
               // how the fade in/out can be accomplished. This value is
               // either 1 or -1 so that we can have r4+1 or r4+-1.

	MOV r2, #1 // r2 is the constant that we use to enable the LED lights.
               // This will always be 1 so we can enable. Only the LSB
               // matters since it reads 1 or 0 for enable or disable.

	MOV r3, #255 // This value does not fluxuate. This is the Period.
                 // It is set to a constant 255 since we want to declare
                 // the time between the rising edges. The
                 // period is the same no matter where the measurement is
                 // taken, but it is easiest to see it from rising edge to
                 // edge. If this value is too small, then we will not be
                 // able to see the light.

	MOV r4, #0 // This value varies from 0 to 255. Dependent on r1, this
               // value will increase or decrease based on r1 being +1 or
               // -1. Every loop, it will be incremented by r1's value.
               // We want to vary this value because it controlls the
               // brightness of the given LED (red, green, or blue). This
               // is the duty cycle. Our eye will see the average over some
               // time becuase this value is rising during the duty cycle
               // then falls for the remaining of the period until the duty
               // cycle is active again. When the value is the same as the
               // period, The duty cycle is 100%, so the average will be
               // around 100. This is why only the width should be changed
               // - to vary the duty cycle. The duty cycle is how long the
               // value is active within each period.

   // NOTE: r5, r6, r7 are set in `resetLights`.

	MOV r8, #0x00 // This value, r8, will be used in the delay section.
                  // It will be used to wait a certian time based on r9,
                  // the max value. It will increase by one every run and
                  // be reset everytime it delays enough time.

	LDR r9,=TEN_MS // r9 is the max value that r8 will count to. We use this
                   // value since it is calculated based on the board's clock
                   // speed. This constant is approx 10ms. If we wanted 20ms,
                   // we could shift r9 to the left once.

    B resetLights // Called to set the offsets that will be used in loop. We
                  // call it here becuase it will reduce the amount of lines
                  // used for instructions and we reuse the same code over
                  // and over again.
loop:

	STR r2, [r0, r5] // Here, we want to store the value of r2 (always 1) to
                     // enable the led at the offset r5 (R G or B) from
                     // the base r0 (RGB Led Address). Starting, this will be
                     // the red led. It will cycle through red, green, blue,
                     // and continue until power is lost on the board or a reset.

    STR r3, [r0, r6] // Here, we want to store the value of r3 (always 255) to
                     // set the period of the led at the offset r6 (R G or B) from
                     // the base r0 (RGB Led Address). Starting, this will be
                     // the red led. It will cycle through red, green, blue,
                     // and continue until power is lost on the board or a reset.

    STR r4, [r0, r7] // Here, we want to store the value of r4 (changes [0,255]) to
                     // set the width of the led at the offset r6 (R G or B) from
                     // the base r0 (RGB Led Address). This is what allows us to
                     // change the brightness. Starting, this will be the red led.
                     // It will cycle through red, green, blue, and will continue
                     // until power is lost on the board or a reset.

	ADD r4,r4,r1 // Add or substract one from r4 (the width). Starting, this will
                 // be adding one to r4 increaseing the brightness to 100% from 0%.
                 // Once 100% is reached, r1 will become negative and continue until
                 // r4 equals zero. At this point, the led will change color and we
                 // will begin adding up to 255 then repeat the process.

    CMP r4,#255 // We want to see if r4 has achieved the max value it can possibly have.
                // If this is equal to eachother, the following line, `BEQ negOne` will
                // run. If they are not equal, we move onto the next comparison.

    BEQ negOne // If the previous line, `CMP r4,#255` is true, we will run the code in
               // negOne which simply sets r1 to -1 so we can do subtraction with the
               // add command. This saves us a few lines explicitly doing division and
               // subtraction.

    CMP r4,#0 // We want to see if r4 has achieved the min value it can possibly have.
              // If this is equal to eachother, the following line, `BEQ nextLight` will
              // run. If they are not equal, we move onto the delay.

    BEQ nextLight // If the previous line, `CMP r4,#0` is true, we will run the code in
                  // nextLine which will update the color we are minipulating now. It
                  // may branch to another section of code.

	B delay // If r4 ≠ 0 and r4 ≠ 255, we must delay so the colors do not ramp up the
            // brightness and return to zero without it being visible to the human eye.
            // We may see the color changes as very quick flashes, but humans cannot see
            // as fast as the computer can run this.

negOne:
// Here, we want to set r1 to negative one, by doing this, we can still use ADD and
// subtract one from the current brightness to ramp up and down the value. Then we
// continue the loop

    MOV r1,#-1
    B loop
nextLight:
// This is more involved as we want to change the offset values. We want to make sure
// there are no incorrect offsets so we must use a condition branch statement. Once
// instructions are done, reguardless of the path, we will end up in the primary loop,
// named `loop`.

    MOV r1,#1 // We want to set r1 back to one so we increase the brightness on the LED.
              // Unlike the negOne, this is called with other code, becuase we only want
              // one when a new color is coming.


    CMP r5,#0x00 // We want to compare the offset, r5, with zero becuase it is the base
                 // address and we do not want to start setting values at -0x10 since we
                 // do not know what we will be controlling at this point. It could be
                 // harmful to the system.

	BLE resetLights // If the comparison is true, we want to reset the lights to the red
                    // memory address. See below for what happens in resetLights. See
                    // the CMP description above to know why we care about this value.

// Here, we want to subtract 0x10 from r5, r6, and r7 becuase we want to move to the next LED.
// This is safe to do becuase we go (in r5) from 0x20 to 0x00 without any incorrect writing.
// We are guaranteed an output at these positions. We do 0x10 becuase we want to change the LED,
// not the offset amount. An example of 0x1y where y is some value ≠ 0, may lead to unexpected
// behaviors and possibly dangerous behaviors on the board.
    SUB r5,r5,#0x10
    SUB r6,r6,#0x10
    SUB r7,r7,#0x10

	B loop // Start the loop and being adjusting the brightness again.
resetLights:
// This will run when r5, the offset for enable is zero so we know to start back at
// red, 0x20. We want to run this code because if we do not reset, the values will
// become negative and the address we try writting to may not exist or damage the
// board (i.e. a sensitive input may not be able to handle output that will be
// passed through). This is also called from main becuase it removes two lines of code.

    MOV r5, #0x20 // This offset is used to set the enable signal for a
        // particular led. This value starts at 0x20 and
        // decrements to 0x00 by increments of 0x10. We want
        // to change this to reduce the number of lines which
        // will decrease power usage. The offset 0x20 is for the
        // Red light, 0x10 for Green, and 0x00 for Blue. This
        // value will always be set to one (Enable/On).

    MOV r6, #0x24 // This offset is used to set the period signal for a
        // particular led. This value starts at 0x24 and
        // decrements to 0x04 by increments of 0x10. We want
        // to change this to reduce the number of lines which
        // will decrease power usage. The offset 0x24 is for the
        // Red light, 0x14 for Green, and 0x04 for Blue. This
        // value is a constant 255 for the period.

    MOV r7, #0x28 // This offset is used to set the period signal for a
        // particular led. This value starts at 0x28 and
        // decrements to 0x08 by increments of 0x10. We want
        // to change this to reduce the number of lines which
        // will decrease power usage. The offset 0x28 is for the
        // Red light, 0x18 for Green, and 0x08 for Blue. This
        // value varies [0,255]. It inherits the value from r4.

    B loop
delay:
// Note: all instructions here take one clock cycle.
	ADD r8,r8,#1 // Increment r8 by one each few clock cycle so we can count
                 // to r9, the 10ms value for a 667MHz clock.

	CMP r8,r9 // We want to compare r8 and r9 (max value, theoretical
              // 10ms value). While this is not equal, we want to count
              // up to delay the code running.

	BLT delay // As stated above, we want to repeat this until r8 and
              // r9 hold the same values. r8 ≠ r9, so loop again. If
              // this condition is false, go the the next line.

    MOV r8,#0 // Reset r8 to zero so we can count again without any errors.
              // If we did not have this instruction, then r8 would always
              // be equal or greater than r9 and the delay would be 4 clock
              // cycles (plus instructions in other areas) rather than a
              // measurable time).

    B loop // We are done counting, the delay has occured and now we can
           // resume our operation.

.end
