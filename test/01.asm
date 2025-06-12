        ORG 200           / Start at address 0200 (octal)
        CLA             / Clear accumulator
        TAD VALUE       / Load 5 into AC
        DCA RESULT      / Store AC into RESULT
        HLT             / Halt

VALUE,  DEC 5           / Define constant 5
RESULT, 0               / Memory location to store result
