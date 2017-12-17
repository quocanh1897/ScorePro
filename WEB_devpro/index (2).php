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
  
    <a href = "index.php?act=showmenu"  class="w3-bar-item w3-button w3-padding w3-blue"><i class="fa fa-male fa-fw"></i>  Tổng quan</a>
    <a href="index.php?act=xemdiem" class="w3-bar-item w3-button w3-padding"><i class="fa fa-eye fa-fw"></i>  Điểm từng môn</a>
    
    <a href="#" class="w3-bar-item w3-button w3-padding"><i class="fa fa-history fa-fw"></i>  Lịch sử nộp bài</a>
    <a href="#" class="w3-bar-item w3-button w3-padding"><i class="fa fa-cog fa-fw"></i>  Cài đặt</a><br><br>
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
	  	<a href='#' class='w3-bar-item w3-button w3-padding w3-green'><i class='fa fa-eye fa-fw'></i>  Điểm từng môn</a>
	   
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
	  	<a href='#' class='w3-bar-item w3-button w3-padding w3-green'><i class='fa fa-eye fa-fw'></i>  Điểm từng môn</a>
	   
	  </div>
	</nav>";
}
?>
<?php endif; ?>


<!-- END ACTION XEMDIEM -->



<?php 
	if (isset($_GET["act"]) && $_GET["act"] != "upload" && $_GET["act"] != "xemdiem") :
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
        <div class="w3-left"><i class="fa fa-code w3-xxxlarge"></i></div>
        <div class="w3-right">
          <h3>52</h3>
        </div>
        <div class="w3-clear"></div>
        <h4>Số lần nộp bài</h4>
      </div>
    </div>
    <div class="w3-quarter">
      <div class="w3-container w3-blue w3-padding-16">
        <div class="w3-left"><i class="fa fa-graduation-cap w3-xxxlarge"></i></div>
        <div class="w3-right">
          <h3>4</h3>
        </div>
        <div class="w3-clear"></div>
        <h4>Môn học tham gia</h4>
      </div>
    </div>
    <div class="w3-quarter">
      <div class="w3-container w3-teal w3-padding-16">
        <div class="w3-left"><i class="fa fa-star w3-xxxlarge"></i></div>
        <div class="w3-right">
          <h3>10</h3>
        </div>
        <div class="w3-clear"></div>
        <h4>Điểm cao nhất</h4>
      </div>
    </div>
    <div class="w3-quarter">
      <div class="w3-container w3-orange w3-text-white w3-padding-16">
        <div class="w3-left"><i class="fa fa-pie-chart w3-xxxlarge"></i></div>
        <div class="w3-right">
          <h3>8.56</h3>
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
          <tr>
            <td><i class="fa fa-user w3-text-blue w3-large"></i></td>
            <td>New record, over 90 views.</td>
            <td><i>10 mins</i></td>
          </tr>
          <tr>
            <td><i class="fa fa-bell w3-text-red w3-large"></i></td>
            <td>Database error.</td>
            <td><i>15 mins</i></td>
          </tr>
          <tr>
            <td><i class="fa fa-users w3-text-yellow w3-large"></i></td>
            <td>New record, over 40 users.</td>
            <td><i>17 mins</i></td>
          </tr>
          <tr>
            <td><i class="fa fa-comment w3-text-red w3-large"></i></td>
            <td>New comments.</td>
            <td><i>25 mins</i></td>
          </tr>
          <tr>
            <td><i class="fa fa-bookmark w3-text-blue w3-large"></i></td>
            <td>Check transactions.</td>
            <td><i>28 mins</i></td>
          </tr>
          <tr>
            <td><i class="fa fa-laptop w3-text-red w3-large"></i></td>
            <td>CPU overload.</td>
            <td><i>35 mins</i></td>
          </tr>
          <tr>
            <td><i class="fa fa-share-alt w3-text-green w3-large"></i></td>
            <td>New shares.</td>
            <td><i>39 mins</i></td>
          </tr>
        </table>
    
      </div>
    </div>
  </div>
  <hr>
  <div class="w3-container">
    
    </table><br>
    <button class="w3-button w3-dark-grey">More Countries  <i class="fa fa-arrow-right"></i></button>
  </div>
  

  <!-- Footer -->
  <footer class="w3-container w3-padding-16 w3-light-grey">
    <h4>FOOTER</h4>
    <p>Powered by <a href="https://www.w3schools.com/w3css/default.asp" target="_blank">w3.css</a></p>
  </footer>

  <!-- End page content -->
</div>


</div>
<?php endif; ?>

</body>
</html>