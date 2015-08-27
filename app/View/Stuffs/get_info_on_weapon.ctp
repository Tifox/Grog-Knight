<?php
	echo "<script type=\"text/javascript\" id=\"runscript\">";
	echo 'var data = { labels: ["Level Min", "Critical", "Damage", "Loot Level", "Recovery time"],';
	echo "datasets: [";
	echo '{"Label": "Stats", fillColor: "rgba(220,220,220,0.2)",
            strokeColor: "rgba(220,220,220,1)",
            pointColor: "rgba(220,220,220,1)",
            pointStrokeColor: "#fff",
            pointHighlightFill: "#fff",
            pointHighlightStroke: "rgba(220,220,220,1)",';
	echo "data: [".$res->infos->lvlRequired.",".$res->infos->critRate.",".$res->infos->damage.",".$res->infos->lootLevel.",".$res->infos->recovery."]}]};";
	echo "</script>";
?>

<h1><?= $res->infos->name ?></h1>
<canvas id="chart"></canvas>
<form>
	<?php
		$keys = array(
			"lvlRequired" => 0,
			"critRate" => 1,
			"damage" => 2,
			"lootLevel" => 3,
			"recovery" => 4
		);
		foreach ($res->infos as $key => $f) {
			echo "<ul>";
			if ($key == "flavor") {
					echo "<li class='spe'><div class='before-t'>".$key."</div></li>";
					echo "<li class='input'><textarea>".$f."</textarea></li>";
			} else if (is_numeric($f) || is_int($f)) {
				echo "<li><div class='before'>".$key."</div></li>";
				if (array_key_exists($key, $keys))
					echo "<li class='input'><input id='".$keys[$key]."' class='changeChart' type='number' value='".$f."' name='".$key."'></li>";
				else
					echo "<li class='input'><input type='number' value='".$f."' name='".$key."'></li>";
			} else {
				echo "<li><div class='before'>".$key."</div></li>";
				echo "<li class='input'><input type='text' value='".$f."'></li>";
			}
			echo "</ul>";
		}
	?>
</form>
