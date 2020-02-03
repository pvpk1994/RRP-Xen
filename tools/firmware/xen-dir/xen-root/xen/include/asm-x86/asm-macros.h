#if 0
.if 0
#endif
asm ( ".include \"/root/xen/tools/firmware/xen-dir/xen-root/xen/include/asm-x86/asm-macros.h\"" );
#if 0
.endif
.macro altinstruction_entry orig repl feature orig_len repl_len pad_len
    .long \orig - .
    .long \repl - .
    .word \feature
    .byte \orig_len
    .byte \repl_len
    .byte \pad_len
    .byte 0
.endm
.macro mknops nr_bytes
    .skip \nr_bytes, 0x90
.endm
.macro ALTERNATIVE oldinstr, newinstr, feature
    .L\@_orig_s: \oldinstr; .L\@_orig_e: .L\@_diff = (.L\@_repl_e\()1 - .L\@_repl_s\()1) - (.L\@_orig_e - .L\@_orig_s); mknops ((-(.L\@_diff > 0)) * .L\@_diff); .L\@_orig_p:
    .pushsection .altinstructions, "a", @progbits
    altinstruction_entry .L\@_orig_s, .L\@_repl_s1, \feature, (.L\@_orig_e - .L\@_orig_s), (.L\@_repl_e\()1 - .L\@_repl_s\()1), (.L\@_orig_p - .L\@_orig_e)
    .section .discard, "a", @progbits
    .byte (.L\@_orig_p - .L\@_orig_s)
    .byte 0xff + (.L\@_repl_e\()1 - .L\@_repl_s\()1) - (.L\@_orig_p - .L\@_orig_s)
    .section .altinstr_replacement, "ax", @progbits
    .L\@_repl_s\()1: \newinstr; .L\@_repl_e\()1:
    .popsection
.endm
.macro ALTERNATIVE_2 oldinstr, newinstr1, feature1, newinstr2, feature2
    .L\@_orig_s: \oldinstr; .L\@_orig_e: .L\@_diff = (((.L\@_repl_e\()1 - .L\@_repl_s\()1)) ^ ((((.L\@_repl_e\()1 - .L\@_repl_s\()1)) ^ ((.L\@_repl_e\()2 - .L\@_repl_s\()2))) & -(-(((.L\@_repl_e\()1 - .L\@_repl_s\()1)) < ((.L\@_repl_e\()2 - .L\@_repl_s\()2)))))) - (.L\@_orig_e - .L\@_orig_s); mknops ((-(.L\@_diff > 0)) * .L\@_diff); .L\@_orig_p:
    .pushsection .altinstructions, "a", @progbits
    altinstruction_entry .L\@_orig_s, .L\@_repl_s1, \feature1, (.L\@_orig_e - .L\@_orig_s), (.L\@_repl_e\()1 - .L\@_repl_s\()1), (.L\@_orig_p - .L\@_orig_e)
    altinstruction_entry .L\@_orig_s, .L\@_repl_s2, \feature2, (.L\@_orig_e - .L\@_orig_s), (.L\@_repl_e\()2 - .L\@_repl_s\()2), (.L\@_orig_p - .L\@_orig_e)
    .section .discard, "a", @progbits
    .byte (.L\@_orig_p - .L\@_orig_s)
    .byte 0xff + (.L\@_repl_e\()1 - .L\@_repl_s\()1) - (.L\@_orig_p - .L\@_orig_s)
    .byte 0xff + (.L\@_repl_e\()2 - .L\@_repl_s\()2) - (.L\@_orig_p - .L\@_orig_s)
    .section .altinstr_replacement, "ax", @progbits
    .L\@_repl_s\()1: \newinstr1; .L\@_repl_e\()1:
    .L\@_repl_s\()2: \newinstr2; .L\@_repl_e\()2:
    .popsection
.endm
#endif
