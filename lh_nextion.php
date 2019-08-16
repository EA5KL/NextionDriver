<?php
include_once $_SERVER['DOCUMENT_ROOT'].'/config/config.php';          // MMDVMDash Config
include_once $_SERVER['DOCUMENT_ROOT'].'/mmdvmhost/tools.php';        // MMDVMDash Tools
include_once $_SERVER['DOCUMENT_ROOT'].'/mmdvmhost/functions.php';    // MMDVMDash Functions

$i = 0;
for ($i = 0;  ($i <= 14); $i++) { //Last 15 calls
        if (isset($lastHeard[$i])) {
                $listElem = $lastHeard[$i];
                echo substr(str_pad($listElem[1], 12), 0, 12)."|";
                echo substr(str_pad($listElem[2], 10), 0, 12)."|";
                echo substr(str_pad($listElem[4], 9), 0 , 9)."|";

                if ($listElem[5] == "RF") {
                        echo "RF |";
                } else {
                        echo str_pad($listElem[5], 3)."|";
                }

                if ($listElem[6] == null) {
                        echo "TX   |";
                } else if ($listElem[6] == "SMS") {
                        echo "SMS  |";
                } else {
                        echo str_pad($listElem[6], 5, " ", STR_PAD_LEFT)."|";
                }

                echo str_pad($listElem[7], 3," ", STR_PAD_LEFT)."|";
                echo str_pad($listElem[8], 4," ", STR_PAD_LEFT)."|";
                echo"</br>\n";
        }
}
?>
