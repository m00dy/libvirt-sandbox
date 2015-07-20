#!/usr/bin/python -Es
# -*- coding: utf-8 -*-
# Authors: Daniel P. Berrange <berrange@redhat.com>
#          Eren Yagdiran <erenyagdiran@gmail.com>
#
# Copyright (C) 2013 Red Hat, Inc.
# Copyright (C) 2015 Universitat Politècnica de Catalunya.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

import argparse
import gettext
import hashlib
import json
import os
import os.path
import shutil
import sys
import urllib2
import subprocess

template_dir = None
storage_dir = None
default_privileged_template_dir = "/var/lib/libvirt/templates"
default_home_dir = os.environ['HOME']
default_unprivileged_template_dir = default_home_dir + "/.local/share/libvirt/templates"
default_privileged_storage_dir = default_privileged_template_dir + "/storage"
default_unprivileged_storage_dir = default_unprivileged_template_dir + "/storage"
debug = False
verbose = False

def check_dir_writable(path):
    if not os.access(path,os.W_OK):
        return False
    return True

def runtime_dir_resolver():
    global default_privileged_template_dir
    global default_privileged_storage_dir
    global default_unprivileged_template_dir
    global default_unprivileged_storage_dir
    global template_dir
    global storage_dir
    if(check_dir_writable(default_privileged_template_dir)):
        template_dir = default_privileged_template_dir
        storage_dir = default_privileged_storage_dir
        return
    template_dir = default_unprivileged_template_dir
    storage_dir = default_unprivileged_storage_dir
    if not os.path.exists(template_dir):
        os.makedirs(template_dir)
    if not os.path.exists(storage_dir):
        os.makedirs(storage_dir)

sys.dont_write_byte_code = True

import importlib
def dynamic_source_loader(name):
    name = name[0].upper() + name[1:]
    modname = "sources." + name + "Source"
    mod = importlib.import_module(modname)
    classname = name + "Source"
    classimpl = getattr(mod,classname)
    return classimpl()

gettext.bindtextdomain("libvirt-sandbox", "/usr/share/locale")
gettext.textdomain("libvirt-sandbox")
try:
    gettext.install("libvirt-sandbox",
                    localedir="/usr/share/locale",
                    unicode=False,
                    codeset = 'utf-8')
except IOError:
    import __builtin__
    __builtin__.__dict__['_'] = unicode


def debug(msg):
    sys.stderr.write(msg)

def info(msg):
    sys.stdout.write(msg)

def delete_template(name, destdir):
    imageusage = {}
    imageparent = {}
    imagenames = {}
    imagedirs = os.listdir(destdir)
    for imagetagid in imagedirs:
        indexfile = destdir + "/" + imagetagid + "/index.json"
        if os.path.exists(indexfile):
            with open(indexfile, "r") as f:
                index = json.load(f)
            imagenames[index["name"]] = imagetagid
        jsonfile = destdir + "/" + imagetagid + "/template.json"
        if os.path.exists(jsonfile):
            with open(jsonfile, "r") as f:
                template = json.load(f)

            parent = template.get("parent", None)
            if parent:
                if parent not in imageusage:
                    imageusage[parent] = []
                imageusage[parent].append(imagetagid)
                imageparent[imagetagid] = parent

    if not name in imagenames:
        raise ValueError(["Image %s does not exist locally" % name])

    imagetagid = imagenames[name]
    while imagetagid != None:
        debug("Remove %s\n" %  imagetagid)
        parent = imageparent.get(imagetagid, None)

        indexfile = destdir + "/" + imagetagid + "/index.json"
        if os.path.exists(indexfile):
            os.remove(indexfile)
        jsonfile = destdir + "/" + imagetagid + "/template.json"
        if os.path.exists(jsonfile):
            os.remove(jsonfile)
        datafile = destdir + "/" + imagetagid + "/template.tar.gz"
        if os.path.exists(datafile):
            os.remove(datafile)
        imagedir = destdir + "/" + imagetagid
        os.rmdir(imagedir)

        if parent:
            if len(imageusage[parent]) != 1:
                debug("Parent %s is shared\n" % parent)
                parent = None
        imagetagid = parent

def download(args):
    try:
        dynamic_source_loader(args.source).download_template(name=args.name,
                                                             registry=args.registry,
                                                             username=args.username,
                                                             password=args.password,
                                                             templatedir=args.template_dir)
    except IOError,e:
        print "Source %s cannot be found in given path" %args.source
    except Exception,e:
        print "Download Error %s" % str(e)

def delete(args):
    info("Deleting %s from %s\n" % (args.name, default_template_dir))
    delete_template(args.name, default_template_dir)

def create(args):
    try:
        dynamic_source_loader(args.source).create_template(name=args.name,
                                                           connect=args.connect,
                                                           templatedir=args.template_dir,
                                                           format=args.format)
    except Exception,e:
        print "Create Error %s" % str(e)

def requires_name(parser):
    parser.add_argument("name",
                        help=_("name of the template"))

def requires_source(parser):
    parser.add_argument("-s","--source",
                        default="docker",
                        help=_("name of the template"))

def requires_connect(parser):
    parser.add_argument("-c","--connect",
                        help=_("Connect string for libvirt"))

def requires_auth_conn(parser):
    parser.add_argument("-r","--registry",
                        help=_("Url of the custom registry"))
    parser.add_argument("-u","--username",
                        help=_("Username for the custom registry"))
    parser.add_argument("-p","--password",
                        help=_("Password for the custom registry"))

def requires_template_dir(parser):
    global template_dir
    parser.add_argument("-t","--template-dir",
                        default=template_dir,
                        help=_("Template directory for saving templates"))

def gen_download_args(subparser):
    parser = subparser.add_parser("download",
                                   help=_("Download template data"))
    requires_source(parser)
    requires_name(parser)
    requires_auth_conn(parser)
    requires_template_dir(parser)
    parser.set_defaults(func=download)

def gen_delete_args(subparser):
    parser = subparser.add_parser("delete",
                                   help=_("Delete template data"))
    requires_name(parser)
    parser.set_defaults(func=delete)

def gen_create_args(subparser):
    parser = subparser.add_parser("create",
                                   help=_("Create image from template data"))
    requires_name(parser)
    requires_source(parser)
    requires_connect(parser)
    requires_template_dir(parser)
    parser.add_argument("-f","--format",
                        default="qcow2",
                        help=_("format format for image"))
    parser.set_defaults(func=create)

if __name__ == '__main__':
    runtime_dir_resolver()
    parser = argparse.ArgumentParser(description='Sandbox Container Image Tool')
    subparser = parser.add_subparsers(help=_("commands"))
    gen_download_args(subparser)
    gen_delete_args(subparser)
    gen_create_args(subparser)

    try:
        args = parser.parse_args()
        args.func(args)
        sys.exit(0)
    except KeyboardInterrupt, e:
        sys.exit(0)
    except ValueError, e:
        for line in e:
            for l in line:
                sys.stderr.write("%s: %s\n" % (sys.argv[0], l))
        sys.stderr.flush()
        sys.exit(1)
    except IOError, e:
        sys.stderr.write("%s: %s: %s\n" % (sys.argv[0], e.filename, e.reason))
        sys.stderr.flush()
        sys.exit(1)
    except OSError, e:
        sys.stderr.write("%s: %s\n" % (sys.argv[0], e))
        sys.stderr.flush()
        sys.exit(1)
