<div class='title'><?= $name ?></div>
<ul class='tiles'>
	<?php
		foreach ($infos as $i) {
			echo "<li id='".$name."/".$i->infos->name."'>";
				echo "<img src='".$obj->imgHelper($i->infos->sprites)."'>";
				echo "<h1>".$i->infos->displayName."</h1>";
			echo "</li>";
		}
	?>
</ul>
