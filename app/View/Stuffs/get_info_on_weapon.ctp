<?php
	$hero = $this->Session->read("character");
	echo "<script type=\"text/javascript\" id=\"runscript\">";
	echo 'var data = { labels: ["Level Min", "Critical", "Damage", "Loot Level", "Recovery time"],';
	echo "datasets: [";
	echo '{"Label": "Stats", fillColor: "rgba(30,30,32,0.2)",
            strokeColor: "rgba(220,220,220,1)",
            pointColor: "rgba(220,220,220,1)",
            pointStrokeColor: "#fff",
            pointHighlightFill: "#fff",
            pointHighlightStroke: "rgba(220,220,220,1)",';
	echo "data: [".$res->infos->starterLevel.",".$res->infos->critRate.",".$res->infos->damage.",".$res->infos->lootLevel.",".$res->infos->recovery."]}]};";
	echo "</script>";
	echo "<script type='text/javascript' id='runscripttwo'>";
		echo "var baseHP = ".$hero->infos->maxHP.";";
		echo "var baseCrit = ".$hero->infos->baseCrit.";";
		echo "var baseDamage = ".$hero->infos->baseDamage.";";
		echo "var baseSpeed = ".$hero->Actions->forward->force.";";
		echo "var critMult = ".$hero->Stats->critMult.";";
		echo "var damageMult = ".$hero->Stats->damageMult.";";
		echo "var HPMult = ".$hero->Stats->HPMult.";";
		echo "var data2 = { labels: ['HP', 'Crit', 'Base Damage', 'Speed'],";
		echo "datasets: [";
		echo '{"Label": "Stats", fillColor: "rgba(30,30,32,0.5)",
			strokeColor: "rgba(220,220,220,0.8)",
			highlightFill: "rgba(220,220,220,0.75)",
			highlightStroke: "rgba(220,220,220,1)",';
		echo "data: [".$hero->infos->maxHP.",".($hero->infos->baseCrit + $res->infos->critRate).",".($hero->infos->baseDamage + $res->infos->damage).",".$hero->Actions->forward->force."]}]}";
	echo "</script>";
?>
<ul>
	<li class='main'>
		<canvas id="chart"></canvas>
		<canvas id="chart2"></canvas>
		<input class='characterLevel' type="number" value="1">
	</li>
	<li class='main'>
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
			echo "<ul class='form'>";
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
</li>
