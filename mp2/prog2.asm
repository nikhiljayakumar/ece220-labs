; Use GETC and OUT to echo an input to the screen. If character is '=', pop the final value into R5, and print
; hex value if stack is empty otherwise print "Invalid Expression," and halt the program. Also if space (' ') just 
; echo and wait for next character. If not '=' or ' ' run EVALUATE subroutine. In EVALUATE subroutine first check 
; if it is a number. If it is then push onto stack. If not pop two numbers off the stack and find which operand to use.
; Once found operand, call appropriate subroutine and push value onto stack, then go back to the main loop for
; next character. If not any of the operands means print invalid and halt. Repeat until '=' or invalid. 

.ORIG x3000
	
MAIN_LOOP
	GETC ;
	OUT ;

	LD R6, EQUAL_INVERSE ;
	ADD R6, R6, R0 ;
	BRz FINISH ; If '=' run FINISH sequence. 

	LD R6, SPACE_INVERSE ;
	ADD R6, R6, R0 ; If ' ' go back to MAIN_LOOP. 
	BRz MAIN_LOOP ;

	JSR EVALUATE ; 
	BRnzp MAIN_LOOP ;


FINISH 
	JSR POP ;
	ADD R5, R5, #0 ;
	BRnp INVALID_EXP ; If underflow, print invalid and halt. 
	JSR POP ;
	ADD R5, R5, #0 ;
	BRz INVALID_EXP ; If not underflow, print invalid and halt.
	ADD R5, R0, #0 ;
	ADD R3, R0, #0 ;
	JSR PRINT_HEX ; If underflow after dummy pop, print hex.
	BRnzp DONE ;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3- value to print in hexadecimal
;callee save R2, R4

; See MP1 for explanation of this subroutine
PRINT_HEX
	ST R2, PRINT_SaveR2 ;
	ST R4, PRINT_SaveR4 ;
	ST R7, PRINT_SaveR7 ;
	AND R2, R2, #0 ;  R2 holds 4 counter
	AND R4, R4, #0 ;  R4 holds 16 counter
	AND R0, R0, #0 ;

	PRINT_LOOP 
		LD R6, NEG_SIXTEEN ;
		ADD R6, R4, R6 ;
		BRz PRINT_DONE ;

		CHAR_LOOP
			ADD R6, R2, #-4 ;
			BRz PRINT_NUM ;
			ADD R0, R0, R0 ;
			ADD R3, R3, #0 ;
			BRzp SHIFT ;
			ADD R0, R0, #1 ;
			SHIFT ADD R3, R3, R3 ;
			ADD R2, R2, #1 ;
			ADD R4, R4, #1 ;
			BRnzp CHAR_LOOP ;

		PRINT_NUM 
			ADD R6, R0, #-10 ;
			BRn IF_NUMERIC 
			LD R6, ADD_CHAR ;
			ADD R0, R0, R6 ;
			OUT
			AND R2, R2, #0 ;
			AND R0, R0, #0 ;
			BRnzp PRINT_LOOP ;

			IF_NUMERIC 
				LD R6, ADD_NUM ;
				ADD R0, R0, R6 ;
				OUT
				AND R2, R2, #0 ;
				AND R0, R0, #0 ;
				BRnzp PRINT_LOOP ;

	PRINT_DONE 
		LD R2, PRINT_SaveR2 ;
		LD R4, PRINT_SaveR4 ;
		LD R7, PRINT_SaveR7 ;
		RET ;


PRINT_SaveR2 .BLKW #1 ;
PRINT_SaveR4 .BLKW #1 ;
PRINT_SaveR7 .BLKW #1 ;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R6 - current numerical output
EVALUATE
	ST R7, EVAL_SaveR7 ;

	LD R6, LWB_INVERSE ; checks if number (above lower bound)
	ADD R6, R6, R0 ;
	BRzp #1 ;
	BRnzp IF_OPERAND ;
	
	LD R6, UPB_INVERSE ; checks if number (below upper bound)
	ADD R6, R6, R0 ;
	BRp IF_OPERAND

	LD R6, LWB_INVERSE ; shift input from ascii value to actual number
	ADD R0, R0, R6 ;
	JSR PUSH ;           if number then push it onto the stack and finish eval
	BRnzp FINISH_EVAL ;


	IF_OPERAND         ; if not number, assume its operand and pop two numbers off stack
		ADD R2, R0, #0 ;
		JSR POP ;
		ADD R5, R5, #0 ;
		BRnp INVALID_EXP ;
		ADD R4, R0, #0 ;
		JSR POP ;
		ADD R5, R5, #0 ;
		BRnp INVALID_EXP ;
		ADD R3, R0, #0 ;

		IF_PLUS
			LD R6, PLUS_INVERSE ;
			ADD R6, R6, R2 ;
			BRnp IF_MIN ;   if not this operator check next operator
			JSR PLUS ;      if it is this operator perform subroutine and add it onto stack, then finish evaluation
			JSR PUSH ;
			BRnzp FINISH_EVAL ;

		IF_MIN
			LD R6, MIN_INVERSE ;
			ADD R6, R6, R2 ;
			BRnp IF_MUL ;
			JSR MIN ;
			JSR PUSH ;
			BRnzp FINISH_EVAL ;

		IF_MUL
			LD R6, MUL_INVERSE ;
			ADD R6, R6, R2 ;
			BRnp IF_DIV ;
			JSR MUL ;
			JSR PUSH ;
			BRnzp FINISH_EVAL ;

		IF_DIV
			LD R6, DIV_INVERSE ;
			ADD R6, R6, R2 ;
			BRnp IF_EXP ;
			JSR DIV ;
			JSR PUSH ;
			BRnzp FINISH_EVAL ;

		IF_EXP
			LD R6, EXP_INVERSE ;
			ADD R6, R6, R2 ;
			BRnp INVALID_EXP ; If none of these operators then its definetly invalid expression
			JSR EXP ;
			JSR PUSH ;
			BRnzp FINISH_EVAL ;


	FINISH_EVAL ;
		LD R7, EVAL_SaveR7 ; reload R7 (b/c nested subroutine) then return
		RET ;

EVAL_SaveR7 .BLKW #1 ;

; print "Invalid Expression" and automatically halt
INVALID_EXP
	LEA R0, INVALID ;
	PUTS ;
	BRnzp DONE ;
	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
PLUS	
	ADD R0, R3, R4 ;
	RET


	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MIN
	NOT R4, R4 ;
	ADD R4, R4, #1 ;
	ADD R0, R3, R4 ;
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MUL	
	AND R0, R0, #0 ;
	ADD R4, R4, #0 ;
	BRz DONE_MUL ;
	BRzp IF_NOT_NEGATIVE

	NOT R4, R4 ;
	ADD R4, R4, #1 ;
	NOT R3, R3 ;
	ADD R3, R3, #1 ; switching the signs of both numbers if the second parameter is negative 
				   ; 3*(-4)=(-3)*4. This way second param can always be loop counter. 

	IF_NOT_NEGATIVE
	MUL_LOOP 
		ADD R0, R0, R3 ;
		ADD R4, R4, #-1 ;
		BRz DONE_MUL ;
		BRnzp MUL_LOOP ;
	
	DONE_MUL RET ;
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0

; subtracts second param (R4) from first param (R3) until first param negative, then outputs R0 (which is incremented 
; for every subtraction). If one of params negative make positive then make R0 negative right before end.
; If both params negative make both positive and perform like normal. 
DIV	
	ADD R3, R3, #0 ;
	BRn R3_NEGATIVE ;
	ADD R4, R4, #0 ;
	BRz DONE ;
	BRn R4_NEGATIVE ;
	BOTH_POSITIVE ;
		AND R0, R0, #0 ;
		NOT R4, R4 ;
		ADD R4, R4, #1 ;
		DIV_POS_LOOP 
			ADD R3, R3, R4 ;
			BRnz POSITIVE_DONE ;
			ADD R0, R0, #1 ;
			BRnzp DIV_POS_LOOP ;

		POSITIVE_DONE 
			ADD R3, R3, #0 ;
			BRn DONE_DIV ;
			ADD R0, R0, #1 ;
			BRnzp DONE_DIV ;
		
	FLIP_BOTH
		NOT R3, R3 ;
		ADD R3, R3, #1 ;
		NOT R4, R4 ;
		ADD R4, R4, #1 ;
		BRnzp BOTH_POSITIVE ;


	R3_NEGATIVE 
		ADD R4, R4, #0 ;
		BRn FLIP_BOTH ;
		NOT R3, R3 ;
		ADD R3, R3, #1 ;
		
		AND R0, R0, #0 ;
		NOT R4, R4 ;
		ADD R4, R4, #1 ;
		BRnzp DIV_NEG_LOOP

	R4_NEGATIVE
		ADD R3, R3, #0 ;
		BRn FLIP_BOTH ;
		NOT R4, R4 ;
		ADD R4, R4, #1 ;

		AND R0, R0, #0 ;
		NOT R4, R4 ;
		ADD R4, R4, #1 ;
		DIV_NEG_LOOP 
			ADD R3, R3, R4 ;
			BRnz NEGATIVE_DONE ;
			ADD R0, R0, #1 ;
			BRnzp DIV_NEG_LOOP ;

	NEGATIVE_DONE 
		ADD R3, R3, #0 ;
		BRn NEGATIVE_FLIP ;
		ADD R0, R0, #1 ;
		BRnzp NEGATIVE_FLIP ;

	NEGATIVE_FLIP
		NOT R0, R0 ;
		ADD R0, R0, #1 ;

	DONE_DIV RET ;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;callee save R7, R6
;out R0

; R6 <- R4 (counter for multiplication loop)
; R4 <- R3 (first param e.g. the 2 in 2^4)
; R3 holds current product
; loop through and multiply R3 and R4 until R6 is zero, then store the product in R0.
; assumes positive numbers for inputs?
EXP
	ST R7, EXP_SaveR7 ;
	ST R6, EXP_SaveR6 ;
	ST R3, EXP_SaveR3 ;

	AND R0, R0, #0 ;
	ADD R4, R4, #0 ;
	BRz ANS_ONE ;
	ADD R4, R4, #-1 ;
	BRz ANS_R3 ;

	ADD R6, R4, #1 ;

	EXP_LOOP
		ADD R6, R6, #-1 ;
		BRz DONE_EXP ;
		LD R4, EXP_SaveR3 ;
		JSR MUL ;
		ADD R3, R0, #0 ;
		BRnzp EXP_LOOP ;

	ANS_ONE 
		ADD R0, R0, #1 ;
		BRnzp DONE_EXP ;
	ANS_R3
		ADD R0, R3, #0 ;
		BRnzp DONE_EXP ;

    DONE_EXP 
		LD R6, EXP_SaveR6 ;
		LD R7, EXP_SaveR7 ;
		RET ;

EXP_SaveR3 .BLKW #1 ;
EXP_SaveR6 .BLKW #1 ;
EXP_SaveR7 .BLKW #1 ;
	
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;

DONE HALT ;

STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;

SPACE_INVERSE .FILL #-32 ;
EQUAL_INVERSE .FILL #-61 ;
PLUS_INVERSE .FILL #-43 ;
MIN_INVERSE .FILL #-45 ;
MUL_INVERSE .FILL #-42 ;
DIV_INVERSE .FILL #-47 ;
EXP_INVERSE .FILL #-94 ; used to check what operand or '=' or ' '

LWB_INVERSE .FILL #-48 ;
UPB_INVERSE .FILL #-57 ; used to check if input is a number

ADD_NUM         .FILL x0030     ; binary to hex for numbers 0-9, add this
ADD_CHAR        .FILL x0037     ; binary to hex for letters A-F, add this
NEG_SIXTEEN     .FILL xFFF0

INVALID .STRINGZ "Invalid Expression" ;

.END