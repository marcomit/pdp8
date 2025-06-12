        ORG *0200
        CLA
        TAD COUNTER     / Load COUNTER
        DCA TEMP        / Store to TEMP
LOOP,   ISZ TEMP        / Increment TEMP; skip next if result is 0
        JMP LOOP        / Loop until TEMP wraps around
        HLT

COUNTER,    DEC -3      / Loop 3 times
TEMP,       0
