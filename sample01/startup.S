@ startup
        .global _start
        .align
_start:
        // IRQモードのスタックポインタ設定
        // IRQ,FIQ disable, Thumb disable, IRQ mode
        mov r0,#0x000000d2
        msr cpsr,r0
        mov sp,#0x8000

        // (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
        mov r0,#0x000000d1
        msr cpsr_c,r0
        mov sp,#0x4000

        // SVCモードのスタックポインタ設定
        // IRQ,FIQ disable, Thumb disable, SVC mode
        mov r0,#0x000000d3
        msr cpsr,r0
        mov sp,#0x6400000

        // SVCモードのスタックポインタ設定
        // IRQ enable ,FIQ disable, Thumb disable, SVC mode
        //mov r0,#0x00000053
        //msr cpsr,r0

        // main 呼び出し
        bl main

        // 終了
        b .
