<div class='title'>Weapon</div>
<ul class='tiles'>
	<?php
		foreach ($infos as $i) {
			echo "<li id='".$i->infos->name."'>";
				echo "<img src='".$obj->imgHelper($i->infos->sprites)."'>";
				echo "<h1>".$i->infos->name."</h1>";
			echo "</li>";
		}
	?>
</ul>
