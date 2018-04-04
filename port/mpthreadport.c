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

#include "stdio.h"

#include "py/mpconfig.h"
#include "py/mpstate.h"
#include "py/gc.h"
#include "py/mpthread.h"
#include "mpthreadport.h"

#if MICROPY_PY_THREAD

#define MP_THREAD_MIN_STACK_SIZE                        (4 * 1024)
#define MP_THREAD_DEFAULT_STACK_SIZE                    (MP_THREAD_MIN_STACK_SIZE + 1024)
#define MP_THREAD_PRIORITY                              (RT_THREAD_PRIORITY_MAX / 2)

// this structure forms a linked list, one node per active thread
typedef struct _thread_t {
    rt_thread_t id;         // system id of thread
    int ready;              // whether the thread is ready and running
    void *arg;              // thread Python args, a GC root pointer
    struct _thread_t *next;
} thread_t;

// the mutex controls access to the linked list
STATIC mp_thread_mutex_t thread_mutex;
STATIC thread_t thread_entry0;
STATIC thread_t *thread; // root pointer, handled by mp_thread_gc_others

void mp_thread_init(void) {
    mp_thread_set_state(&mp_state_ctx.thread);
    // create the first entry in the linked list of all threads
    thread = &thread_entry0;
    thread->id = rt_thread_self();
    thread->ready = 1;
    thread->arg = NULL;
    thread->next = NULL;
    mp_thread_mutex_init(&thread_mutex);
}

void mp_thread_gc_others(void) {
    mp_thread_mutex_lock(&thread_mutex, 1);
    for (thread_t *th = thread; th != NULL; th = th->next) {
        gc_collect_root((void**)&th, 1);
        gc_collect_root(&th->arg, 1); // probably not needed
        if (th->id == rt_thread_self()) {
            continue;
        }
        if (!th->ready) {
            continue;
        }
        rt_thread_delete(th->id);
    }
    mp_thread_mutex_unlock(&thread_mutex);
}

mp_state_thread_t *mp_thread_get_state(void) {
    return (mp_state_thread_t *)(rt_thread_self()->user_data);
}

void mp_thread_set_state(void *state) {
    rt_thread_self()->user_data = (rt_uint32_t)state;
}

void mp_thread_start(void) {
    mp_thread_mutex_lock(&thread_mutex, 1);
    for (thread_t *th = thread; th != NULL; th = th->next) {
        if (th->id == rt_thread_self()) {
            th->ready = 1;
            break;
        }
    }
    mp_thread_mutex_unlock(&thread_mutex);
}

STATIC void *(*ext_thread_entry)(void*) = NULL;

STATIC void rtthread_entry(void *arg) {
    if (ext_thread_entry) {
        ext_thread_entry(arg);
    }
    while(1) {
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
}

void mp_thread_create_ex(void *(*entry)(void*), void *arg, size_t *stack_size, int priority, char *name) {
    // store thread entry function into a global variable so we can access it
    ext_thread_entry = entry;//(void (*)(void *parameter))

    if (*stack_size == 0) {
        *stack_size = MP_THREAD_DEFAULT_STACK_SIZE; // default stack size
    } else if (*stack_size < MP_THREAD_MIN_STACK_SIZE) {
        *stack_size = MP_THREAD_MIN_STACK_SIZE; // minimum stack size
    }

    // allocate linked-list node (must be outside thread_mutex lock)
    thread_t *th = m_new_obj(thread_t);
    if (th == NULL) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "can't create thread obj"));
    }

    mp_thread_mutex_lock(&thread_mutex, 1);

    // create thread
    rt_thread_t id = rt_thread_create(name, rtthread_entry, arg, *stack_size, priority, 0);
    if (id == NULL) {
        mp_thread_mutex_unlock(&thread_mutex);
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "can't create thread"));
    }

    // add thread to linked list of all threads
    th->id = id;
    th->ready = 0;
    th->arg = arg;
    th->next = thread;
    thread = th;

    rt_thread_startup(id);

    mp_thread_mutex_unlock(&thread_mutex);
}

void mp_thread_create(void *(*entry)(void*), void *arg, size_t *stack_size) {
    static uint8_t count = 0;
    char name[RT_NAME_MAX];

    /* build name */
    rt_snprintf(name, sizeof(name), "mp%02d", count ++);

    mp_thread_create_ex(entry, arg, stack_size, MP_THREAD_PRIORITY, name);
}

void mp_thread_finish(void) {
    mp_thread_mutex_lock(&thread_mutex, 1);
    for (thread_t *th = thread; th != NULL; th = th->next) {
        if (th->id == rt_thread_self()) {
            th->ready = 0;
            break;
        }
    }
    mp_thread_mutex_unlock(&thread_mutex);
}

void mp_thread_mutex_init(mp_thread_mutex_t *mutex) {
    static uint8_t count = 0;
    char name[RT_NAME_MAX];

    /* build name */
    rt_snprintf(name, sizeof(name), "mp%02d", count ++);

    /* only init once */
    if (mutex->parent.parent.type != (RT_Object_Class_Mutex | RT_Object_Class_Static)) {
        rt_mutex_init((rt_mutex_t) mutex, name, RT_IPC_FLAG_FIFO);
    }
}

int mp_thread_mutex_lock(mp_thread_mutex_t *mutex, int wait) {
    return (RT_EOK == rt_mutex_take((rt_mutex_t) mutex, wait ? RT_WAITING_FOREVER : 0));
}

void mp_thread_mutex_unlock(mp_thread_mutex_t *mutex) {
    rt_mutex_release((rt_mutex_t) mutex);
}

void mp_thread_deinit(void) {
    mp_thread_mutex_lock(&thread_mutex, 1);
    for (thread_t *th = thread; th != NULL; th = th->next) {
        // don't delete the current task
        if (th->id == rt_thread_self()) {
            continue;
        }
        rt_thread_delete(th->id);
    }
    mp_thread_mutex_unlock(&thread_mutex);
    // allow RT-Thread to clean-up the threads
    rt_thread_delay(200);
}

#endif /* MICROPY_PY_THREAD */
