<?php
$files[]=NULL;
$sotu=0;
$dem1=0;
$chuoi="";
$arr[]=NULL;
$arr_1[]=NULL;
$thua[]="<option value='"	;
$thua[]="'>";
$thua[]="</option>";
       $file_handle = fopen("D:/ScorePro/settings.config", "r");

             while (!feof($file_handle)) 
	{
             $line_of_text = fgets($file_handle);
			 $dem1=$dem1+1;
			 if($dem1==6)
			 {
				$chuoi=$line_of_text;
				//echo  "$line_of_text"."\n";
				//echo  "$chuoi\n";
			 }
    }
            fclose($file_handle);
	
			$chuoi=explode(' ', $chuoi);
				//echo "$chuoi[0]";
				//echo "chuoi[1]";
				$sotu=count($chuoi);
				//echo "$sotu";
			for ($i = 4; $i < $sotu; $i=$i+2)
			{
				$arr[]=$chuoi[$i];
			}
			$sotu=count($arr);
			//echo "      $sotu";
			for ($i = 1; $i <= $sotu-1; $i++)
			{
				if(!is_dir("D:/workingDir/$arr[$i]"))
				{
					mkdir("D:/workingDir/$arr[$i]");
				}
				
			}
			for ($i = 1; $i <= $sotu-1; $i++)
			{
				$arr_1[]=$thua[0].$arr[$i].$thua[1].$arr[$i].$thua[2];
			}

$quy=implode(' ', $arr_1);
echo "
<div class = 'w3-container' style='margin-left:180px;margin-top:100px;'>
<form action='' enctype='multipart/form-data' method='post'>

	<ul class = 'w3-ul w3-card-4 w3-white'><li class = 'w3-padding-16'>
	<div  class='w3-main' style='margin-left:50px'>
	<select name='monhoc'>
	 <option value='khong'> Chọn môn </option>"
	.$quy.
				
	"
	</select>
	</div >

	    <div class='w3-main' style='margin-left:300px;'>
	      <label for='upload'><font color ='black'>Add các Testcase vào đây:</font></label>
	      <input id='upload' name='upload[]' type='file' multiple='multiple' />
	    </div>
	<br> </br>
		<div style='margin-left:300px;'>
		<p><input type='submit' class='btn btn-danger' name='submit' value='Submit'><font color='black'>   Submit các Testcase!</font>
		</p>  
		</div>
	</li></ul>

</form>
</div>";
echo
"
</div></div>

	<div id = 'sidebarMenu'>
	<nav class='w3-sidebar w3-collapse w3-white w3-animate-left' 
	style='z-index:3;width:300px; margin-left: 0px; margin-top: -270px'><br>
	  <div class='w3-container w3-row'>
	    <div class='w3-col s4'>
	      
	    </div>
	    <div class='w3-col s8 w3-bar'>
	      <span>Welcome, <strong>".    $_SESSION['var']." </strong></span><br>
	      
	    </div>
	  </div>
	  <hr>
	 
	  <div class='w3-bar-block'>
		<a href = 'index.php?act=showmenu'  class='w3-bar-item w3-button w3-padding '><i class='fa fa-male fa-fw'></i>  Tổng quan</a>
		<a href='index.php?act=xemdiem' class='w3-bar-item w3-button w3-padding'><i class='fa fa-eye fa-fw'></i>  Điểm từng môn</a>
    
		
		<a href='index.php?act=upload' class='w3-bar-item w3-button w3-padding w3-green'><i class='fa fa-eye fa-fw'></i> Upload Testcase</a>

	  
	   
	  </div>
	</nav>";

$dem=1;
$y=$_SESSION['var'];//$y is any declared variable
if(isset($_POST['submit']))
{
	$mon_hoc = $_POST["monhoc"];
if ($mon_hoc=="khong"){ echo "<h3><center> <font color=black> Chưa chọn môn học!</font></center></h3>";}
else
{
	
	date_default_timezone_set("Asia/Ho_Chi_Minh");
	$now = date("m/d/H/i/s");//lay thoi gian
	function delete_folder($folder)
	{
    $glob = glob($folder);
    foreach ($glob as $g) 
	{
        if (!is_dir($g))
			{
            unlink($g);
			} else 
			{
            delete_folder("$g/*");
            rmdir($g);
			}
    }
	}
####################
		if(is_dir("uploaded/$y"))
		{
			delete_folder("uploaded/$y");
			//rmdir("uploaded/$y");
			mkdir("uploaded/$y");
		}
		else 
		{
			mkdir("uploaded/$y");
		}
		

	if(count($_FILES['upload']['name']) > 0)
	{
		//Loop through each file
		for($i=0; $i<count($_FILES['upload']['name']); $i++)
		{ 
			
			//Get the temp file path
			$tmpFilePath = $_FILES['upload']['tmp_name'][$i];

			//Make sure we have a filepath
			if($tmpFilePath != "")
			{
				$dem=$dem+1;
				//save the filename
				$shortname = $_FILES['upload']['name'][$i];
	//lay duoi .h .cpp
				$file = $_FILES['upload']['name'][$i];
				$fileExt = explode('.', $file);
				$fileActualExt = strtolower(end($fileExt));
				//save the url and the file
				$filePath = "uploaded/$y/".$_FILES['upload']['name'][$i];
				//Upload the file into the temp dir
				if(move_uploaded_file($tmpFilePath, $filePath)) 
				{
					$files[] = $shortname;
					
					//insert into db 
					//use $shortname for the filename
					//use $filePath for the relative url to the file
				}
			}
		}
	}

//show success message
	$soluong=$dem-1;
if(count($files)==1) {echo "<h3><center> <font color=black> Đã nộp: 0 file testcase</font></center></h3>";  }
else
{
//show success message
	echo "<h3> <center><font color=black> Đã nộp: ".$soluong." files </font></center></h3>";    
	if(is_array($files))
	{
		
		foreach($files as $file)
		{
			echo "<center><font color ='black'>$file</font></center>";
		}
		
	}
	
}
	

	if(is_dir("D:/workingDir/$mon_hoc/testcase"))
	{
		$src = "uploaded/$y";
		$dst = "D:/workingDir/$mon_hoc/testcase";
		$files = glob("uploaded/$y/*");
		foreach($files as $file)
		{
			$file_to_go = str_replace($src,$dst,$file);
			copy($file, $file_to_go);	
		}			
	}
	else
	{
		mkdir("D:/workingDir/$mon_hoc/testcase");
		$src = "uploaded/$y";
		$dst = "D:/workingDir/$mon_hoc/testcase";
		$files = glob("uploaded/$y/*");
		foreach($files as $file)
		{
			$file_to_go = str_replace($src,$dst,$file);
			copy($file, $file_to_go);	
		}			
	}
}
//echo "Chào mừng bạn $y" ;
}

?>





