<?php
  include "corefunctions.php";
?>

<?php pageHeader("Downloads"); ?>

<body>

<div id="pagewrapper">

	<?php pageMenu("download"); ?>

	<div id="contentwrapper">
		<div id="leftside">
			<h3>Obtain the source code of the PLplot library</h3>
		  <p>You can obtain the latest stable version of PLplot from our
		     <a href="http://sourceforge.net/projects/plplot/files/plplot/">file
		     release site</a>. Alternatively, you can obtain the cutting-edge version of
		     PLplot (but with no promises about stability) from SVN via anonymous
		     access to our <a href="http://svn.code.sf.net/p/plplot/code/trunk/">
		     SVN repository</a>.  Finally, you can browse our SVN repository
		     <a href="http://sourceforge.net/p/plplot/code/HEAD/tree/">
		     here</a>.</p>

			<h3>Subversion Access</h3>
		  <p>The PLplot Subversion repository can be checked out on the command line through svn
		     with the following instruction set:</p>
		  <p><tt>svn checkout http://svn.code.sf.net/p/plplot/code/trunk plplot</tt></p>
		  <p>Here is a partial list of subversion clients</p>
		  <ul class="arrowlist">
		  <li><a href="http://code.google.com/p/svnx/">SvnX (Mac OS X)</a></li>
		  <li><a href="http://tortoisesvn.tigris.org/">TortoiseSVN (Windows)</a></li>
		  <li><a href="http://kdesvn.alwins-world.de/">KDESvn (Linux)</a></li>
		  <li>svn, a powerful command-line subversion client (Linux)</li>
	
		  </ul>
			<h3>PLplot Packages for Linux Distributions</h3>
		  <ul class="arrowlist">
<li><a href="http://packages.ubuntu.com/search?keywords=plplot&amp;searchon=names&amp;suite=all&amp;section=all">Ubuntu</a></li>
<li><a href="http://packages.debian.org/search?searchon=sourcenames&amp;keywords=plplot">Debian</a></li>
<li><a href="http://rpmfind.net/linux/rpm2html/search.php?query=plplot&amp;submit=Search+...">Rpmfind Results</a></li>
<li><a href="http://rpm.pbone.net/">http://rpm.pbone.net/</a> (enter "plplot" search term)</li>
		  </ul>
		</div>

		<?php pageSidebar(1); ?>

		<div id="spacer"></div>
	</div>

	<?php pageFooter(); ?>
</div>

</body>
</html>
