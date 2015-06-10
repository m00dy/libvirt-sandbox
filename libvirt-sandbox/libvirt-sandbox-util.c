/*
 * libvirt-sandbox-util.c: libvirt sandbox util functions
 *
 * Copyright (C) 2015 Universitat Politècnica de Catalunya.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Eren Yagdiran <erenyagdiran@gmail.com>
 */

#include <config.h>
#include <string.h>

#include "libvirt-sandbox/libvirt-sandbox.h"

/* This array contains string values for GVirConfigDomainDiskFormat,
 * order is important.*/
static const gchar *FORMATS_STRINGS[] = {
    "raw",
    "dir",
    "bochs",
    "cloop",
    "cow",
    "dmg",
    "iso",
    "qcow",
    "qcow2",
    "qed",
    "vmdk",
    "vpc",
    "fat",
    "vhd",
    NULL
};

gint gvir_sandbox_util_guess_image_format(const gchar *path){

    gchar *tmp;

    if ((tmp = strchr(path, '.')) == NULL) {
        return -1;
    }
    tmp = tmp + 1;
    
    if (strcmp(tmp,"img")==0){
       return GVIR_CONFIG_DOMAIN_DISK_FORMAT_RAW;
    }
    
    return gvir_sandbox_util_disk_format_from_str(tmp);
}

gint gvir_sandbox_util_disk_format_from_str(const gchar *value)
{
    gint i = 0;

    while (FORMATS_STRINGS[i] != NULL) {
       	if (strcmp(FORMATS_STRINGS[i], value) == 0)
            return i;
        i++;
    }
    return -1;
}

const gchar *gvir_sandbox_util_disk_format_to_str(GVirConfigDomainDiskFormat format)
{
    return FORMATS_STRINGS[format];
}
