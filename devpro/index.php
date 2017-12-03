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
</head>
<body>



	<!-- 'start thực hiện kiểm tra dữ liệu người dùng nhập ở form đăng nhập' -->
	<?php
		if(isset($_POST["dangnhap"]))
		{
			$tk = $_POST["user_name_lg"];
			$_SESSION['var']=$tk;
			$mk =($_POST["passlg"]);
			$rows = mysqli_query($connect," select * from user where user_name = '$tk' and password = '$mk' and quyen = 'sv'");
			$count = mysqli_num_rows($rows);
			if($count==1)
			{
				
				$_SESSION["loged"] = true;
				header("location:index.php");
				echo "bal";
				setcookie("success", "Đăng nhập thành công!", time()+1, "/","", 0);
			}
			else{
				header("location:index.php");
				setcookie("error", "Đăng nhập không thành công!", time()+1, "/","", 0);
			}
			
		}
	
	//<!-- 'end thực hiện kiểm tra dữ liệu người dùng nhập ở form đăng nhập' -->



	//<!-- 'start thực hiện đăng xuất' -->
	
		if(isset($_GET["act"])&&$_GET["act"]=="logout"){
			unset($_SESSION["loged"]);
			header("location:index.php");
			setcookie("success", "Bạn đã đăng xuất! .$mssv", time()+1, "/","", 0);
		}
	?>
	<!-- end thực hiện đăng xuất -->

	<div class="container">
		<div class="row">
		
			<a href="index.php" class="btn btn-info">Trang chủ</a>
			<?php 
			if(isset($_SESSION["loged"])) 
			{
				echo "<a href='index.php?act=logout' class='btn btn-danger'>Đăng xuất</a>"; 
				//echo "<a href='index.php?act=upload' class='btn btn-success'>Upload</a>";
			}
			?>
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
			<div class="alert alert-success">
			  	<strong>'Chúc mừng!'</strong> <?php echo $_COOKIE["success"]; ?>
			</div>
			<?php } ?>
			<!-- 'end nếu thành công thì hiện thông báo:' -->




			
			<?php
			
			//nếu không tồn tại biến $_GET["page"] = "dangky"
			if(!isset($_GET["page"]))
			{
				//nếu tồn tại biến session $_SESSION["loged"] thì gọi nội dung trang upload.php vào
				if(isset($_SESSION["loged"]))
				{
					
					include "upload.php";

				
				}
				
				//nếu không tồn tại biến session $_SESSION["loged"] thì gọi nội dung trang login.php vào
				else
					include "login.php";
			}
			?>
		</div>

	</div>

</body>
</html>