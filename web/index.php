<html><body>
<style>
    p {margin-bottom:1px;}
    h1 {margin-bottom:1px; text-shadow: 1px 1px 2px green, 0 0 25px lime, 0 0 5px lime;}
    a:link { color: yellow; background-color: transparent; text-decoration: none;}
    a:visited { color: yellow; background-color: transparent; text-decoration: none;}
    a:hover { color: red; background-color: transparent; text-decoration: underline;}
    .failed { color:#ff3333; }
    .passed { color:#1aff1a; }
</style>
<body style="font-size:25px" bgcolor="#1a1a1a" text="#00ff00">
<div align="center">
    <h1>Retesteth stats</h1>
    <a href="./web">==> online interface</a></br>
    <a href="https://github.com/ethereum/retesteth">https://github.com/ethereum/retesteth</a>, <a href="https://github.com/ethereum/retesteth/releases"> ==>Releases</a></br>
    <a href="http://retesteth.ethdevops.io/release/0.2.2-merge/dretesteth-0.2.2-merge.tar">dretesteth-0.2.2-merge.tar (1.6 GB)</a> Docker available! (<a href="https://ethereum-tests.readthedocs.io/en/latest/retesteth-tutorial.html">docs</a>) </br>
    <a href="http://retesteth.ethdevops.io/dretesteth.tar">dretesteth.tar (1.6 GB)</a> Docker nightly build from develop branch.
</div>
<hr>

<div style="font-size:25px">
<?php
    $dir = "/data/web";
    $files = scandir($dir."/results", SCANDIR_SORT_DESCENDING);
    $page = max(1,intval(htmlspecialchars($_GET["page"])));
    $perpage = 40;

    $i = 0; $a = 0;
    $iStart = $perpage * ($page-1);
    $iEnd = $perpage * $page;

	 $defaultColor = "#b3ffff"; 
	 $bundleHTML = "";
	 $bundleName = "";
	 $bundleDate = "";
    foreach($files as $file)
    {
        $fpath = $dir."/results/".$file;
        if (is_file($fpath))
        {
            if ($i >= $iStart && $i < $iEnd)
            {
                $a++;
                $origColor = $defaultColor;
                $origBName = $bundleName;
                $origBDate = $bundleDate; 
                $contents = file_get_contents($fpath);
                $bundleDate = date('Y-m-d', filemtime($fpath)); 

                if (strstr($contents, "ethereumjs -t"))
                {
                    $defaultColor = "#ffeecc";
                    $bundleName = "<font style='color:$defaultColor'>ethjs</font>";
                }
                elseif (strstr($contents, "besu"))
                {
                    $defaultColor = "#ffccd5";
                    $bundleName = "<font style='color:$defaultColor'>besu</font>";
                }
                elseif (strstr($contents, "geth -t"))
                {
                	  $defaultColor = "#b3ffff";
                	  $bundleName = "<font style='color:$defaultColor'>geth</font>";
                }
                elseif (strstr($contents, "t8ntool -t"))
                {
                	  $defaultColor = "#93ffff";
                	  $bundleName = "<font style='color:$defaultColor'>geth-t8ntool</font>";
                }
                elseif (strstr($contents, "oewrap -t"))
                {
                	  $defaultColor = "#ff66cc";
                	  $bundleName = "<font style='color:$defaultColor'>oewrap</font>";
                }
                elseif (strstr($contents, "aleth -t"))
                {
                    $defaultColor = "#ffe680";
                    $bundleName = "<font style='color:$defaultColor'>aleth</font>";
                }
                if (strstr($contents, "testeth -t"))
                {
                    $defaultColor = "#efe680";
                    $bundleName = "<font style='color:$defaultColor'>testeth</font>";
                }
                if (strstr($contents, "retesteth -t"))
                {
                    $defaultColor = "#99ff99";
                    $bundleName = "<font style='color:$defaultColor'>retesteth</font>";
                }
                
                if ($defaultColor == $origColor)
                {
                	 $bundleHTML .= "<p style='color:$defaultColor'>".$contents."</p>";
                     $mempng = "/results/log/".pathinfo($file)['filename'].".png";
                     if (file_exists($dir.$mempng))
                         $bundleHTML .= "<img src='$mempng'>";
                }
                else 
                {
                	 if (strlen($bundleHTML) > 1)
                	 {
                   	     echo "<details>";
                   	     $failcount = substr_count($bundleHTML, "failed");
                   	     $passcount = substr_count($bundleHTML, "passed");
                   	     if ($failcount > 0)
                   	         $failpass = "$failcount / <font class='passed'>$passcount</font>";
                   	     if (strstr($bundleHTML, "failed"))
                            echo "<summary class='failed'>Failed  $failpass $origBName - $origBDate</summary>";
                        else
                            echo "<summary class='passed'>Passed  $passcount $origBName - $origBDate</summary>";
                        echo $bundleHTML;
                        echo "</p></details>";
			            if (!($origBDate == $bundleDate))
			        	    echo "<hr>";
				    }

                    // Initial record 
                    $bundleHTML = "";
                    $bundleHTML .= "<p style='color:$defaultColor'>".$contents."</p>";
                    $mempng = "/results/log/".pathinfo($file)['filename'].".png";
                    if (file_exists($dir.$mempng))
                        $bundleHTML .= "<img src='$mempng'>";
                }
            }
            $i+=1;
        }
    }

    if ($page > 1)
        $prevpage = $page - 1;
    else $prevpage = $page;
    if ($a == $perpage)
        $nextpage = $page + 1;
    else $nextpage = $page;

    while(++$a <= $perpage)
        echo "<p style='visibility:hidden'>Failed 2020-03-18 geth -t GeneralStateTests -- --all -j2 (Test: #73cf1e2f, Geth: #8d7aa907, Retesteth: #7aac168b) Log: (2020-03-18-1584496822.txt)</p>";

    echo "<hr><div align='center'><p><a href='./index.php?page=$prevpage'>&lt;&lt;Previous</a>   <a href='./index.php?page=$nextpage'>Next>></a></p></div>";
?>
</div><hr>
</body></html>
