<?php
$arr=$_GET;
date_default_timezone_set("UTC");
$seconds_in_day=60*60*24;
$dayStart=25;
$monthStart="Kislev";
//$monthStart = "Heshvan";
$responseArr=array();
$latitude=51.5072;
$longitude=0.1275;
$jewishDate;
//DoTheyKnow() //returns base with day of hannukah assuming london,england as a base point. No sunset information.
//DoTheyKnow(lat,lon) //returns sunset information with day of hannukah etc.
//IsSunSet()
//IsSunSet(lat,lon);
if(isHannukah($arr)===true) {
	if($responseArr["isHappening"]===true) {
			//if the functions don't seem to be working at all / spitting out the wrong times, try comparing the time echo'ed below to the sunset time listed in the foreach statement.
			//if they're vaguely similar then yeah, stuff's working fine.
			//echo date("D M d Y"). ', sunset time : ' .date_sunset(time(), SUNFUNCS_RET_STRING, $latitude, $longitude ,90, 1);
		$info=date_sun_info(time(),$latitude,$longitude);
		if ($info["sunrise"] == 0 && $info["sunset"] == 0 || $info["sunrise"] == 1 && $info["sunset"] == 1) {
			$info=date_sun_info(strtotime('today midnight'),$latitude,$longitude);
		}
		if($info["nautical_twilight_end"]<=time() && !key_exists("forceDay",$arr)) {
			$responseArr["dayOf"]=++$responseArr["dayOf"];
		}
/*echo($info["nautical_twilight_end"] .":". time());
 foreach($info as $inf=>$t)
 echo("<br>");
 echo($inf.":".date("H : m : s",$t));
 }*/
		printOut();
//	if(sunset)
	} else {
		printOut();
	}
} else {
		printOut();
}

function isHannukah($arr) {
		global $responseArr;
	if(key_exists("forceDay", $arr)){
		if(intval($arr["forceDay"],10) > 8){
			$arr["forceDay"] = 8;
		}
		$responseArr["dayOf"]=intval($arr["forceDay"],10);
		$responseArr["isHappening"]=true;
		return true;
	}else{
		global $monthStart;
		global $dayStart;
		global $seconds_in_day;
		global $latitude;
		global $longitude;
		global $jewishDate;
		$toAdd = 0;
		if(key_exists("lat",$arr)&&key_exists("lon",$arr)) {
			$latitude=$arr["lat"];
			$longitude=$arr["lon"];
		}
		
		$info=date_sun_info(time(),$latitude,$longitude);
		if ($info["sunrise"] != 0 && $info["sunset"] != 0 && $info["sunrise"] != 1 && $info["sunset"] != 1) {
		$toAdd = (ceil(($info["civil_twilight_end"]/86400))*86400)-$info["civil_twilight_end"];		
		}
//		$toAdd+=3600;
		$jewishDate=cal_from_jd(unixtojd(time()+$toAdd),CAL_JEWISH);
		//var_dump( date("F j, Y, g:i a",time()+$toAdd));
		//var_dump($jewishDate);
		for($i=0;$i<8;$i++) {
			$cals=cal_from_jd(unixtojd((time()+$toAdd)-($seconds_in_day*$i)),CAL_JEWISH);
			if($cals["monthname"]==$monthStart&&$cals["day"]==$dayStart) {
				$responseArr["dayOf"]=$i;
			}
		}
		if(key_exists("dayOf",$responseArr)) {
			$responseArr["isHappening"]=true;
		} else {
			$responseArr["isHappening"]=false;
		}
		return (bool) $responseArr["isHappening"];
	}
}

function printOut() {
		global $responseArr;
		$data = json_encode($responseArr);
		if(array_key_exists('callback', $_GET)){
	
	    header('Content-Type: text/javascript; charset=utf8');
	    header('Access-Control-Allow-Origin: *');
	    header('Access-Control-Max-Age: 3628800');
	    header('Access-Control-Allow-Methods: GET, POST, PUT, DELETE');
	
	    $callback = $_GET['callback'];
	    echo $callback.'('.$data.');';

	}else{
	    // normal JSON string
	    header('Content-Type: application/json; charset=utf8');
	
	    echo $data;
	}
}
?>