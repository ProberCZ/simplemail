<?php

$version = "0.30";
$date = "2006/12/24";

  require_once("language.inc.php");
  require_once("support.inc.php");


?>

<p>
<?php echo get_string($DownloadsIntroText);?>
</p>

<center>
<table>
<tr bgcolor="black">
<th><font color="white">Archive</font></th>
<th><font color="white">Version</font></th>
<th><font color="white">Date</font></th>
<?php //<th><font color="white">Size</font></th>
?>
</tr>

<tr bgcolor="gray">
<?php
  echo '<td align="center"><A HREF="http://prdownloads.sourceforge.net/simplemail/simplemail-'.$version.'-os3.lha?download">'."simplemail-$version-os3.lha".'</A></td>';
  echo '<td align="center">'.$version.'</td>';
  echo '<td align="center">'.$date.'</td>';
?>
</tr>

<tr bgcolor="gray">
<?php
  echo '<td align="center"><A HREF="http://prdownloads.sourceforge.net/simplemail/simplemail-'.$version.'-os4.lha?download">'."simplemail-$version-os4.lha".'</A></td>';
  echo '<td align="center">'.$version.'</td>';
  echo '<td align="center">'.$date.'</td>';
?>
</tr>

<tr bgcolor="gray">
<?php
  echo '<td align="center"><A HREF="http://prdownloads.sourceforge.net/simplemail/simplemail-'.$version.'-morphos.lha?download">'."simplemail-$version-morphos.lha".'</A></td>';
  echo '<td align="center">'.$version.'</td>';
  echo '<td align="center">'.$date.'</td>';
?>
</tr>

</table>
</center>

<p>
<?php echo get_string($BannersText); ?>
</p>

<table border=0 cellspacing=3 cellpadding=3 align="center" summary="Banner">
<tr>
<td><?php get_img("banner/sm_now1.png");?></td>
<td><?php get_img("banner/sm_now2.png");?></td>
</tr>
<td><?php get_img("banner/sm_now3.png");?></td>
<td><?php get_img("banner/sm_now4.png");?></td>
</table>

<p>
<?php echo get_string($BannersCreatorText); ?>
</p>

<p>
<?php echo get_string($BannersCreator2Text); get_img("banner/sm_now5.png");?>

