/*
 * libvirt-sandbox-config-env.h: libvirt sandbox configuration
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

#if !defined(__LIBVIRT_SANDBOX_H__) && !defined(LIBVIRT_SANDBOX_BUILD)
#error "Only <libvirt-sandbox/libvirt-sandbox.h> can be included directly."
#endif

#ifndef __LIBVIRT_SANDBOX_CONFIG_ENV_H__
#define __LIBVIRT_SANDBOX_CONFIG_ENV_H__

G_BEGIN_DECLS

#define GVIR_SANDBOX_TYPE_CONFIG_ENV             (gvir_sandbox_config_env_get_type ())
#define GVIR_SANDBOX_CONFIG_ENV(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_SANDBOX_TYPE_CONFIG_ENV, GVirSandboxConfigEnv))
#define GVIR_SANDBOX_CONFIG_ENV_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_SANDBOX_TYPE_CONFIG_ENV, GVirSandboxConfigEnvClass))
#define GVIR_SANDBOX_IS_CONFIG_ENV(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_SANDBOX_TYPE_CONFIG_ENV))
#define GVIR_SANDBOX_IS_CONFIG_ENV_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_SANDBOX_TYPE_CONFIG_ENV))
#define GVIR_SANDBOX_CONFIG_ENV_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_SANDBOX_TYPE_CONFIG_ENV, GVirSandboxConfigEnvClass))

#define GVIR_SANDBOX_TYPE_CONFIG_ENV_HANDLE      (gvir_sandbox_config_env_handle_get_type ())

typedef struct _GVirSandboxConfigEnv GVirSandboxConfigEnv;
typedef struct _GVirSandboxConfigEnvPrivate GVirSandboxConfigEnvPrivate;
typedef struct _GVirSandboxConfigEnvClass GVirSandboxConfigEnvClass;

struct _GVirSandboxConfigEnv
{
    GObject parent;

    GVirSandboxConfigEnvPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirSandboxConfigEnvClass
{
    GObjectClass parent_class;

    gpointer padding[LIBVIRT_SANDBOX_CLASS_PADDING];
};

GType gvir_sandbox_config_env_get_type(void);

const gchar *gvir_sandbox_config_env_get_key(GVirSandboxConfigEnv *config);

const gchar *gvir_sandbox_config_env_get_value(GVirSandboxConfigEnv *config);

G_END_DECLS

#endif /* __LIBVIRT_SANDBOX_CONFIG_DISK_H__ */

/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  indent-tabs-mode: nil
 *  tab-width: 8
 * End:
 */
