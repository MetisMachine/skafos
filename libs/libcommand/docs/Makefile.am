if HAVE_DOXYGEN

directory = $(top_srcdir)/docs/man/man3

# dist_man_MANS = $(directory)/*.3
# $(directory)/*.3: doxyfile.stamp
#$(directory)/man_page_2.3: doxyfile.stamp

doxyfile.stamp:
	$(DOXYGEN) Doxyfile
	echo Timestamp > doxyfile.stamp

CLEANFILES = doxyfile.stamp

all-local: doxyfile.stamp
clean-local:
	rm  -f $(top_srcdir)/docs/Doxyfile
	rm -rf $(top_srcdir)/docs/man
	rm -rf $(top_srcdir)/docs/html
	rm -rf $(top_srcdir)/docs/latex


endif