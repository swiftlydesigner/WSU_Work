.text
.global main

.set LED_CTL, 0x41210000
.set SW_DATA, 0x41220000


main:
	LDR r1,=SW_DATA
	LDR r2,=LED_CTL
loop:
	LDR r0,[r1]    @load switch value, (*r1) ->r0
	AND r0,r0,#0xF @ set the bits if they are on
	STR r0,[r2]    @ load the values into the leds
	B loop

.end
