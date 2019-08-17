<?php
include_once $_SERVER['DOCUMENT_ROOT'].'/config/config.php';          // MMDVMDash Config
include_once $_SERVER['DOCUMENT_ROOT'].'/mmdvmhost/tools.php';        // MMDVMDash Tools
include_once $_SERVER['DOCUMENT_ROOT'].'/mmdvmhost/functions.php';    // MMDVMDash Functions

$i = 0;
$LHlist = "";
for ($i = 0;  ($i <= 14); $i++) { // Last 15 calls
        if (isset($lastHeard[$i])) {
                $listElem = $lastHeard[$i];
                $LHlist .= "LHt".strval($i+1).'.txt="';
                $LHlist .= substr(str_pad($listElem[1], 11), 0, 11)."|";
                $LHlist .= substr(str_pad($listElem[2], 11), 0, 11)."|";
                $LHlist .= substr(str_pad($listElem[4], 9), 0 , 9)."|";

                if ($listElem[5] == "RF") {
                        $LHlist .= "RF |";
                } else {
                        $LHlist .= str_pad($listElem[5], 3)."|";
                }

                if ($listElem[6] == null) {
                        $LHlist .= "TX   |";
                } else if ($listElem[6] == "SMS") {
                        $LHlist .= "SMS  |";
                } else {
                        $LHlist .= str_pad($listElem[6], 5, " ", STR_PAD_LEFT)."|";
                }

                $LHlist .= str_pad($listElem[7], 3," ", STR_PAD_LEFT)."|";
                $LHlist .= str_pad($listElem[8], 4," ", STR_PAD_LEFT)."|";
                $LHlist .= '"';
        }
}
echo $LHlist;
?>
