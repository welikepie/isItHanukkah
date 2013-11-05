<?php
echo( date("F j, Y, g:i a",time()));
echo("<br>");
$jewishDate=cal_from_jd(unixtojd(),CAL_JEWISH);
foreach($jewishDate as $key=>$value){
	echo($key.":".$value."<br>");
}
echo("<br>"); 
?>