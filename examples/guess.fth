\ guess a random number

.( Guess a random number from 0-9) CR

RAND 10 MOD CONSTANT N

: GUESS ( -- g )
    KEY [CHAR] 0 -
    DUP N = IF EXIT THEN
    DUP N < IF S" Too low" ELSE S" Too high" THEN
    TYPE CR ;

BEGIN GUESS N = UNTIL

.( You guessed it!) CR
.( The number was ) N . CR
