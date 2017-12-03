<?php
$y=$_SESSION['var'];//$y is any declared variable
if(isset($_POST['submit']))
{
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
				//save the filename
				$shortname = $_FILES['upload']['name'][$i];
				//save the url and the file
				$filePath = "uploaded/$y/".$_FILES['upload']['name'][$i];
				//Upload the file into the temp dir
				if(move_uploaded_file($tmpFilePath, $filePath)) 
				{
					$files[] = $shortname;
					//copy('demo2.txt', 'demo3.txt');
					//insert into db 
					//use $shortname for the filename
					//use $filePath for the relative url to the file
				}
			}
		}
	}

//show success message
	echo "<h1>Uploaded:</h1>";    
	if(is_array($files))
	{
	echo "<ul>";
	foreach($files as $file)
	{
		echo "<li>$file</li>";
	}
	echo "</ul>";
	}
	if(is_dir("D:/submitFolder/$y"))
	{
		$src = "uploaded/$y";
		$dst = "D:/submitFolder/$y";
		$files = glob("uploaded/$y/*.*");
		foreach($files as $file)
		{
			$file_to_go = str_replace($src,$dst,$file);
			copy($file, $file_to_go);	
		}			
	}
	else
	{
		mkdir("D:/submitFolder/$y");
		$src = "uploaded/$y";
		$dst = "D:/submitFolder/$y";
		$files = glob("uploaded/$y/*.*");
		foreach($files as $file)
		{
			$file_to_go = str_replace($src,$dst,$file);
			copy($file, $file_to_go);	
		}			
	}
echo "I love .$y" ;
}
?>
<form action="" enctype="multipart/form-data" method="post">

    <div>
        <label for='upload'>Add các file vào đây:</label>
        <input id='upload' name="upload[]" type="file" multiple="multiple" />
    </div>

	<!-- <p><input type="submit" name="submit" value="Submit"></p>      -->
	<p><input type="submit" class='btn btn-danger' name="submit" value="Submit">Submit vào đây nè!</p>  
	<!-- <a href='index.php?act=logout' class='btn btn-danger'>Đăng xuất</a>"; 
	<p><input type="submit" class="btn btn-default" name="submit" value<"Submit"> Submit</button></p>
	-->
   

</form>


















