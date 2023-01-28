.text
.global main

.set LED_CTL, 0x41210000
.set SSD_BSE, 0x43C10000
.set BTN_BSE, 0x41200000

.set ONE_MS, 0x036447D // one milisecond on blackboard.
.set TEN_MS, 0x021ECE5 // ten miliseconds on blackboard.
.set ONE_S,  0xD408994 // One second on blackboard.

main:
	LDR r1,=BTN_BSE // We need to load the base memory value in r1 so we can
	                // read from here later to get the button values.

	LDR r2,=SSD_BSE // We want to load the base memory for the seven segment
	                // display so we can enable it and set the values for each
	                // digit.

	MOV r3,#0x00 // r3 is set to zero here so it can be used in the loop to
	             // determine if the button was released or depressed. We
	             // use zero as the inital condition since we assume the
	             // button is not depressed at program start.

	MOV r0, #0x01 // r0 is set to one so we can activate it for use. This
	              // variable will later be used to get the button values.

	STR r0,[r2] // enable 7 seg disp (default mode) by using the value set
	            // by r0, in short, 0x01 = ENABLED. Seven Segment Display (SSD)

	MOV r9, #0x00 // r9 is used for the value to display on the SSD. We start
	              // counting at zero, so the inital value is zero.

	MOV r10,#0 // r10 will be used to debounce the button. This declartion is
	           // required, but helps with readability so we know it will appear
	           // later. This will only be used in the debounce loop.

	LDR r11,=TEN_MS // This will set r11 to the value it takes approx 10ms to
	                // count up to on the blackboard. This is dependent on the
	                // clock cycle. This will only be used to compare in the
	                // debounce loop.

	// All code above in main will only run once. We want this because if we set
	// values in the seven_seg loop below, it will reset variables every time it
	// runs, so we humans will never see anything work. If we reset r9 every time,
	// then the value will always be zero. If we reset r3 every time, we will never
	// know if the button changes state since we need to keep track of the previous
	// value.
seven_seg:
	LDR r0,[r1]	// load button value, (*r1) ->r0.
	            // We want to get the button values so we can add to the displayed
	            // value, if needed.

	AND r0,r0,#0x1 // Get first button. We want to ignore the other buttons.

	EOR r4,r0,r3 // Use EOR to compare the value of r3 to r0. r3 contains the
	             // previous button state (pressed or released). r0 is the
	             // current button state. We use exclusive or since we want
	             // to check if they are the same or different values. Store
	             // the result into r4. If r4 is 1, then we know the button is
	             // has recently be pressed or released; we do not know which
	             // at this point. If r4 is 0, then we know the button is in a
	             // steady pressed or released state; in this case, we do not
	             // care. This will be know as the state change variable.

	AND r4,r4,r0 // Now that we know r4 (if it is a state change), we can
	             // AND this with r0, the button value, to see if the button
	             // was recently pressed or released. If the button was
	             // recently presessed, r4 will remain one. If the button
	             // was recently released, r4 will become zero. If r4 was
	             // zero to start (no state change), its postcondition value
	             // is guaranteed to be zero. In essence, we are checking for
	             // a rising edge.

	ADD r9,r9,r4 // r4 is guaranteed to be zero or one. If it is zero, r9
	             // will remain unchanged and nothing will change. If the
	             // value is one, r9 will increase by one. This means the
	             // button recently had a rising edge. The value is
	             // unconstrained at this point.

	AND r9,r9,#0xF // This will ensure the value is between 0 and F so other
	               // attributes are not changed in the SSD.

	MOV r3,r0 // This will set r3 to the current state of the button, so we
	          // can repeat the process over again.

	STR r9,[r2,#0x04] // This will set the value of r9, our counter value,
	                  // to the zeroth digit in the SSD. We are accessing
	                  // r2, offset by 0x04. This means to access our SSD
	                  // base memory address and offset it by four so we
	                  // can change a specific value, in this case the 0th
	                  // digit's value. When we use this format, we do not
	                  // change the value of r2 itself, but rather offset
	                  // our target in this instance only.

	CMP r4,#1 // If r4 is one, that means our button had a rising edge.
	          // If this condition is true, we want to debounce our
	          // press to ensure that electrical currents are not caught
	          // twice or more when the physical button was only pressed
	          // once.

	BEQ debounce // Jump to debounce to if the CMP value is true so we can
	             // debounce the button.

	B seven_seg // Debounce was not needed, so loop over this code again.
debounce:
// Note: all instructions here take one clock cycle.
	ADD r10,r10,#1 // Increment r10 by one each clock cycle so we can count
	               // to r11, the 10ms value for a 667MHz clock.

	CMP r10,r11 // We want to compare r10 and r11 (max value, theoretical
	            // 10ms value). While this is not equal, we want to count
	            // up to prevent any unwanted readings.

	BLT debounce // As stated above, we want to repeat this until r10 and
	             // r11 hold the same values.

    MOV r10,#0 // Reset r10 to zero so we can count again without any errors.

    B seven_seg // Now that we have waited enough time for any bounces to occur,
                // Go back to the primary method.

.end

/** NOTE: Clock cyce is 667MHz */
