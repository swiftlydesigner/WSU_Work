.text
.global main

.set LED_CTL, 0x41210000
.set LED_RGB, 0x43C00000
.set SW_DATA, 0x41220000

main:
	LDR r1,=SW_DATA
	LDR r2,=LED_RGB
	MOV r9, #255
loop:
	LDR r0,[r1]	@load switch value, (*r1) ->r0
	AND r0,r0,#0xF @ set the bits if they are on
	AND r3,r0,#0x08 @ isolate the controlling bit

	MOV r3,r3,LSR #1 @ shift to right one bit to check for blue (0100)
	AND r4,r3,r0 @ check if both the switch and controlling bit are on
	MOV r4,r4, LSR #2 @ shift bit to LSB position for setting blue.
	STR r4,[r2, #0x00] @ BLUE
	STR r9,[r2, #0x04] @ BLUE PER
	STR r9,[r2, #0x08] @ BLUE WID

	MOV r3,r3,LSR #1 @ shift to right one bit to check for green (0010)
	AND r4,r3,r0 @ check if both the switch and controlling bit are on
	MOV r4,r4,LSR #1
	STR r4,[r2, #0x10] @ GREEN EN
	STR r9,[r2, #0x14] @ GREEN PER
	STR r9,[r2, #0x18] @ GREEN WID

	MOV r3,r3,LSR #1 @ shift to right one bit to check for red (0001)
	AND r4,r3,r0 @ check if both the switch and controlling bit are on
	STR r4,[r2, #0x20] @ RED EN
	STR r9,[r2, #0x24] @ RED PER
	STR r9,[r2, #0x28] @ RED WID

	B loop

	@STR r0,[r2, #0x00] @ BLUE
	@STR r0,[r2, #0x10] @ GREEN
	@STR r0,[r2, #0x20] @ RED

.end
