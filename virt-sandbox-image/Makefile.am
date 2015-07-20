
EXTRA_DIST = \
	virt-sandbox-image.py \
	sources

install-data-local:
	$(mkinstalldirs) $(DESTDIR)/$(pkgpythondir)/sources
	$(INSTALL) -m 0755 $(srcdir)/virt-sandbox-image.py $(DESTDIR)$(pkgpythondir)
	$(INSTALL) -m 0644 $(srcdir)/sources/__init__.py $(DESTDIR)$(pkgpythondir)/sources
	$(INSTALL) -m 0644 $(srcdir)/sources/Source.py $(DESTDIR)$(pkgpythondir)/sources
	$(INSTALL) -m 0644 $(srcdir)/sources/DockerSource.py $(DESTDIR)$(pkgpythondir)/sources

uninstall-local:
	rm -f $(DESTDIR)$(pkgpythondir)
