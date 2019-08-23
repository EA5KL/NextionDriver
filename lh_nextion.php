<?php
include_once $_SERVER['DOCUMENT_ROOT'].'/config/config.php';          // MMDVMDash Config
include_once $_SERVER['DOCUMENT_ROOT'].'/mmdvmhost/tools.php';        // MMDVMDash Tools
include_once $_SERVER['DOCUMENT_ROOT'].'/mmdvmhost/functions.php';    // MMDVMDash Functions
$i = 0;
$LHlistNET = "";
$LHlistLOC = "";
for ($i = 0;  ($i <= 14); $i++) { // Last 15 calls
    if (isset($lastHeard[$i])) {

        // Last heard list Network received
        $listElem = $lastHeard[$i];
        $LHlistNET .= substr(str_pad($listElem[1], 11), 0, 11)."|";
        $LHlistNET .= substr(str_pad($listElem[2], 11), 0, 11)."|";
        $LHlistNET .= substr(str_pad($listElem[4], 9), 0 , 9)."|";
        if ($listElem[5] == "RF") {
            $LHlistNET .= "RF |";
        } else {
            $LHlistNET .= str_pad($listElem[5], 3)."|";
        }
        if ($listElem[6] == null) {
            $LHlistNET .= "TX   |";
        } else if ($listElem[6] == "SMS") {
            $LHlistNET .= "SMS  |";
        } else {
            $LHlistNET .= str_pad($listElem[6], 5, " ", STR_PAD_LEFT)."|";
        }
        $LHlistNET .= str_pad($listElem[7], 3," ", STR_PAD_LEFT)."|";
        $LHlistNET .= str_pad($listElem[8], 4," ", STR_PAD_LEFT);

        // Last heard list RF received
        if ($listElem[5] == "RF" && ($listElem[1] == "D-Star" || startsWith($listElem[1], "DMR") || $listElem[1] == "YSF" || $listElem[1]== "P25" || $listElem[1]== "NXDN")) {
            $LHlistLOC .= substr(str_pad($listElem[1], 11), 0, 11)."|";
            $LHlistLOC .= substr(str_pad($listElem[2], 11), 0, 11)."|";
            $LHlistLOC .= substr(str_pad($listElem[4], 9), 0 , 9)."|";
            if ($listElem[5] == "RF") {
                $LHlistLOC .= "RF |";
            } else {
                $LHlistLOC .= str_pad($listElem[5], 3)."|";
            }
            if ($listElem[6] == null) {
                $LHlistLOC .= "TX   |";
            } else if ($listElem[6] == "SMS") {
                $LHlistLOC .= "SMS  |";
            } else {
                $LHlistLOC .= str_pad($listElem[6], 5, " ", STR_PAD_LEFT)."|";
            }
            $LHlistLOC .= str_pad($listElem[7], 3," ", STR_PAD_LEFT)."|";
            $LHlistLOC .= str_pad($listElem[8], 4," ", STR_PAD_LEFT);
        }
    }
}
if (isset($_GET["from"])) {
   if ($_GET["from"] == "NET") {
      echo $LHlistNET;
   } else if ($_GET["from"] == "LOC") {
      echo $LHlistLOC;
   }
} else {
   echo "There is no last heard data"; // Fallback behaviour goes here
}
?>
