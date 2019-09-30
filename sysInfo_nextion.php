<?php 
include_once $_SERVER['DOCUMENT_ROOT'].'/config/ircddblocal.php';
include_once $_SERVER['DOCUMENT_ROOT'].'/config/config.php';          // MMDVMDash Config
include_once $_SERVER['DOCUMENT_ROOT'].'/mmdvmhost/tools.php';        // MMDVMDash Tools
include_once $_SERVER['DOCUMENT_ROOT'].'/mmdvmhost/functions.php';    // MMDVMDash Functions

$configs = array();
if ($configfile = fopen($gatewayConfigPath,'r')) {
        while ($line = fgets($configfile)) {
                list($key,$value) = preg_split('/=/',$line);
                $value = trim(str_replace('"','',$value));
                if ($key != 'ircddbPassword' && strlen($value) > 0)
                $configs[$key] = $value;
        }
}
$cpuLoad = sys_getloadavg();
$cpuTempCRaw = exec('cat /sys/class/thermal/thermal_zone0/temp');
if ($cpuTempCRaw > 1000) { $cpuTempC = round($cpuTempCRaw / 1000, 1); } else { $cpuTempC = round($cpuTempCRaw, 1); }
$cpuTempF = round(+$cpuTempC * 9 / 5 + 32, 1);
$cpuTemp = $cpuTempC."|".$cpuTempF;
$hostName = exec('hostname -I');
echo substr($hostName, 0, strpos($hostName, " "))."|".substr(exec('uptime -p'),3)."|".php_uname('n')."|".php_uname('r')."|";
echo exec('platformDetect.sh')."|".$cpuLoad[0]."/".$cpuLoad[1]."/".$cpuLoad[2]."|".$cpuTemp."|";
echo getMHZ(getConfigItem("Info", "TXFrequency", $mmdvmconfigs))."|".getMHZ(getConfigItem("Info", "RXFrequency", $mmdvmconfigs))."|";
if (getDVModemFirmware()) echo getDVModemFirmware()."|";
if (getDVModemTCXOFreq()) echo getDVModemTCXOFreq()."|";
// TRX Status code
if (isset($lastHeard[0])) {
	$listElem = $lastHeard[0];
	if ( $listElem[2] && $listElem[6] == null && $listElem[5] !== 'RF') {
	        echo "TX ".$listElem[1]."|63878|0|";
	        }
	        else {
	        if (getActualMode($lastHeard, $mmdvmconfigs) === 'idle') {
	                echo "Listening|1472|384|";
	                }
	        elseif (getActualMode($lastHeard, $mmdvmconfigs) === NULL) {
	                if (isProcessRunning("MMDVMHost")) echo "Listening|1472|384|"; else echo "OFFLINE|25356|46486|";
		        }
	        elseif ($listElem[2] && $listElem[6] == null && getActualMode($lastHeard, $mmdvmconfigs) === 'D-Star') {
	                echo "RX D-Star|19724|0|";
	                }
	        elseif (getActualMode($lastHeard, $mmdvmconfigs) === 'D-Star') {
	                echo "Listen D-Star|44797|0|";
	                }
	        elseif ($listElem[2] && $listElem[6] == null && getActualMode($lastHeard, $mmdvmconfigs) === 'DMR') {
	                echo "RX DMR|19724|0|";
	                }
	        elseif (getActualMode($lastHeard, $mmdvmconfigs) === 'DMR') {
	                echo "Listen DMR|64710|0|";
	                }
	        elseif ($listElem[2] && $listElem[6] == null && getActualMode($lastHeard, $mmdvmconfigs) === 'YSF') {
	                echo "RX YSF|19724|0|";
	                }
	        elseif (getActualMode($lastHeard, $mmdvmconfigs) === 'YSF') {
	                echo "Listen YSF|65523|0|";
	                }
	        elseif ($listElem[2] && $listElem[6] == null && getActualMode($lastHeard, $mmdvmconfigs) === 'P25') {
        	        echo "RX P25|19724|0|";
        	        }
        	elseif (getActualMode($lastHeard, $mmdvmconfigs) === 'P25') {
        	        echo "Listening P25|64735|0|";
        	        }
		elseif ($listElem[2] && $listElem[6] == null && getActualMode($lastHeard, $mmdvmconfigs) === 'NXDN') {
        	        echo "RX NXDN|19724|0|";
        	        }
        	elseif (getActualMode($lastHeard, $mmdvmconfigs) === 'NXDN') {
        	        echo "Listen NXDN|52447|0|";
        	        }
		elseif (getActualMode($lastHeard, $mmdvmconfigs) === 'POCSAG') {
        	        echo "POCSAG|19724|0|";
        	        }
        	else {
        	        echo getActualMode($lastHeard, $mmdvmconfigs)."|23709|0";
        	        }
		}
	}
else {
	echo "|";
}
?>
