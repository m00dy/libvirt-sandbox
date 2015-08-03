/*
 * libvirt-sandbox-config-env.c: libvirt sandbox configuration
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

#include "libvirt-sandbox/libvirt-sandbox-config-all.h"

/**
 * SECTION: libvirt-sandbox-config-env
 * @short_description: Disk attachment configuration details
 * @include: libvirt-sandbox/libvirt-sandbox.h
 * @see_aloso: #GVirSandboxConfig
 *
 * Provides an object to store information about a environment variable in the sandbox
 *
 */

#define GVIR_SANDBOX_CONFIG_ENV_GET_PRIVATE(obj)                      \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_SANDBOX_TYPE_CONFIG_ENV, GVirSandboxConfigEnvPrivate))


struct _GVirSandboxConfigEnvPrivate
{
    gchar *key;
    gchar *value;
};

G_DEFINE_TYPE(GVirSandboxConfigEnv, gvir_sandbox_config_env, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_KEY,
    PROP_VALUE
};

enum {
    LAST_SIGNAL
};



static void gvir_sandbox_config_env_get_property(GObject *object,
                                                  guint prop_id,
                                                  GValue *value,
                                                  GParamSpec *pspec)
{
    GVirSandboxConfigEnv *config = GVIR_SANDBOX_CONFIG_ENV(object);
    GVirSandboxConfigEnvPrivate *priv = config->priv;

    switch (prop_id) {
    case PROP_KEY:
        g_value_set_string(value, priv->key);
        break;
    case PROP_VALUE:
        g_value_set_string(value, priv->value);
        break;
   default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_sandbox_config_env_set_property(GObject *object,
                                                  guint prop_id,
                                                  const GValue *value,
                                                  GParamSpec *pspec)
{
    GVirSandboxConfigEnv *config = GVIR_SANDBOX_CONFIG_ENV(object);
    GVirSandboxConfigEnvPrivate *priv = config->priv;

    switch (prop_id) {
    case PROP_KEY:
        g_free(priv->key);
        priv->key = g_value_dup_string(value);
        break;
    case PROP_VALUE:
        g_free(priv->value);
        priv->value = g_value_dup_string(value);
        break;
   default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_sandbox_config_env_finalize(GObject *object)
{
    GVirSandboxConfigEnv *config = GVIR_SANDBOX_CONFIG_ENV(object);
    GVirSandboxConfigEnvPrivate *priv = config->priv;

    g_free(priv->key);
    g_free(priv->value);

    G_OBJECT_CLASS(gvir_sandbox_config_env_parent_class)->finalize(object);
}


static void gvir_sandbox_config_env_class_init(GVirSandboxConfigEnvClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = gvir_sandbox_config_env_finalize;
    object_class->get_property = gvir_sandbox_config_env_get_property;
    object_class->set_property = gvir_sandbox_config_env_set_property;

    g_object_class_install_property(object_class,
                                    PROP_KEY,
                                    g_param_spec_string("key",
                                                        "Key",
                                                        "The sandbox key property",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));

    g_object_class_install_property(object_class,
                                    PROP_VALUE,
                                    g_param_spec_string("value",
                                                        "Value",
                                                        "The sandbox value property",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirSandboxConfigEnvPrivate));
}


static void gvir_sandbox_config_env_init(GVirSandboxConfigEnv *config)
{
    config->priv = GVIR_SANDBOX_CONFIG_ENV_GET_PRIVATE(config);
}


/**
 * gvir_sandbox_config_env_get_key:
 * @config: (transfer none): the sandbox env config
 *
 * Retrieves the key property for the environment variable
 *
 * Returns: (transfer none): the key property
 */
const gchar *gvir_sandbox_config_env_get_key(GVirSandboxConfigEnv *config)
{
    GVirSandboxConfigEnvPrivate *priv = config->priv;
    return priv->key;
}


/**
 * gvir_sandbox_config_disk_get_value:
 * @config: (transfer none): the sandbox env config
 *
 * Retrieves the value property for the environment variable
 *
 * Returns: (transfer none): the value property
 */
const gchar *gvir_sandbox_config_env_get_value(GVirSandboxConfigEnv *config)
{
    GVirSandboxConfigEnvPrivate *priv = config->priv;
    return priv->value;
}

/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  indent-tabs-mode: nil
 *  tab-width: 8
 * End:
 */
