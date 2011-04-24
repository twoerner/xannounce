EXTRA_DIST = xannounce.jpg

MY_SOURCES = authors.sgml compile.sgml changelog.sgml download.sgml news.sgml \
    readme.sgml screenshot.sgml

EXTRA_PROGRAMS = Makefile.p2w changelog.sgml
noinst_PROGRAMS = @CVS@ @P2W@
Makefile_p2w_SOURCES = xannounce.p2w $(MY_SOURCES)
changelog_sgml_SOURCES =

if USEPROJ2WEB
Makefile.p2w: xannounce.p2w $(MY_SOURCES)
	@PROJ2WEB@ xannounce.p2w
	$(MAKE) -f Makefile.p2w
endif

if CVSDIREXISTS
if USECVSLOG2SGML
changelog.sgml: $(top_srcdir)/CVS/Entries $(top_srcdir)/code/CVS/Entries $(top_srcdir)/doc/CVS/Entries
	cd $(top_srcdir); cvs log > doc/log.txt
	@CVSLOG2SGML@ /home/trevor/Documentation/HTMLpublic/templates/changelog.tmpl < log.txt > changelog.sgml
	$(RM) log.txt
endif
endif
