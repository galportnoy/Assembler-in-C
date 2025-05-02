char *COMMANDS[16] = {"mov", "cmp", "add" 
                    , "sub", "lea", "clr"
                    , "not", "inc", "dec"
                    , "jmp", "bne","jsr" 
                    ,"red", "prn", "rts"
                    , "stop"};

char *GUIDANCE[4] = {".data", ".string"
                   ,".extern",".entry"};

char *REGISTER[16] = {"r0","r1","r2","r3"
                    ,"r4","r5","r6","r7"
                    ,"r8","r9","r10","r11"
                    ,"r12","r13","r14","r15"};

char *REGISTER_ADDRESS[16] = {"[r0]","[r1]","[r2]","[r3]"
                            ,"[r4]","[r5]","[r6]","[r7]"
                            ,"[r8]","[r9]","[r10]","[r11]"
                            ,"[r12]","[r13]","[r14]","[r15]"};

char *funct2[2] = {"add" , "sub"};

char *funct5[4] = {"clr", "not", "inc", "dec"};

char *funct9[3] = { "jmp", "bne","jsr"};
