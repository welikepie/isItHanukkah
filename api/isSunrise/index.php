<?php
$arr=$_GET;
date_default_timezone_set("UTC");
$latitude=51.5072;
$longitude=0.1275;
$responseArr = array();

	if(key_exists("lat",$arr)&&key_exists("lon",$arr)) {
			$latitude=$arr["lat"];
			$longitude=$arr["lon"];
		}
			//if the functions don't seem to be working at all / spitting out the wrong times, try comparing the time echo'ed below to the sunset time listed in the foreach statement.
			//if they're vaguely similar then yeah, stuff's working fine.
			//echo date("D M d Y"). ', sunset time : ' .date_sunset(time(), SUNFUNCS_RET_STRING, $latitude, $longitude ,90, 1);
		$info=date_sun_info(time(),$latitude,$longitude);
		if ($info["sunrise"] == 0 && $info["sunset"] == 0 || $info["sunrise"] == 1 && $info["sunset"] == 1) {
		$responseArr["civil_sunrise"] = (bool)$info["sunset"];
		$responseArr["nautical_sunrise"] = (bool)$info["sunset"];
		$responseArr["astronomical_sunrise"] = (bool)$info["sunset"];
			
		}
		
		else{
		$responseArr["civil_sunrise"] = (bool)($info["civil_twilight_begin"]<=time());
		$responseArr["nautical_sunrise"] = (bool)($info["nautical_twilight_begin"]<=time());
		$responseArr["astronomical_sunrise"] = (bool)($info["astronomical_twilight_begin"]<=time());
		}
	printOut();
function printOut() {
		global $responseArr;
		$data = json_encode($responseArr);
		if(array_key_exists('callback', $_GET)){
	
	    header('Content-Type: text/javascript; charset=utf8');
	    header('Access-Control-Allow-Origin: http://www.example.com/');
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