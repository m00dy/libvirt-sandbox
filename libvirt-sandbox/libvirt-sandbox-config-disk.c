/*
 * libvirt-sandbox-config-disk.c: libvirt sandbox configuration
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

/**
 * SECTION: libvirt-sandbox-config-disk
 * @short_description: Disk attachment configuration details
 * @include: libvirt-sandbox/libvirt-sandbox.h
 * @see_aloso: #GVirSandboxConfig
 *
 * Provides an object to store information about a disk attachment in the sandbox
 *
 */

#define GVIR_SANDBOX_CONFIG_DISK_GET_PRIVATE(obj)                      \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_SANDBOX_TYPE_CONFIG_DISK, GVirSandboxConfigDiskPrivate))

struct _GVirSandboxConfigDiskPrivate
{
    GVirConfigDomainDisk *diskType;
    gchar *target;
    gchar *source;
    GVirConfigDomainDiskDriver *diskDriver;
};

G_DEFINE_TYPE(GVirSandboxConfigDisk, gvir_sandbox_config_disk, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_DISKTYPE,
    PROP_TARGET,
    PROP_SOURCE,
    PROP_DISKDRIVER
};

enum {
    LAST_SIGNAL
};



static void gvir_sandbox_config_disk_get_property(GObject *object,
                                                  guint prop_id,
                                                  GValue *value,
                                                  GParamSpec *pspec)
{
    GVirSandboxConfigDisk *config = GVIR_SANDBOX_CONFIG_DISK(object);
    GVirSandboxConfigDiskPrivate *priv = config->priv;

    switch (prop_id) {
    case PROP_TARGET:
        g_value_set_string(value, priv->target);
        break;
    case PROP_SOURCE:
        g_value_set_string(value, priv->source);
        break;
    case PROP_DISKTYPE:
        g_value_set_object(value, priv->diskType);
        break;
    case PROP_DISKDRIVER:
        g_value_set_object(value, priv->diskDriver);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_sandbox_config_disk_set_property(GObject *object,
                                                  guint prop_id,
                                                  const GValue *value,
                                                  GParamSpec *pspec)
{
    GVirSandboxConfigDisk *config = GVIR_SANDBOX_CONFIG_DISK(object);
    GVirSandboxConfigDiskPrivate *priv = config->priv;

    switch (prop_id) {
    case PROP_TARGET:
        g_free(priv->target);
        priv->target = g_value_dup_string(value);
        break;
    case PROP_SOURCE:
        g_free(priv->source);
        priv->source = g_value_dup_string(value);
        break;
    case PROP_DISKTYPE:
        g_free(priv->diskType);
        priv->diskType = g_value_dup_object(value);
        break;
    case PROP_DISKDRIVER:
        g_free(priv->diskDriver);
        priv->diskDriver = g_value_dup_object(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_sandbox_config_disk_finalize(GObject *object)
{
    GVirSandboxConfigDisk *config = GVIR_SANDBOX_CONFIG_DISK(object);
    GVirSandboxConfigDiskPrivate *priv = config->priv;

    g_free(priv->target);
    g_free(priv->source);

    //g_free(priv->diskType);
    //g_free(priv->diskDriver);

    G_OBJECT_CLASS(gvir_sandbox_config_disk_parent_class)->finalize(object);
}


static void gvir_sandbox_config_disk_class_init(GVirSandboxConfigDiskClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = gvir_sandbox_config_disk_finalize;
    object_class->get_property = gvir_sandbox_config_disk_get_property;
    object_class->set_property = gvir_sandbox_config_disk_set_property;

    g_object_class_install_property(object_class,
                                    PROP_TARGET,
                                    g_param_spec_string("target",
                                                        "Target",
                                                        "The sandbox target directory",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));

    g_object_class_install_property(object_class,
                                    PROP_SOURCE,
                                    g_param_spec_string("source",
                                                        "Source",
                                                        "The sandbox source directory",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));


    g_object_class_install_property(object_class,
                                    PROP_DISKTYPE,
                                    g_param_spec_object("disktype",
                                                      "Disktype",
                                                      "The sandbox disk type",
                                                      GVIR_CONFIG_TYPE_DOMAIN_DEVICE,
                                                      G_PARAM_READABLE |
                                                      G_PARAM_WRITABLE |
                                                      G_PARAM_CONSTRUCT_ONLY |
                                                      G_PARAM_STATIC_NAME |
                                                      G_PARAM_STATIC_NICK |
                                                      G_PARAM_STATIC_BLURB));

    g_object_class_install_property(object_class,
                                    PROP_DISKDRIVER,
                                    g_param_spec_object("diskdriver",
                                                        "Diskdriver",
                                                        "The sandbox disk driver",
                                                        GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirSandboxConfigDiskPrivate));
}


static void gvir_sandbox_config_disk_init(GVirSandboxConfigDisk *config)
{
    config->priv = GVIR_SANDBOX_CONFIG_DISK_GET_PRIVATE(config);
}

/**
 * gvir_sandbox_config_disk_get_disktype:
 * @config: (transfer none): the sandbox disk config
 *
 * Retrieves the type property for the custom disk
 *
 * Returns: (transfer none): the type property of disk
 */
GVirConfigDomainDiskType gvir_sandbox_config_disk_get_disktype(GVirSandboxConfigDisk *config)
{
    GVirSandboxConfigDiskPrivate *priv = config->priv;
    return gvir_config_domain_disk_get_disk_type(priv->diskType);
}


/**
 * gvir_sandbox_config_disk_get_format:
 * @config: (transfer none): the sandbox disk config
 *
 * Retrieves the format property for the custom disk
 *
 * Returns: (transfer none): the format property of disk
 */
GVirConfigDomainDiskFormat gvir_sandbox_config_disk_get_diskformat(GVirSandboxConfigDisk *config)
{
    GVirSandboxConfigDiskPrivate *priv = config->priv;
    return gvir_config_domain_disk_driver_get_format(priv->diskDriver);
}


/**
 * gvir_sandbox_config_disk_get_target:
 * @config: (transfer none): the sandbox disk config
 *
 * Retrieves the target property for the custom disk
 *
 * Returns: (transfer none): the target property path
 */
const gchar *gvir_sandbox_config_disk_get_target(GVirSandboxConfigDisk *config)
{
    GVirSandboxConfigDiskPrivate *priv = config->priv;
    return priv->target;
}


/**
 * gvir_sandbox_config_disk_get_source:
 * @config: (transfer none): the sandbox disk config
 *
 * Retrieves the source property for the custom disk
 *
 * Returns: (transfer none): the source property
 */
const gchar *gvir_sandbox_config_disk_get_source(GVirSandboxConfigDisk *config)
{
    GVirSandboxConfigDiskPrivate *priv = config->priv;
    return priv->source;
}


/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  indent-tabs-mode: nil
 *  tab-width: 8
 * End:
 */
