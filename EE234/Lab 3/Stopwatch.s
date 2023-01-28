.text
.global main

.set TTC_BSE, 0xF8001000
.set SSD_BSE, 0x43C10000
.set BTN_BSE, 0x41200000

main:
	LDR r1,=BTN_BSE // We need to load the base memory value in r1 so we can
	                // read from here later to get the button values.

	LDR r2,=SSD_BSE // We want to load the base memory for the seven segment
	                // display so we can enable it and set the values for each
	                // digit.


	LDR r10,=TTC_BSE // We need to load the tripple counter into memory.

	MOV r0,#0x13 // configuration value for TTC

	STR r0,[r10,#0x00] // set config value for TTC

	LDR r0,=0x43B // config intervals value for TTC

	STR r0,[r10,#0x24] // set config intervals value for TTC

	MOV r0,#0x01 // config interrupt setup value for TTC

	STR r0,[r10,#0x60] // set config interrupt setup value for TTC

	MOV r0,#0x22 // confiugration mode value for TTC

	STR r0,[r10,#0x0C] // set confiugration mode value for TTC

	MOV r3,#0   // r3 is set to zero here so it can be used in the loop to
	            // determine if the button was released or depressed. We
	            // use zero as the inital condition since we assume the
	            // button is not depressed at program start.

	MOV r0, #0x01 // r0 is set to one so we can activate it for use. (see below for usage)

	STR r0,[r2] // enable 7 seg disp (default mode) by using the value set
	            // by r0, in short, 0x01 = ENABLED. Seven Segment Display (SSD)

	MOV r8, #0 // r8 is used for the value to display on the SSD. We start
               // counting at zero, so the inital value is zero.
    MOV r7, #0 // r7 is used for the value to display on the SSD. We start
               // counting at zero, so the inital value is zero.
	MOV r6, #0 // See comment above
	MOV r5, #0 // See comment above

	MOV r0,#0 // this is the variable for the current state.
			  // 0 = ST_RESET
			  // 1 = ST_RUN
			  // 2 = ST_STOP
			  // 3 = ST_BG

	BL sumNumbersSSD // Since r5-8 were set to 0, we can now set the display values to 0.

	// All code above in main will only run once. We want this because if we set
	// values in the stopwatch_loop loop below, it will reset variables every time it
	// runs, so we humans will never see anything work. If we reset r9 every time,
	// then the value will always be zero. If we reset r3 every time, we will never
	// know if the button changes state since we need to keep track of the previous
	// value.
stopwatch_loop:
// Note: Button 0 has priority 0.
	mov r12,#0 // reset change flag

	LDR r9,[r10,#0x54]

	CMP r9,#1 // check if the time should update.
	bleq check_update_stopwatch_loop

	LDR r4,[r1]

	EOR r4,r4,r3 // see if state changed

	PUSH {r4} // push r4 to stack for later retrival

	AND r4,r4,#0x1 // get first button changed.

// if button changed == 1
	CMP r4,#1
	BEQ ST_CHANGE_1 // if btn 1 is pressed, change state w/ Code 1

	POP {r4} // pop r4 from stack for retrival

	AND r4,r4,#0x8 // get leftmost button changed.

// else if button changed == 8
	CMP r4,#8
	BEQ ST_CHANGE_2 // if btn 3 is pressed, change state w/ Code 2

// else no buttons changed

	B stopwatch_loop

check_update_stopwatch_loop:

	cmp r0, #2 // if state is stop, do nothing, jump back to caller.
	bxeq lr

	push {lr}

	CMP r0, #0 // check is state is reset
	BLNE updateSSD // if state is not reset, add number and continue.

	pop {lr}
	bx lr
updateSSD:

	ADD r8,r8,#1 // update time by one, we know the state is not reset

	PUSH {LR} // push lr to stack so we can bl to other methods.

	CMP r8,#9
	BLGT updateSSD1 // r8 > 9 // 0th digit // Reset digit and add 1 to next
	CMP r7,#9
	BLGT updateSSD2 // r7 > 9 // 1st digit // Reset digit and add 1 to next
	CMP r6,#9
	BLGT updateSSD3 // r6 > 9 // 2nd digit // Reset digit and add 1 to next
	CMP r5,#5
	BLGT updateSSD4 // r5 > 5 // 3rd digit // Reset digit

	cmp r0, #3 // check is state is background.
	BLNE sumNumbersSSD // if the state is NOT background, update number

	POP {LR} // pop lr from stack to restore caller.
	BX LR
updateSSD1:
    ADD r7,r7,#1 // set 1st digit to r7+1
    MOV r8,#0 // set 0th digit to 0 [XY:Z0]
    BX LR
updateSSD2:
    ADD r6,r6,#1 // set 2nd digit to r6+1
    MOV r7,#0 // set 1st digit to 0 [XY:0Z]
    BX LR
updateSSD3:
    ADD r5,r5,#1 // set 3rd digit to r5+1
    MOV r6,#0 // set 2nd digit to 0 [X0:YZ]
    BX LR
updateSSD4:
    MOV r5,#0 // set 3rd digit to 0 [0X:YZ]
    BX LR
sumNumbersSSD:
	mov r9, r8 // copy r8 to r9 to set 0th digit

	push {r8} // borrowing r8 for computions, so we need to save it in the stack

	mov r8,r7,lsl #8 // shift r7 to the left storing into r8 for the first digit.
	add r9,r9,r8 // add first digit to the exisitng value.

	mov r8,r6,lsl #16 // shift r6 to the left storing into r8 for the second digit.
	add r9,r9,r8 // add second digit to the exisitng value.

	mov r8,r5,lsl #24 // shift r5 to the left storing into r8 for the third digit.
	add r9,r9,r8 // add third digit to the existing values.

	ldr r8,=#0x80008080 // Decmials // load the value of the decmial places into r8
	add r9,r9,r8 // add decmial places to r9. This will shows `AB.CD`.

	pop {r8} // restore the value of r8.

	STR r9,[r2,#4] // update the SSD.

	bx lr
ST_CHANGE_1: // button 0 pressed

	PUSH {LR} // preserve the value of caller in stack

	CMP r0, #0 // if current state is reset
	BLEQ SET_ST_RUN // set state run

	CMP r0, #1 // if current state is run
	BLEQ SET_ST_STOP // set state stop

	CMP r0, #2 // if current state is stop
	BLEQ SET_ST_RUN // set state run

	CMP r0, #3 // if current state is bg
	BLEQ SET_ST_STOP // set state stop

	BL debounce // debounce the button

	POP {LR} // retreve value of caller
	BX LR
ST_CHANGE_2: // button 3 pressed

	CMP r0, #0 // if current state is reset (No action)
	bxeq lr // jump back to caller, there is nothing to do; saves power consumption.

	PUSH {LR} // preserve value of caller on stack

	CMP r0, #1 // if current state is run
	BLEQ SET_ST_BG // set state to background.

	CMP r0, #2 // if current state is stop
	BLEQ SET_ST_RESET // set state to rst.

	CMP r0, #3 // if current state is bg
	BLEQ SET_ST_RUN // set state to run.

	BL debounce // debounce button.

	POP {LR} // restore caller's addr.
	BX LR
SET_ST_RESET:
	cmp R12, #1
	bxeq lr // state has already checked for button press, ignore this call
    mov r12,#1 // set state change

	push {lr} // push caller's address to the stack

	mov r0,#0 // set state 0 (rst)
	mov r5,#0 // LHS of left digits = 0 [X0:00]
	mov r6,#0 // RHS of left digits = 0 [0X:00]
	mov r7,#0 // LHS of right digits = 0 [00:X0]
	mov r8,#0 // RHS of right digits = 0 [00:0X]

	BL sumNumbersSSD // update the numbers after setting them to 0.

	pop {lr} // restore the callers address to lr.

	bx lr
SET_ST_RUN:
	cmp R12, #1
	bxeq lr // state has already checked for button press
	mov r12,#1 // set state change

	mov r0,#1 // set the state to run (to be handled with calls throughout pgrm)

	bx lr
SET_ST_STOP:
	cmp R12, #1
	bxeq lr // state has already checked for button press
	mov r12,#1 // set state change

	mov r0,#2 // set the state to stop (to be handled with calls throughout pgrm)

	push {lr}
	BL updateSSD // update screen
	pop {lr}

	bx lr
SET_ST_BG:
	cmp R12, #1
	bxeq lr // state has already checked for button press
	mov r12,#1 // set state change occured.

	mov r0,#3 // set the state to background (to be handled with calls throughout pgrm)

	bx lr
debounce:
    LDR r9,[r10,#0x54] // changed this from r11

	CMP r9,#1 // If this is not true, we want to keep waiting.

	BLT debounce // ~10ms have not passed, keep waiting.

    BX LR // Now that we have waited enough time for any bounces to occur,
          // Go back to the primary method.

.end
