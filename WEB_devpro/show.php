<?php
$con=mysqli_connect("localhost","root","","phpdb");
mysqli_set_charset($con,"utf8");
// Check connection
$result_1 = mysqli_query($con,"SELECT * FROM diem_1");



echo 
"
<div class = 'w3-container' style='margin-left:180px;margin-top:10px;'>


	<ul class = 'w3-ul w3-card-4 w3-white'>	<li class = 'w3-padding-16'>
	<div  class='w3-main' style='margin-left:10px'>
	
	</div >

	    <div class='w3-main' style='margin-left:10px;'>
		<div class = 'w3-container' style='margin-top:0px;'>

	<center> <table border=1 ORDERCOLOR=Red  cellspacing=0 cellpading=0>
	<tr>
	<th height=5 width = 150><center> <font color=black> MSSV </font></center></th>
	<th width = 150><center><font color=black> Số Lần Nộp</font> </center></th>
	<th width = 150><center><font color=black> Điểm Cao Nhất </font> </center></th>
	<th width = 150><center><font color=black> Thời gian nộp </font> </center></th>
	</tr> </center>


</div>
		
	    </div>
	
	</ul>



<h1>Bảng điểm tổng kết ".$_SESSION['chonmon']=$mon_hoc."</h1>
</div>
";

while($row_1 = mysqli_fetch_array($result_1))
{
echo "<tr>";
echo "<td><font color=black><center>" . $row_1['MSSV'] . "</font> </center> </td>";
echo "<td><font color=black><center>" . $row_1['lannop'] . "</font> </center> </td>";
echo "<td><font color=black><center>" . $row_1['diem'] . "</font> </center> </td>";
echo "<td><font color=black><center>" . $row_1['time'] . "</font> </center> </td>";
echo "</tr>";
}
echo "</table>";

##########################



$result_2 = mysqli_query($con,"SELECT * FROM diem_2");
echo 
"
<div class = 'w3-container' style='margin-top:50px;'>
<ul class = 'w3-ul w3-card-4 w3-white' style = 'padding:50px'>
	<center> <table border=1 ORDERCOLOR=Red  cellspacing=0 cellpading=0>
	<tr>
	<th height=5 width = 150><center> <font color=black> MSSV </font></center></th>
	<th width = 150><center><font color=black> Lần Nộp</font> </center></th>
	<th width = 150><center><font color=black> Điểm Lần Nộp</font> </center></th>
		<th width = 150><center><font color=black> Thời gian nộp</font> </center></th>
	</tr> </center>
</ul>
<h1>Bảng điểm thành phần ". $_SESSION['chonmon']=$mon_hoc."</h1>
</div>
";

while($row_2 = mysqli_fetch_array($result_2))
{
echo "<tr>";
echo "<td><font color=black><center>" . $row_2['MSSV'] . "</font> </center> </td>";
echo "<td><font color=black><center>" . $row_2['lannop'] . "</font> </center> </td>";
echo "<td><font color=black><center>" . $row_2['diem'] . "</font> </center> </td>";
echo "<td><font color=black><center>" . $row_2['time'] . "</font> </center> </td>";
echo "</tr>";
}
echo "</table>";

mysqli_close($con);

?>