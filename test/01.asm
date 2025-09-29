        ORG 200           / Start at address 0200 (octal)
        CLA             / Clear accumulator
        ADD VALUE      / Load 5 into AC
        ISZ RESULT      / Store AC into RESULT
        HLT             / Halt

VALUE,  DEC 5           / Define constant 5
RESULT, HEX BFAF               / Memory location to store result
