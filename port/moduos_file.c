/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Damien P. George
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

mp_obj_t mp_vfs_mount(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_vfs_mount_obj, 2, mp_vfs_mount);

mp_obj_t mp_vfs_umount(mp_obj_t mnt_in) {

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_vfs_umount_obj, mp_vfs_umount);

mp_obj_t mp_vfs_chdir(mp_obj_t path_in) {
    char *changepath = mp_obj_str_get_str(path_in);
    if (chdir(changepath) != 0) {
        rt_kprintf("No such directory: %s\n", changepath);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_vfs_chdir_obj, mp_vfs_chdir);

mp_obj_t mp_vfs_getcwd(void) {
    char buf[MICROPY_ALLOC_PATH_MAX + 1];
    getcwd(buf, sizeof(buf));
    return mp_obj_new_str(buf, strlen(buf), false);
}
MP_DEFINE_CONST_FUN_OBJ_0(mp_vfs_getcwd_obj, mp_vfs_getcwd);

mp_obj_t mp_vfs_listdir(size_t n_args, const mp_obj_t *args) {
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
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp_vfs_listdir_obj, 0, 1, mp_vfs_listdir);

mp_obj_t mp_vfs_mkdir(mp_obj_t path_in) {
    char *createpath = mp_obj_str_get_str(path_in);
    int res = mkdir(createpath, 0);
    if (res != 0) {
        mp_raise_OSError(MP_EEXIST);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_vfs_mkdir_obj, mp_vfs_mkdir);

mp_obj_t mp_vfs_remove(uint n_args, const mp_obj_t *arg) {
    int index;
    if (n_args == 0) {
        rt_kprintf("Usage: rm FILE...\n");
        rt_kprintf("Remove (unlink) the FILE(s).\n");
        return mp_const_none;
    }
    for (index = 0; index < n_args; index++) {
        //rt_kprintf("Remove %s.\n", mp_obj_str_get_str(arg[index]));
        unlink(mp_obj_str_get_str(arg[index]));
    }
    // TODO
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR(mp_vfs_remove_obj, 0, mp_vfs_remove);

mp_obj_t mp_vfs_rename(mp_obj_t old_path_in, mp_obj_t new_path_in) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(mp_vfs_rename_obj, mp_vfs_rename);

mp_obj_t mp_vfs_rmdir(uint n_args, const mp_obj_t *arg) {
    int index;
    if (n_args == 0) {
        rt_kprintf("Usage: rm FILE...\n");
        rt_kprintf("Remove (unlink) the FILE(s).\n");
        return mp_const_none;
    }
    for (index = 0; index < n_args; index++) {
        rt_kprintf("Remove %s.\n", mp_obj_str_get_str(arg[index]));
        unlink(mp_obj_str_get_str(arg[index]));
    }
    // TODO
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR(mp_vfs_rmdir_obj, 0, mp_vfs_rmdir);

mp_obj_t mp_vfs_stat(mp_obj_t path_in) {
    mp_obj_t path_out;
    mp_vfs_mount_t *vfs = lookup_path(path_in, &path_out);
    if (vfs == MP_VFS_ROOT) {
        mp_obj_tuple_t *t = MP_OBJ_TO_PTR(mp_obj_new_tuple(10, NULL));
        t->items[0] = MP_OBJ_NEW_SMALL_INT(MP_S_IFDIR); // st_mode
        for (int i = 1; i <= 9; ++i) {
            t->items[i] = MP_OBJ_NEW_SMALL_INT(0); // dev, nlink, uid, gid, size, atime, mtime, ctime
        }
        return MP_OBJ_FROM_PTR(t);
    }
    return mp_vfs_proxy_call(vfs, MP_QSTR_stat, 1, &path_out);
}
MP_DEFINE_CONST_FUN_OBJ_1(mp_vfs_stat_obj, mp_vfs_stat);


#endif // MICROPY_VFS
