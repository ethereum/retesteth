<html><body>
<style>
    p {margin-bottom:1px;}
    h1 {margin-bottom:1px; text-shadow: 1px 1px 2px green, 0 0 25px lime, 0 0 5px lime;}
    a:link { color: yellow; background-color: transparent; text-decoration: none;}
    a:visited { color: yellow; background-color: transparent; text-decoration: none;}
    a:hover { color: red; background-color: transparent; text-decoration: underline;}
    .failed { color:#ff3333; }
    .passed { color:#1aff1a; }
    .submitbtn { font-weight: bold; }
    label.constrlabel { color:white; }
    details > summary {
      color:yellow;
    }
    pre.bash {
      background-color: #262626; 
      color: #D3D3D3;
      font-size: medium ; 
      font-family: Consolas,Monaco,Lucida Console,Liberation Mono,DejaVu Sans Mono,Bitstream Vera Sans Mono,Courier New, monospace;
      width: 100%;
      display: inline-block;
    }
</style>

<script src="singletests.js"></script>
<script>
    function copytext() {
        var copyText = document.getElementById("generatedfile");
        var textArea = document.createElement("textarea");
        textArea.value = copyText.textContent;
        document.body.appendChild(textArea);
        textArea.select();
        document.execCommand("Copy");
        textArea.remove(); 
    }
    
    function sortSelect(selElem) {
        var tmpAry = new Array();
        for (var i=0;i<selElem.options.length;i++) {
            tmpAry[i] = new Array();
            tmpAry[i][0] = selElem.options[i].text;
            tmpAry[i][1] = selElem.options[i].value;
            tmpAry[i][2] = selElem.options[i].selected;
        }
        tmpAry.sort();
        while (selElem.options.length > 0) {
            selElem.options[0] = null;
        }
        for (var i=0;i<tmpAry.length;i++) {
            var op = new Option(tmpAry[i][0], tmpAry[i][1]);
            if (tmpAry[i][2] == true)
            	op.selected = true;
            selElem.options[i] = op;
        }
        return;
    }
    
    function bodyload()
    {
    		testinit();
    		sortSelect(document.requesthelper.gtests);
    		
    		return;
    }
</script>

<body style="font-size:25px" bgcolor="#1a1a1a" text="#00ff00" onload="bodyload();">
<div align="center">
    <h1>Retesteth WEB</h1>
    <a href="https://github.com/ethereum/retesteth">https://github.com/ethereum/retesteth</a>, <a href="https://github.com/ethereum/retesteth/releases"> ==>Releases</a></br>
    <a href="http://retesteth.ethdevops.io/release/0.2.2-merge/dretesteth-0.2.2-merge.tar">dretesteth-0.2.2-merge.tar (1.6 GB)</a> Docker available! (<a href="https://ethereum-tests.readthedocs.io/en/latest/retesteth-tutorial.html">docs</a>) </br>
    <a href="http://retesteth.ethdevops.io/dretesteth.tar">dretesteth.tar (1.6 GB)</a> Docker nightly build from develop branch.
</div>
<hr>

<a href="https://ethereum-tests.readthedocs.io/en/latest/retesteth-ref.html">>>Retesteth cmd reference</a>
<div style="font-size:15px; color:white;">
    -t GeneralStateTests/stExample -- --clients t8ntool --vmtraceraw </br>
    -t GeneralStateTests/stInitCodeTest -- --clients t8ntool --singletest ReturnTest --vmtrace </br>
</div>

<!-- REQUEST HELPER -->
</br>

<details open>
    <summary>Request helper:</summary>
    <form action="index.php" name="requesthelper">
      <label for="gtests" class="constrlabel">./retesteth -t GeneralStateTests/</label>
      <select name="gtests" id="gtests" onchange="singletests()">
         <option selected="selected" value="stExample">stExample</option>
      </select>
      <label for="singletest" class="constrlabel">-- --singletest</label>
      <select name="singletest" id="singletest">
          <option value="stZeroKnowledge2">stZeroKnowledge2</option>
          <option value="stZeroKnowledge1">stZeroKnowledge1</option>
      </select>
      <label for="clients" class="constrlabel">--clients</label>
      <select name="clients" id="clients" onchange=""> 
         <option value="t8ntool">t8ntool</option>
         <option value="besu">besu</option>
         <option value="ethereumjs">ethereumjs</option>
      </select>
      <label for="snetwork" class="constrlabel">--singlenet</label>
      <select name="snetwork" id="snetwork">
         <option selected="selected" value="Merge">Merge</option>
         <option value="London">London</option>
         <option value="Berlin">Berlin</option>
         <option value="Istanbul">Istanbul</option>
      </select>
      <label for="vmtrace" class="constrlabel"> --vmtrace </label>
      <select name="vmtrace" id="vmtrace">
         <option value="none">none</option>
         <option value="raw">raw</option>
         <option value="vmtrace">vmtrace</option>
      </select>
      <label for="verbosity" class="constrlabel"> --verbosity </label>
      <select name="verbosity" id="verbosity">
         <option value="none">none</option>
         <option value="2">2</option>
         <option value="3">3</option>
         <option value="4">4</option>
         <option value="5">5</option>
         <option value="6">6</option>
         <option value="STATS">STATS</option>
         <option value="RPC">RPC</option>
         <option value="TESTLOG">TESTLOG</option>
         <option value="LOWLOG">LOWLOG</option>
         <option value="SOCKET">SOCKET</option>
         <option value="STATE">STATE</option>
      </select>
      <input type="submit" value="Submit" class="submitbtn">      
</br>
      <label for="trd" class="constrlabel" style="position:relative; left:620px;"> -d </label>
      <input type="text" name="trd" style="width:150px; position:relative; left:620px;">
      <label for="trg" class="constrlabel" style="position:relative; left:620px;"> -g </label>
      <input type="text" name="trg" style="width:50px; position:relative; left:620px;">
      <label for="trv" class="constrlabel" style="position:relative; left:620px;"> -v </label>
      <input type="text" name="trv" style="width:50px; position:relative; left:620px;">
    </form>
</details>

<!-- REQUEST SINGLETEST -->
</br>
<details open>
    <summary>Request single file:</summary>
    <form action="index.php" name="requestsingle" method="post" enctype="multipart/form-data">
        <label for="singlet" class="constrlabel">./retesteth -t </label>
        <select name="singlet" id="singlet">
            <option value="GeneralStateTests">GeneralStateTests</option>
            <option value="BlockchainTests">BlockchainTests</option>
        </select>
        <label for="testToUpload" class="constrlabel"> -- --testfile </label>
        <input type="file" name="testToUpload" id="testToUpload">

        <label for="clients" class="constrlabel">--clients</label>
        <select name="clients" id="clients" onchange=""> 
            <option value="t8ntool">t8ntool</option>
            <option value="besu">besu</option>
            <option value="ethereumjs">ethereumjs</option>
        </select>

        <label for="vmtrace" class="constrlabel"> --vmtrace </label>
        <select name="vmtrace" id="vmtrace">
             <option value="none">none</option>
             <option value="raw">raw</option>
             <option value="vmtrace">vmtrace</option>
        </select>

        <label for="filltests" class="constrlabel"> --filltests </label>
        <select name="filltests" id="filltests">
             <option value="none">none</option>
             <option value="filltests">filltests</option>
        </select>

        <input type="submit" class="submitbtn">
    </form>
</details>

<!-- MAIN REQUEST SECTION -->
</br>
<details open>
<summary>Custom command:</summary>
<div style="font-size:25px">
<form action="index.php" method="post">
./retesteth <input type="text" name="command" value="--help" style="width:300px;">
<input type="submit" class="submitbtn">
</form>
</details>
<!-- gtests=stArgsZeroOneBalance&clients=t8ntool&vmtrace=none -->

<div style="color:#D3D3D3;">
<?php

putenv("ETHEREUMJS_PATH=/data/ethereumjs-monorepo");

//phpinfo();
//ini_set('display_errors', 1);
//ini_set('display_startup_errors', 1);
//error_reporting(E_ALL);
$errors = array();

function bashColorToHtml($string) {
    $colors = [
    '[0m' => '</span>',
    '[33m' => '<span style="color:#e6e600">',
    '[43m' => '',
    '[30m' => '<span style="font-weight:bold">',
    '[1;31;49m' => '<span style="color:#e60000">',
    '[0;31m' => '<span style="color:#e60000">',
    '[1;32;49m' => '<span style="color:#00ff00">',
    '[0;39;49m' => '</span>'
    ];

    return str_replace(array_keys($colors), $colors, $string);
    //return preg_replace(array_keys($colors), $colors, $string);
}

$target_file="";
$retesteth_option="";
if ($_POST["command"])
{
    if (strlen($_POST["command"]) < 100)
    {
        $retesteth_option = $_POST["command"];
        $retesteth_option = str_replace("--filltests", "", $retesteth_option);
    }
    else
        $errors[] = "POST `command` is too long!";
} 
else if ($_GET["gtests"]) 
{
    $gtest = $_GET["gtests"];
    $clients = $_GET["clients"];
    $vmtrace = "--".$_GET["vmtrace"];
    if ($_GET["vmtrace"] == "none")
        $vmtrace = "";
    if ($_GET["vmtrace"] == "raw")
        $vmtrace = "--vmtraceraw";
    $verbosity = "--verbosity ".$_GET["verbosity"];
    if ($_GET["verbosity"] == "none")
        $verbosity = "";
    $singletest = "";
    if ($_GET["singletest"])
        $singletest = "--singletest ".$_GET["singletest"];
    $singlenet = "";
    if ($_GET["snetwork"])
        $singlenet = "--singlenet ".$_GET["snetwork"];
    $trd = "";
    $trg = "";
    $trv = "";
    if ($_GET["trd"])
        $trd = "-d ".$_GET["trd"];
    if ($_GET["trg"])
        $trd = "-g ".$_GET["trg"];
    if ($_GET["trv"])
        $trd = "-v ".$_GET["trv"];

    $retesteth_option = "-t GeneralStateTests/$gtest -- --clients $clients $singlenet $vmtrace $verbosity  $singletest $trd $trg $trv";
    $retesteth_option = str_replace("--filltests", "", $retesteth_option);
}
else if ($_POST["singlet"]) 
{
    $suite = $_POST["singlet"];
    $clients = $_POST["clients"];

    $vmtrace = "--".$_POST["vmtrace"];
    if ($_POST["vmtrace"] == "none")
        $vmtrace = "";
    if ($_POST["vmtrace"] == "raw")
        $vmtrace = "--vmtraceraw";

    $filltests = "--".$_POST["filltests"];
    if ($_POST["filltests"] == "none")
        $filltests = "";
    if ($_POST["filltests"] == "filltests")
        $filltests = "--filltests";

    $uploadOk = 1;
    $target_dir = "/data/web/web/uploads/";
    $target_file = $target_dir . basename($_FILES["testToUpload"]["name"]);
    
    if (file_exists($target_file)) {
        unlink($target_file);        
    }

    if ($_FILES["testToUpload"]["size"] > 1048510 * 20) {
          $errors[] = "Sorry, your file is too large.";
          $uploadOk = 0;
    }
    if ($uploadOk)
    {
        //ini_set('display_errors', 1);
        //error_reporting(E_ALL);
        if (move_uploaded_file($_FILES["testToUpload"]["tmp_name"], $target_file))
            echo "The file ". htmlspecialchars( basename( $_FILES["testToUpload"]["name"])). " has been uploaded.</br>";
        else
            $errors[] = "Sorry, there was an error uploading your file.</br>";
    }

    $retesteth_option = "-t $suite -- --clients $clients $vmtrace $filltests --testfile $target_file";
}

if (count($errors) > 0)
{
    echo "<div style='color:red;'>Errors detected: ";
    foreach($errors as $e)
        echo "</br> * ".$e;
    echo "</div>";
}

if ($retesteth_option)
{
    $prearg = "export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64";
    echo "<hr>";
    if (strpos($retesteth_option, "-- ") !== false)
        $addtestpath = " --testpath /data/tests --lowcpu";
    else
        $addtestpath = " -- --testpath /data/tests --lowcpu";
    $arg = escapeshellcmd("/data/retesteth/build/retesteth/retesteth " . $retesteth_option . $addtestpath);
    $retesteth_option = str_replace("/data/web/web/uploads/", "", $retesteth_option);
    echo "./retesteth ". $retesteth_option;
    $output = bashColorToHtml(shell_exec($arg. " 2>&1"));
    echo "<pre class='bash'><span style='color:#D3D3D3'><span style='color:#D3D3D3'>".$output."</span></pre>";

    if (!empty($target_file))
    {
        $filled_file =  str_replace("Filler.", ".", $target_file);
        $handle = fopen($filled_file, "r");
        $contents = fread($handle, filesize($filled_file));
        fclose($handle);
        $contents = str_replace("/data/web/web/uploads/", "", $contents);
        echo "Executed test: <button onclick=\"copytext()\">Copy text</button>";
        echo "<pre class='bash'><span style='color:#D3D3D3'><span style='color:#D3D3D3' id='generatedfile'>".$contents."</span></pre>";
        unlink($filled_file);
    }
}

?>
</div>

</div><hr>
</body></html>
