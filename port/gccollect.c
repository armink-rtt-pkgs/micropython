/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Armink (armink.ztl@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>

#include "py/mpstate.h"
#include "py/gc.h"

// If we don't have architecture-specific optimized support,
// just fall back to setjmp-based implementation.

// If MICROPY_GCREGS_SETJMP was requested explicitly, or if
// we enabled it as a fallback above.
#include <setjmp.h>

typedef jmp_buf regs_t;

STATIC void gc_helper_get_regs(regs_t arr) {
    setjmp(arr);
}

void gc_collect_regs_and_stack(void) {
    regs_t regs;
    gc_helper_get_regs(regs);
    // GC stack (and regs because we captured them)
    void **regs_ptr = (void**)(void*)&regs;
    gc_collect_root(regs_ptr, ((uintptr_t)MP_STATE_THREAD(stack_top) - (uintptr_t)&regs) / sizeof(uintptr_t));
}

void gc_collect(void) {
    gc_collect_start();
    // trace root pointers from any threads
    gc_collect_regs_and_stack();

#if MICROPY_PY_THREAD
    //TODO has some problem when multithreading using gc at the same time
    mp_thread_gc_others();
#endif

    gc_collect_end();
    // gc_dump_info();
}
