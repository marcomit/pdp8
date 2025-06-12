        ORG 200
        CLA
        TAD MAX         / Load 4095 (07777)
        TAD ONE         / Add 1 (wraps to 0000, sets Link)
        DCA SUM         / Store result (should be 0)
        HLT

MAX,    OCT 7777
ONE,    DEC 1
SUM,    0
