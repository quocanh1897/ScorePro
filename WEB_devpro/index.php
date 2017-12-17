<?php
	$connect = mysqli_connect('localhost','root','','khong');
	mysqli_set_charset($connect,"utf8");
	session_start();
	
?>
<!DOCTYPE html>
<html>
<head>
	<title>NopbaiOnline</title>
	<meta charset="utf-8">
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
	<link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Raleway">
	<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
<style>
	html,body,h1,h2,h3,h4,h5 {font-family: "Arial", sans-serif}

	table {
    border-collapse: collapse;
    width: 100%;
	}

	th, td {
		
	    text-align: left;
	    padding: 8px;
	}

	tr:nth-child(even) {background-color: #f2f2f2;}
</style>

</head>
<!--<body BACKGROUND="ab.jpg">-->
<body class="w3-light-grey">


	<!-- 'start thực hiện kiểm tra dữ liệu người dùng nhập ở form đăng nhập' -->
	<?php
	$rows_sv100 = mysqli_query($connect," select * from user where quyen = 'sv'");
	$count_sv100 = mysqli_num_rows($rows_sv100);
	$_SESSION['tonghs']=$count_sv100;
		$_SESSION["daxem"]=0;
		$_SESSION["rediem"]=1;
		if(isset($_POST["dangnhap"]))
		{
			$tk = $_POST["user_name_lg"];
			$_SESSION['var']=$tk;
			$mk =($_POST["passlg"]);
			$rows_sv = mysqli_query($connect," select * from user where user_name = '$tk' and password = '$mk' and quyen = 'sv'");
			$count_sv = mysqli_num_rows($rows_sv);
			$_SESSION['sv']=$count_sv;
			$rows_thay = mysqli_query($connect," select * from user where user_name = '$tk' and password = '$mk' and quyen = 'thay'");
			$count_thay = mysqli_num_rows($rows_thay);
			$_SESSION['thay']=$count_thay;
		
			if($count_sv==1 || $count_thay==1)
			{
				
				$_SESSION["loged"] = true;
				header("location:index.php");
				setcookie("success", "Đăng nhập thành công!", time()+1, "/","", 0);
			}
			else{
				header("location:index.php");
				setcookie("error", "Đăng nhập không thành công!", time()+1, "/","", 0);
			}
			
		}
	?>
	<!-- 'end thực hiện kiểm tra dữ liệu người dùng nhập ở form đăng nhập' -->


	<?php
	//<!-- 'start thực hiện đăng xuất' -->
	
		if(isset($_GET["act"])&&$_GET["act"]=="logout"){
			unset($_SESSION["loged"]);
			unset($_SESSION['sv']);
			unset($_SESSION['thay']);
			header("location:index.php");
			setcookie("success", "Bạn đã đăng xuất! ", time()+1, "/","", 0);
		}
		//<!-- 'start thực hiện xem diem' -->
	?>
	<?php 
			if(isset($_SESSION["loged"])) { ?>
	<div class="container">
		<div class="row">

			<!-- Top container  -->
			<div class="w3-bar w3-top w3-blue w3-large" style="z-index:4;margin-left:-185px">
			<a href = "index.php?act=showmenu" >
			  <button class="w3-bar-item w3-button w3-hover-none w3-hover-text-light-grey">
			  	<i class="fa fa-bars"></i>  
			  	Menu
			  </button>
			</a>
			
	<?php } ?>
			<?php 
			if(isset($_SESSION["loged"])) 
			{
				echo "<a href='index.php?act=logout' >
				<button class='w3-bar-item w3-right'>LOG OUT</button></a>"; 
				echo "<a href='index.php?act=xemdiem' >
				<button class='w3-bar-item w3-button w3-hover-none w3-hover-text-light-black'>Xem Điểm</button></a>"; 
				echo "<a href='index.php?act=upload'>
				<button class='w3-bar-item w3-button'>Upload</button></a>";
				
			}
			?>
			</div>
		</div>

		<div class="row">
			<!-- 'start nếu xảy ra lỗi thì hiện thông báo:' -->
			<?php
				if(isset($_COOKIE["error"])){
			?>
			<div class="alert alert-danger">
			  	<strong>'Có lỗi!'</strong> <?php echo $_COOKIE["error"]; ?>
			</div>
			<?php } ?>
			<!-- 'end nếu xảy ra lỗi thì hiện thông báo:' -->


			<!-- 'start nếu thành công thì hiện thông báo:' -->
			<?php
				if(isset($_COOKIE["success"])){
			?>
			<div class="alert alert-success" style="margin-top: 100px;margin-left: 300px;">
			  	<strong>'Chúc mừng!'</strong> <?php echo $_COOKIE["success"]; ?>
			</div>
			<?php } ?>
			<!-- 'end nếu thành công thì hiện thông báo:' -->


			<?php
			if(!isset($_GET["page"]))
			{
				//nếu tồn tại biến session $_SESSION["loged"] thì gọi nội dung trang upload.php vào
				
				if(isset($_SESSION["loged"]) && isset($_GET["act"]) && $_GET["act"] == "upload")
				{
					if($_SESSION['thay']==1)
					{
						include "upload_thay.php";
					}
					else 
					{
						include "upload.php";

					}
				
				}
				
				//nếu không tồn tại biến session $_SESSION["loged"] thì gọi nội dung trang login.php vào
				else if (!isset($_SESSION["loged"])) 
					include "login.php";
					//include "show.php";
			}
			?>
		</div>

	</div>

	<!-- end thực hiện đăng xuất -->
	<?php 
	if (isset($_GET["bam"]) && $_GET["bam"] == "xemloi")
{
$bocuoc[0]="";
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
<div class = 'w3-container' style='margin-left:310px;margin-top:100px;'>
<form action='' enctype='multipart/form-data' method='post'>

	<ul class = 'w3-ul w3-card-4 w3-white'><li class = 'w3-padding-16'>
	<div  class='w3-main' style='margin-left:50px'>
	<select name='monhoc'>
	 <option value='khong'> Chọn môn </option>"
	.$quy.
				
	"
	</select>
		<p><input type='submit' class='btn btn-danger' name='OK' value='OK'><font color='black'>   Nhấn Ok để xem lỗi!</font>
		</p>  
</li></ul>

</form>
</div>";
	echo
"
</div></div>

	<div id = 'sidebarMenu'>
	<nav class='w3-sidebar w3-collapse w3-white w3-animate-left' 
	style='z-index:3;width:300px; margin-left: 0px; margin-top: -170px'><br>
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
    
		<a href='index.php?bam=xemloi' class='w3-bar-item w3-button w3-padding w3-green'><i class='fa fa-history fa-fw'></i>  Xem lỗi</a>
		<a href='index.php?act=upload' class='w3-bar-item w3-button w3-padding '><i class='fa fa-cloud-upload fa-fw'></i> Upload bài tập </a>
	  </div>
	</nav>";	
	$y=$_SESSION['var'];
$so_sub=0;
	if(isset($_POST['OK']))
	{
	$mon_hoc = $_POST["monhoc"];
	if ($mon_hoc=="khong"){ echo "<h3><center> <font color=black> Chưa chọn môn học!</font></center></h3>";}
		else
		{
			$duongdanmon="D:/workingDir/$mon_hoc/$y";
				if(is_dir($duongdanmon)) 
				{
					//$dem_somon=$dem_somon+1;
					$duongdandiemmon="D:/workingDir/$mon_hoc/$y/scoreOfSV.csv";
					if(file_exists($duongdandiemmon))
					{
						$filename_1=$duongdandiemmon;
						$file_1 = fopen($filename_1, "r");
						$count_1 = 0;     // add this line
						while (($emapData_1 = fgetcsv($file_1, 10000, ",")) !== FALSE)
						{
			
							$count_1++;// add this line

							if($count_1>1)
							{
							$so_sub = $emapData_1[1];
							}
				
						}
						fclose($file_1);
						for($i=1;$i<=$so_sub;$i++)
						{
						$bocuoc[$i]="";
						$sub_2=strval($i);
						$sub="sub".strval($i);
					
						$duongdan_loi="D:/workingDir/$mon_hoc/$y/$sub";
						//echo "$duongdan_loi";
						$filelist = glob("$duongdan_loi/*.log");
							foreach ($filelist as $filename) 
							{
								$file_handle = fopen($filename, "r");
								while (!feof($file_handle)) 
								{
								$line_of_text = fgets($file_handle);
								
								$bocuoc[$i]=$bocuoc[$i]."<br>".$line_of_text;
								
								}
								fclose($file_handle);
							}
							//echo  $bocuoc;
							$sql_100 = "INSERT into diem_1(MSSV,lannop,loi,time) values ($y,$i,'ss','0')";
							mysqli_query($connect,$sql_100);
							$result_1 = mysqli_query($connect,"SELECT * FROM diem_1");
						}	
echo 
"
<div class = 'w3-container' style='margin-left:310px;margin-top:10px;'>


	<ul class = 'w3-ul w3-card-4 w3-white'>	<li class = 'w3-padding-16'>
	<div  class='w3-main' style='margin-left:10px'>
	
	</div >

	    <div class='w3-main' style='margin-left:10px;'>
		<div class = 'w3-container' style='margin-top:0px;'>

	<center> <table border=1 ORDERCOLOR=Red  cellspacing=0 cellpading=0>
	<tr>
	<th height=5 width = 50><center> <font color=black> MSSV </font></center></th>
	<th width = 50><center><font color=black> Lần Nộp</font> </center></th>
	<th width = 250><center><font color=black> Lỗi </font> </center></th>
	
	</tr> </center>


</div>
		
	    </div>
	
	</ul>



<h1>Bảng liệt kê lỗi ".$mon_hoc."</h1>
</div>
";
$hotro=1;
while($row_1 = mysqli_fetch_array($result_1))
{
echo "<tr>";
echo "<td><font color=black><center>" . $row_1['MSSV'] . "</font> </center> </td>";
echo "<td><font color=black><center>" . $row_1['lannop'] . "</font> </center> </td>";
echo "<td><font color=black>" . $bocuoc[$hotro] . "</font></td>";
$hotro=$hotro+1;
//echo "<td><font color=black><center>" . $row_1['time'] . "</font> </center> </td>";
echo "</tr>";
}
echo "</table>";		
						}
						
								//echo $duongdan_loi;
						}
					}
					
				}
	
		
	
}
mysqli_query($connect,"DELETE FROM diem_1");
	?>
<?php 
	if (isset($_GET["act"]) && $_GET["act"] != "upload" && $_GET["act"] != "xemdiem") :
?>

<!-- Sidebar/menu -->
<div id = "sidebarMenu">
<!-- chinh thanh menu o menu -->
<nav class="w3-sidebar w3-collapse w3-white w3-animate-left" style="z-index:3;width:300px; margin-top: 40px;" id="mySidebar"><br>
  <div class="w3-container w3-row">
    <div class="w3-col s4">
      
    </div>
    <div class="w3-col s8 w3-bar">
      <span>Welcome, <strong>  <?php echo $_SESSION['var']; ?></strong></span><br>
      <!--<a href="#" class="w3-bar-item w3-button"><i class="fa fa-envelope"></i></a>
      <a href="#" class="w3-bar-item w3-button"><i class="fa fa-user"></i></a>
      <a href="#" class="w3-bar-item w3-button"><i class="fa fa-cog"></i></a>
  -->
    </div>
  </div>
  <hr>
 
  <div class="w3-bar-block">
  
    <a href = "index.php?act=showmenu"  class="w3-bar-item w3-button w3-padding w3-green"><i class="fa fa-male fa-fw"></i>  Tổng quan</a>
    <a href="index.php?act=xemdiem" class="w3-bar-item w3-button w3-padding"><i class="fa fa-eye fa-fw"></i>  Điểm từng môn</a>
    
   <?php 
   if ($_SESSION['sv']==1)
   {
	   echo"
   <a href='index.php?bam=xemloi' class='w3-bar-item w3-button w3-padding'><i class='fa fa-history fa-fw'></i>  Xem lỗi</a>
    <a href='index.php?act=upload' class='w3-bar-item w3-button w3-padding'><i class='fa fa-cloud-upload fa-fw'></i> Upload bài tập</a><br><br>
	";
   }
   if ($_SESSION['thay']==1)
   {
	   echo"
  
    <a href='index.php?act=upload' class='w3-bar-item w3-button w3-padding'><i class='fa fa-cloud-upload fa-fw'></i> Upload testcase</a><br><br>
	";
   }
	?>
  </div>
</nav>
<?php endif; ?>



<!-- ACTION XEMDIEM -->
<!-- chinh o chon -->

<div class="w3-container"> <div style="z-index:3;width:1000px; margin-left: 300px;">
<?php 
	if (isset($_GET["act"]) && $_GET["act"] == "xemdiem" && $_SESSION['sv']==1):
	{
		//$_SESSION["daxem"]=0;
		include "chonmon.php";
	}
	
?>
<?php
if ($_SESSION['daxem']==1){}
else
{
echo
"
</div></div>

	<div id = 'sidebarMenu'>
	<nav class='w3-sidebar w3-collapse w3-white w3-animate-left' 
	style='z-index:3;width:300px; margin-left: 0px; margin-top: -170px'><br>
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
		<a href='index.php?act=xemdiem' class='w3-bar-item w3-button w3-padding w3-green'><i class='fa fa-eye fa-fw'></i>  Điểm từng môn</a>
    
		<a href='index.php?bam=xemloi' class='w3-bar-item w3-button w3-padding'><i class='fa fa-history fa-fw'></i> Xem lỗi</a>
		<a href='index.php?act=upload' class='w3-bar-item w3-button w3-padding'><i class='fa fa-cloud-upload fa-fw'></i> Upload bài tập</a>
	   
	  </div>
	</nav>";
}
?>
<?php endif; ?>



<?php 
	
	if (isset($_GET["act"]) && $_GET["act"] == "xemdiem" && $_SESSION['thay']==1):
	{
			include "chonmon_thay.php";
	}
?>
<?php
if ($_SESSION['daxem']==1){}
else
{
echo
"
</div></div>

	<div id = 'sidebarMenu'>
	<nav class='w3-sidebar w3-collapse w3-white w3-animate-left' 
	style='z-index:3;width:300px; margin-left: 0px; margin-top: -170px'><br>
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
		<a href='index.php?act=xemdiem' class='w3-bar-item w3-button w3-padding w3-green'><i class='fa fa-eye fa-fw'></i>  Điểm từng môn</a>
    
		
		<a href='index.php?act=upload' class='w3-bar-item w3-button w3-padding'><i class='fa fa-cloud-upload fa-fw'></i> Upload testcase</a>
	  </div>
	</nav>";
}
?>
<?php endif; ?>


<!-- END ACTION XEMDIEM -->



<?php 
	//if (isset($_GET["act"]) && $_GET["act"] != "upload" && $_GET["act"] != "xemdiem") 
	//{
	//$sql = "select * from user where user_name= '$y'";
	//$result = mysqli_query($connect,$sql);
	//$data = mysqli_fetch_assoc($result);
	//$_SESSION['menu_lannop']=$data['sub'];
	
//echo $data['sub'];
?>
<?php 
	if (isset($_GET["act"]) && $_GET["act"] != "upload" && $_GET["act"] != "xemdiem") :
	$y=$_SESSION['var'];//$y is any declared variable
	$sql = "select * from user where user_name= '$y'";
	$result = mysqli_query($connect,$sql);
	$data = mysqli_fetch_assoc($result);
	$_SESSION['menu_lannop']=$data['sub'];
	////////////////////////////////////////////////////
$sum_diemmon=0;
$dem_somon=0;
$sotu=0;
$sotu_diemmon=0;
$dem1=0;
$max=0;
$chuoi="";
$diemmon[]=NULL;
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
			
			 }
    }
            fclose($file_handle);
	
			$chuoi=explode(' ', $chuoi);
				
				$sotu=count($chuoi);
				
			for ($i = 4; $i < $sotu; $i=$i+2)
			{
				$arr[]=$chuoi[$i];
			}
			$sotu=count($arr);
			$_SESSION['tongmon']=$sotu-1;
			for ($i = 1; $i <= $sotu-1; $i++)
			{
				$duongdanmon="D:/workingDir/$arr[$i]/$y";
				if(is_dir($duongdanmon)) 
				{
					$dem_somon=$dem_somon+1;
					$duongdandiemmon="D:/workingDir/$arr[$i]/$y/maxScore.csv";
					if(file_exists($duongdandiemmon))
					{
						$filename_1=$duongdandiemmon;
						$file_1 = fopen($filename_1, "r");
						$count_1 = 0;     // add this line
						while (($emapData_1 = fgetcsv($file_1, 10000, ",")) !== FALSE)
						{
			
							$count_1++;// add this line

							if($count_1>1)
							{
							$diemmon[] = $emapData_1[2];
							}
				
						}
						fclose($file_1);
					}
					
				}
				
				
			}
			
				$_SESSION['demsomon']=$dem_somon;
				
				$sotu_diemmon=count($diemmon);
				$max=$diemmon[1];
				for ($i = 1; $i <= $sotu_diemmon-1; $i++)
				{
					if($max<$diemmon[$i]) {$max=$diemmon[$i];}
					$sum_diemmon=$sum_diemmon+$diemmon[$i];
				}
				if($sotu_diemmon-1 == 0) {
					$sum_diemmon=0;
				}
				else {
					$sum_diemmon=$sum_diemmon/($sotu_diemmon - 1);
				}
				$_SESSION['diemtb']=$sum_diemmon;
				$_SESSION['diemmax']=$max;
			
	
?>
 <?php  if ($_SESSION['sv']==1):?>
<!-- Overlay effect when opening sidebar on small screens -->
<div class="w3-overlay w3-hide-large w3-animate-opacity" onclick="w3_close()" style="cursor:pointer" title="close side menu" id="myOverlay"></div>

<!-- !PAGE CONTENT! -->
<div class="w3-main" style="margin-left:00px;margin-top:40px;">

  <!-- Header -->
  <header class="w3-container" style="padding-top:22px">
    <h5><b><i class="fa fa-line-chart"></i> Thống kê</b></h5>
  </header>

  <div class="w3-row-padding w3-margin-bottom">
    <div class="w3-quarter">
      <div class="w3-container w3-red w3-padding-16">
        <div class="w3-left"><i class="fa fa-code w3-xxxlarge"></i></div>
        <div class="w3-right">
          <h3><?php echo $_SESSION['menu_lannop']; ?></h3>
        </div>
        <div class="w3-clear"></div>
        <h4>Số lần nộp bài</h4>
      </div>
    </div>
    <div class="w3-quarter">
      <div class="w3-container w3-blue w3-padding-16">
        <div class="w3-left"><i class="fa fa-graduation-cap w3-xxxlarge"></i></div>
        <div class="w3-right">
          <h3> <?php echo $_SESSION['demsomon']; ?></h3>
        </div>
        <div class="w3-clear"></div>
        <h4>Môn học tham gia</h4>
      </div>
    </div>
    <div class="w3-quarter">
      <div class="w3-container w3-teal w3-padding-16">
        <div class="w3-left"><i class="fa fa-star w3-xxxlarge"></i></div>
        <div class="w3-right">
          <h3> <?php echo round($_SESSION['diemmax'],2); ?></h3>
        </div>
        <div class="w3-clear"></div>
        <h4>Điểm cao nhất</h4>
      </div>
    </div>
    <div class="w3-quarter">
      <div class="w3-container w3-orange w3-text-white w3-padding-16">
        <div class="w3-left"><i class="fa fa-pie-chart w3-xxxlarge"></i></div>
        <div class="w3-right">
          <h3><?php echo round($_SESSION['diemtb'],2); ?></h3>
        </div>
        <div class="w3-clear"></div>
        <h4>Điểm trung bình</h4>
      </div>
    </div>
  </div>

  <div class="w3-panel">
    <div class="w3-row-padding" style="margin:0 -16px">
      <div class="w3-third">
        <!---->
        <table class="w3-table w3-striped w3-white">
             
        </table>
    
      </div>
    </div>
  </div>
  <hr>
  <div class="w3-container">
    
    </table><br>
   
  </div>
  


</div>


</div>
<?php endif; ?>

<?php  if ($_SESSION['thay']==1):
	

?>
<!-- Overlay effect when opening sidebar on small screens -->
<div class="w3-overlay w3-hide-large w3-animate-opacity" onclick="w3_close()" style="cursor:pointer" title="close side menu" id="myOverlay"></div>

<!-- !PAGE CONTENT! -->
<div class="w3-main" style="margin-left:00px;margin-top:40px;">

  <!-- Header -->
  <header class="w3-container" style="padding-top:22px">
    <h5><b><i class="fa fa-line-chart"></i> Thống kê</b></h5>
  </header>

  <div class="w3-row-padding w3-margin-bottom">
    <div class="w3-quarter">
      <div class="w3-container w3-red w3-padding-16">
        <div class="w3-left"><i class="fa fa-users w3-xxxlarge"></i></div>
        <div class="w3-right">
          <h3><?php echo $_SESSION['tonghs']; ?></h3>
        </div>
        <div class="w3-clear"></div>
        <h4>Số học sinh</h4>
      </div>
    </div>
    <div class="w3-quarter">
      <div class="w3-container w3-blue w3-padding-16">
        <div class="w3-left"><i class="fa fa-graduation-cap w3-xxxlarge"></i></div>
        <div class="w3-right">
          <h3> <?php echo $_SESSION['tongmon']; ?></h3>
        </div>
        <div class="w3-clear"></div>
        <h4>Số môn học </h4>
      </div>
    </div>
    
    
  </div>

  <div class="w3-panel">
    <div class="w3-row-padding" style="margin:0 -16px">
      <div class="w3-third">
        <!---->
        <table class="w3-table w3-striped w3-white">
             
        </table>
    
      </div>
    </div>
  </div>
  <hr>
  <div class="w3-container">
    
    </table><br>
   
  </div>
  


</div>


</div>
<?php endif; ?>

<?php endif; ?>

</body>
</html>