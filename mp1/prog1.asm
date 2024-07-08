;
; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop


; R3 -> Points to Current Histogram Location (starts at x3f00)
; R4 -> mem[R3]
; R1 -> Character Counter, starts at 0 and when reaches 4 prints a character
; R2 -> Line Counter, starts at 0 and when reaches 16 prints newline
; R5 -> Current Ascii Value (starts at '@' and goes to 'Z')
; R6 -> temp register 

; An inner loop takes the four MSB of the current histogram location and prints it, while the outerloop checks if there has been 16 loops of the current histogram location (one character is done) and moves on to the next histogram location. Taking the four MSB is done through the same algorithm as the lab. R6 is used to load .FILL values and setting condition codes. 

PRINT_HIST 	
	LD R3, HIST_ADDR ;
	LDR R4, R3, #0 ;
	LD R5, AT_HEX ;
	AND R1, R1, #0 ;
        AND R2, R2, #0 ; initalizing registers
	
	PRINT_CHAR
		LD R6, NEG_Z ;
		ADD R6, R5, R6 ; checking if done with all characters
		BRz DONE ;
        	ADD R0, R5, #0 ;
		OUT
		LD R0, SP_HEX ;
		OUT           ; print character and space
		AND R0, R0, #0 ; reset R0 in order to hold the number in histogram

		LINE_LOOP 
			LD R6, NEG_SIXTEEN ;
			ADD R6, R2, R6 ;
			BRz PRINT_NEWLINE ; check if one character is finished (thus print newline and move to next character)

			CHAR_LOOP
				ADD R6, R1, #-4 ;
				BRz PRINT_NUM
				ADD R0, R0, R0 ;
				ADD R4, R4, #0 ;
				BRzp SHIFT ;
				ADD R0, R0, #1 ;
				SHIFT ADD R4, R4, R4 ;
				ADD R2, R2, #1 ;
				ADD R1, R1, #1 ;
				BRnzp CHAR_LOOP

			PRINT_NUM
				ADD R6, R0, #-10 ;
				BRn IF_NUMERIC   ; If the number is 0-9 there is a different offset than if it were A-F, so if 0-9 branch somewhere else
				LD R6, ADD_CHAR ;
				ADD R0, R0, R6 ;
				OUT
				AND R1, R1, #0 ;
				AND R0, R0, #0 ;
				BRnzp LINE_LOOP ;

				IF_NUMERIC 
					LD R6, ADD_NUM ;
					ADD R0, R0, R6 ;
					OUT
					AND R1, R1, #0 ;
					AND R0, R0, #0 ;
					BRnzp LINE_LOOP
			
			PRINT_NEWLINE 
				LD R0, NEWLINE_HEX ;
				OUT
				AND R0, R0, #0 ;
				ADD R5, R5, #1 ;
				AND R2, R2, #0 ;
				ADD R3, R3, #1 ;
				LDR R4, R3, #0 ; reset counters and initalize next histogram address
				BRnzp PRINT_CHAR

DONE	HALT			; done


; the data needed by the program
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00     ; histogram starting address
STR_START	.FILL x4000	; string starting address

AT_HEX          .FILL x0040     ; '@'
SP_HEX          .FILL x0020     ; ' '
NEG_Z           .FILL xFFA5     ; additive inverse of ASCII 'Z'
ADD_NUM         .FILL x0030     ; binary to hex for numbers 0-9, add this
ADD_CHAR        .FILL x0037     ; binary to hex for letters A-F, add this
NEG_SIXTEEN     .FILL xFFF0
NEWLINE_HEX     .FILL x000A     ; '\n'



; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END
