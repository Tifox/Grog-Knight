<?php
	$hero = $this->Session->read("character");
	echo "<script type=\"text/javascript\" id=\"runscript\">";
		echo "var baseHP = ".$hero->infos->HP.";";
		echo "var baseCrit = ".$hero->infos->baseCrit.";";
		echo "var baseDamage = ".$hero->infos->baseDamage.";";
		echo "var baseDamageReduction = ".$hero->infos->baseDamageReduction.";";
		echo "var baseSpeed = ".$hero->Actions->forward->force.";";
		echo "var critMult = ".$hero->Stats->critMult.";";
		echo "var damageMult = ".$hero->Stats->damageMult.";";
		echo "var HPMult = ".$hero->Stats->HPMult.";";
		echo "var damageReductionMult = ".$hero->Stats->damageReductionMult.";";


	$bonus = array(0);
	switch ($type) {
		case "Weapons":
			$bonus["crit"] = $res->infos->critRate;
			$bonus["damage"] = $res->infos->damage;
			$bonus["speed"] = $bonus["hp"] = $bonus["reduction"] = 0;
		break;
		case "Armors": 
			$bonus["speed"] = $res->infos->speed;
			$bonus["hp"] = $res->infos->hpBuff;
			$bonus["reduction"] = $res->infos->damageReduction;
			$bonus["crit"] = $bonus["damage"] = 0;
		break;
	}

	if ($type == "Weapons") {
		echo 'var data = { labels: ["Level Min", "Critical", "Damage", "Loot Level", "Recovery time"],';
		echo "datasets: [";
		echo '{"Label": "Stats", fillColor: "rgba(30,30,32,0.2)",
   	         strokeColor: "rgba(220,220,220,1)",
   	         pointColor: "rgba(220,220,220,1)",
   	         pointStrokeColor: "#fff",
   	         pointHighlightFill: "#fff",
   	         pointHighlightStroke: "rgba(220,220,220,1)",';
				echo "data: [".$res->infos->lvlRequired.",".
					$res->infos->critRate.",".$res->infos->damage.",".
						$res->infos->lootLevel.",".$res->infos->recovery."]}]};";
	} else if ($type == "Armors") {
		echo 'var data = { labels: ["Level Min", "HP Buff", "Damage Reduction", "Loot Level", "Speed bonus"],';
		echo "datasets: [";
		echo '{"Label": "Stats", fillColor: "rgba(30,30,32,0.2)",
   	         strokeColor: "rgba(220,220,220,1)",
   	         pointColor: "rgba(220,220,220,1)",
   	         pointStrokeColor: "#fff",
   	         pointHighlightFill: "#fff",
   	         pointHighlightStroke: "rgba(220,220,220,1)",';
				echo "data: [".$res->infos->lvlRequired.",".
					$res->infos->hpBuff.",".$res->infos->damageReduction.",".
						$res->infos->lootLevel.",".$res->infos->speed."]}]};";

	}
	echo "</script>";
	echo "<script type='text/javascript' id='runscripttwo'>";
			echo "var data2 = { labels: ['HP', 'Crit', 'Base Damage', 'Speed', 'Damage Reduction'],";
		echo "datasets: [";
		echo '{"Label": "Stats", fillColor: "rgba(30,30,32,0.5)",
			strokeColor: "rgba(220,220,220,0.8)",
			highlightFill: "rgba(220,220,220,0.75)",
			highlightStroke: "rgba(220,220,220,1)",';
		echo "data: [".($hero->infos->HP + $bonus["hp"]).","
				.($hero->infos->baseCrit + $bonus["crit"]).","
				.($hero->infos->baseDamage + $bonus["damage"]).","
				.($hero->Actions->forward->force + $bonus["speed"]).", "
				.($hero->infos->baseDamageReduction + $bonus["reduction"])."]}]}";
	echo "</script>";
?>
<ul>
	<li class='main'>
		<canvas id="chart"></canvas>
		<canvas id="chart2"></canvas>
		<input class='characterLevel' type="number" value="1">
	</li>
	<li class='main'>
	<?php
		if ($type == "Weapons") {
		$keys = array(
			"lvlRequired" => 0,
			"critRate" => 1,
			"damage" => 2,
			"lootLevel" => 3,
			"recovery" => 4
		);
		} else if ($type == "Armors") {
			$keys = array(
				"lvlRequired" => 0,
				"lootLevel" => 3,
				"hpBuff" => 1,
				"damageReduction" => 2,
				"speedBonus" => 4
			);
		}
		echo "<form action='/Grog-Knight/stuffs/index/".$type."' method='post' id='".$type."'>";
		foreach ($res->infos as $key => $f) {
			echo "<ul class='form'>";
			if ($key == "flavor") {
					echo "<li class='spe'><div class='before-t'>".$key."</div></li>";
					echo "<li class='input'><textarea>".$f."</textarea></li>";
			} else if (is_numeric($f) || is_int($f)) {
				echo "<li><div class='before'>".$key."</div></li>";
				if (array_key_exists($key, $keys))
					echo "<li class='input'><input id='".$keys[$key]."' class='changeChart' type='number' value='".$f."' name='data[".$type."][".$key."]'></li>";
				else
					echo "<li class='input'><input type='number' value='".$f."' name='data[".$type."][".$key."]'></li>";
			} else {
				echo "<li><div class='before'>".$key."</div></li>";
				echo "<li class='input'><input type='text' value='".$f."' name='data[".$type."][".$key."]'></li>";
			}
			echo "</ul>";
		}
		echo "<button type='submit'>Submit</button>";
		echo "</form>";
	?>
</li>
