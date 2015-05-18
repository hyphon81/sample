        .global _hangup
_hangup:
        // IRQ,FIQ disable,Thumb disable, SVC mode
        mov r0,#0x000000d3
        msr cpsr,r0
        bl hangup
        b .

        .global _IRQ_interrupt
_IRQ_interrupt:
        // スタックにレジスタの内容を退避させる
        stmfd r13!,{r0-r12,lr}
        // C言語の割込みハンドラ呼び出し
        bl IRQ_handler
        // スタックからレジスタの内容を復元する
        ldmfd r13!,{r0-r12,lr}
        // 戻りアドレス修正(IRQはlr-4)して、元の実行番地に復帰
        subs pc,lr,#4
        /*
        sub lr, lr, #4
        srsdb sp!,#31   ; LR_irq と SPSR_irq をシステムモードスタックに保存
        cps #031 ; システムモードに切り替え
        push {r0-r3,r12} ; 他の AAPCS レジスタを保存
        and r1, sp, #4
        sub sp, sp, r1
        push {r1, lr}
        bl identify_and_clear_source
        cpsie i ; IRQ を有効にする
        bl C_irq_handler
        cpsid i ; IRQ を無効にする
        pop {r1,lr}
        add sp, sp, R1
        pop {r0-r3, r12} ; レジスタを復元
        rfeia sp! ; システムモードスタックから RFE を使用して復帰
        */

// IRQ有効化(CPSR設定)
        .global enable_IRQ
enable_IRQ:
        mrs r0,cpsr
        bic r0,r0,#0x80
        msr cpsr_c,r0
        bx lr

// IRQ無効化(CPSR設定)
        .global disable_IRQ
disable_IRQ:
        mrs r0,cpsr
        ldr r1,=0x80
        orr r0,r0,r1
        msr cpsr_c,r0
        bx lr

// CPSRの値を取得して返す
        .global getmode
getmode:
        mrs r0,cpsr
        bx lr

// ベクタテーブル設定用
        .global _initial_vector_start
_initial_vector_start:
        ldr pc,_vec_Reset
        ldr pc,_vec_Undef
        ldr pc,_vec_SWI
        ldr pc,_vec_PrefAbort
        ldr pc,_vec_DataAbort
        ldr pc,_vec_Reserved
        ldr pc,_vec_IRQ
        ldr pc,_vec_FIQ
_vec_Reset:     .word _start
_vec_Undef:     .word _hangup
_vec_SWI:       .word _hangup
_vec_PrefAbort: .word _hangup
_vec_DataAbort: .word _hangup
_vec_Reserved:  .word _hangup
_vec_IRQ:       .word _IRQ_interrupt
_vec_FIQ:       .word _hangup
        .global _initial_vector_end
_initial_vector_end:
