        ORG *0200
        CLA
        TAD ARG         / Load 7
        JMS DOUBLE      / Call subroutine to double it
        DCA RESULT      / Store result
        HLT

ARG,    DEC 7
RESULT, 0

DOUBLE, 0               / Entry point
        DCA TEMP        / Store argument
        TAD TEMP        / Load it back
        TAD TEMP        / Add it again
        JMP I DOUBLE    / Return to caller

TEMP,   0
