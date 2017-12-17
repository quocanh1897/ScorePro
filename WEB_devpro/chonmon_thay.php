<?php

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
		<p><input type='submit' class='btn btn-danger' name='OK' value='OK'><font color='black'>   Nhấn Ok để xem điểm!</font>
		</p>  
</li></ul>

</form>
</div>";
$MSSV=$_SESSION['var'];//$y is any declared variable
if(isset($_POST['OK']))
{
	$_SESSION['daxem']=1;
	$mon_hoc = $_POST["monhoc"];
	$_SESSION['chonmon']=$mon_hoc;
	if ($mon_hoc=="khong"){echo "Chưa chọn môn học!";}
	else
	{
		if(is_dir("D:/workingDir/$mon_hoc"))
		{
		$host='localhost'; // Host Name.
	    $db_user='root'; //User Name
	    $db_password= '';
	    $db= 'phpdb'; // Database Name.
	    $conn=mysqli_connect($host,$db_user,$db_password,$db); 
		if(file_exists("D:/workingDir/$mon_hoc/listAllSubSV.csv") &&(file_exists("D:/workingDir/$mon_hoc/listSV.csv")))
		{
		$duongdan_1="D:/workingDir/$mon_hoc/listSV.csv";
		$duongdan_2="D:/workingDir/$mon_hoc/listAllSubSV.csv";
	
	    $filename_1=$duongdan_1;
		//$filename_2="D:/workingDir/KTMT/listAllSubSV.csv";
		$filename_2=$duongdan_2;
	    $file_1 = fopen($filename_1, "r");
		//$sql_data = "SELECT * FROM prod_list_1 ";
		$count_1 = 0;     // add this line
		while (($emapData_1 = fgetcsv($file_1, 10000, ",")) !== FALSE)
		{
			//print_r($emapData);
			//exit();
			$count_1++;// add this line

			if($count_1>1)
			{// add this line
				$sql_1 = "INSERT into diem_1(MSSV,lannop,diem,time) values ('$emapData_1[0]','$emapData_1[1]','$emapData_1[2]','$emapData_1[3]')";
				mysqli_query($conn,$sql_1);
			}// add this line
		}
        
 
        fclose($file_1);
		
		
		
		################
		
		
		
		
		
		
		$file_2 = fopen($filename_2, "r");
		//$sql_data = "SELECT * FROM prod_list_1 ";
		$count_2 = 0;     // add this line
		while (($emapData_2 = fgetcsv($file_2, 10000, ",")) !== FALSE)
		{
			//print_r($emapData);
			//exit();
			$count_2++;// add this line

			if($count_2>1)
			{// add this line
				$sql_2 = "INSERT into diem_2(MSSV,lannop,diem,time) values ('$emapData_2[0]','$emapData_2[1]','$emapData_2[2]','$emapData_2[3]')";
				mysqli_query($conn,$sql_2);
			}// add this line
		}
        
 
        fclose($file_2);

		include "show.php";
		mysqli_query($conn,"DELETE FROM diem_1");
		mysqli_query($conn,"DELETE FROM diem_2");
		}
		else{echo "Đang chấm hoặc chưa có bài nộp!";}
		}
		else{echo "Chưa có bài nộp!";}
	}
	
}

























?>