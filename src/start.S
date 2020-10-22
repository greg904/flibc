.text
.extern sys_main_amd64

.global _start
_start:
	xorl %ebp, %ebp /* This is the outermost stack frame. */
	movq %rsp, %rdi /* Put argv in first argument. */
	call sys_main_amd64

/* Tell the linker that we do not require an executable stack. */
.section .note.GNU-stack,"",@progbits
