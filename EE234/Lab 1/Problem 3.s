.text
.global main

.set LED_CTL, 0x41210000
.set SSD_BSE, 0x43C10000
.set BTN_BSE, 0x41200000

main:
	LDR r1,=BTN_BSE
	LDR r2,=SSD_BSE
	MOV r3,#0x00 @ define r3 as Zero, Will be used for testing rising edge.
	MOV r0, #0x01
	STR r0,[r2] @ enable 7 seg disp (default mode)
	MOV r9, #0x00 @ define the value for SSD.
loop:
	LDR r0,[r1]	@ load button value, (*r1) ->r0

	AND r0,r0,#0x1 @ get first button.

	EOR r4,r0,r3 @ determine if it is a state change.

	AND r4,r4,r0 @ determine if state changed and value is one

	ADD r9,r9,r4 @ add the state line 22 to r9, possibly incrementing the value.

	MOV r3,r0 @ update current state

	STR r9,[r2,#0x04]

	B loop
.end
