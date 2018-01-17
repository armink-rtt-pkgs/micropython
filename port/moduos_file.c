/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 SummerGift <zhangyuan@rt-thread.com>
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

#include <stdint.h>
#include <string.h>

#include "py/runtime.h"
#include "py/objstr.h"
#include "py/mperrno.h"
#include "moduos_file.h"

#if MICROPY_MODUOS_FILE

mp_obj_t mp_posix_mount(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_posix_mount_obj, 2, mp_posix_mount);

mp_obj_t mp_posix_umount(mp_obj_t mnt_in) {

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_posix_umount_obj, mp_posix_umount);

mp_obj_t mp_posix_chdir(mp_obj_t path_in) {
    char *changepath = mp_obj_str_get_str(path_in);
    if (chdir(changepath) != 0) {
        rt_kprintf("No such directory: %s\n", changepath);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_posix_chdir_obj, mp_posix_chdir);

mp_obj_t mp_posix_getcwd(void) {
    char buf[MICROPY_ALLOC_PATH_MAX + 1];
    getcwd(buf, sizeof(buf));
    return mp_obj_new_str(buf, strlen(buf), false);
}
MP_DEFINE_CONST_FUN_OBJ_0(mp_posix_getcwd_obj, mp_posix_getcwd);

mp_obj_t mp_posix_listdir(size_t n_args, const mp_obj_t *args) {
    extern void ls(const char *pathname);
    if (n_args == 0) {
#ifdef DFS_USING_WORKDIR
        extern char working_directory[];
        ls(working_directory);
#else
        ls("/");
#endif
    } else {
        ls(mp_obj_str_get_str(args[0]));
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp_posix_listdir_obj, 0, 1, mp_posix_listdir);

mp_obj_t mp_posix_mkdir(mp_obj_t path_in) {
    char *createpath = mp_obj_str_get_str(path_in);
    int res = mkdir(createpath, 0);
    if (res != 0) {
        mp_raise_OSError(MP_EEXIST);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_posix_mkdir_obj, mp_posix_mkdir);

mp_obj_t mp_posix_remove(uint n_args, const mp_obj_t *arg) {
    int index;
    if (n_args == 0) {
        rt_kprintf("Usage: rm FILE...\n");
        rt_kprintf("Remove (unlink) the FILE(s).\n");
        return mp_const_none;
    }
    for (index = 0; index < n_args; index++) {
        //rt_kprintf("Remove %s.\n", mp_obj_str_get_str(arg[index]));
        rm(mp_obj_str_get_str(arg[index]));
    }
    // TODO
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR(mp_posix_remove_obj, 0, mp_posix_remove);

mp_obj_t mp_posix_rename(mp_obj_t old_path_in, mp_obj_t new_path_in) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(mp_posix_rename_obj, mp_posix_rename);

mp_obj_t mp_posix_rmdir(uint n_args, const mp_obj_t *arg) {
    int index;
    if (n_args == 0) {
        rt_kprintf("Usage: rm FILE...\n");
        rt_kprintf("Remove (unlink) the FILE(s).\n");
        return mp_const_none;
    }
    for (index = 0; index < n_args; index++) {
        //rt_kprintf("Remove %s.\n", mp_obj_str_get_str(arg[index]));
        rmdir(mp_obj_str_get_str(arg[index]));
    }
    // TODO
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR(mp_posix_rmdir_obj, 0, mp_posix_rmdir);

mp_obj_t mp_posix_stat(mp_obj_t path_in) {
    struct stat buf;
    char *createpath = mp_obj_str_get_str(path_in);
    int res = stat(createpath, &buf);
    if (res != 0) {
        mp_raise_OSError(MP_EPERM);
    }
    mp_obj_tuple_t *t = MP_OBJ_TO_PTR(mp_obj_new_tuple(10, NULL));
    t->items[0] = MP_OBJ_NEW_SMALL_INT(buf.st_mode); // st_mode
    t->items[1] = MP_OBJ_NEW_SMALL_INT(buf.st_ino); // st_ino
    t->items[2] = MP_OBJ_NEW_SMALL_INT(buf.st_dev); // st_dev
    t->items[3] = MP_OBJ_NEW_SMALL_INT(buf.st_nlink); // st_nlink
    t->items[4] = MP_OBJ_NEW_SMALL_INT(buf.st_uid); // st_uid
    t->items[5] = MP_OBJ_NEW_SMALL_INT(buf.st_gid); // st_gid
    t->items[6] = mp_obj_new_int_from_uint(buf.st_size); // st_size
    t->items[7] = MP_OBJ_NEW_SMALL_INT(buf.st_atime); // st_atime
    t->items[8] = MP_OBJ_NEW_SMALL_INT(buf.st_mtime); // st_mtime
    t->items[9] = MP_OBJ_NEW_SMALL_INT(buf.st_ctime); // st_ctime
    return MP_OBJ_FROM_PTR(t);
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_posix_stat_obj, mp_posix_stat);


#endif //MICROPY_MODUOS_FILE
