transparentOn = 0;
currentChart = currentChart2 = 0;

$(document).ready(function() {
	$(".tiles li").on("click", function() {
		transparentOn = 1;
		$(".transparent").fadeIn(300);
		$.ajax({
			url: "/Grog-Knight/stuffs/getInfoOnWeapon/" + $(this).attr("id")
		}).done(function(data) {
			$(".out-window").html(data);
			$(".out-window #chart").css({"height": "400px", "width": "400px"});
			$(".out-window #chart2").css({"height": "200px", "width": "500px"});
			$(".out-window").fadeIn(300);
			eval(document.getElementById("runscript").innerHTML);
			eval(document.getElementById("runscripttwo").innerHTML);
			var ctx = document.getElementById("chart").getContext("2d");
			currentChart = new Chart(ctx).Radar(data);
			var ctx = document.getElementById("chart2").getContext("2d");
			currentChart2 = new Chart(ctx).HorizontalBar(data2, {"scaleShowVerticalLines": false});
		});
	});

	$(".transparent").on("click", function() {
		$(".transparent").fadeOut(300);
		$(".out-window").fadeOut(300);
		transparentOn = 0;
	});

	$(document).on("change", ".changeChart", function() {
		currentChart.datasets[0].points[$(this).attr("id")].value = $(this).val();
		currentChart.update();
		var k = $(".characterLevel").val(), j;
		var plusCrit = 0, plusDamage = 0, plusHP = 0;

		for (j = 0; j < k; j++) {
			plusCrit += baseCrit * critMult;
			plusDamage += baseDamage * damageMult;
			plusHP += baseHP * HPMult;
		}

		if ($(this).attr("id") == 2) {
			currentChart2.datasets[0].bars[2].value = baseDamage + plusDamage + parseInt(currentChart.datasets[0].points[2].value);
		} else if ($(this).attr("id") == 1)
			currentChart2.datasets[0].bars[1].value = baseCrit + plusCrit + parseInt(currentChart.datasets[0].points[1].value);
		currentChart2.update();
	});

	$(document).on("change", ".characterLevel", function() {
		var k = $(this).val(), j;
		var plusCrit = 0, plusDamage = 0, plusHP = 0;

		for (j = 0; j < k; j++) {
			plusCrit += baseCrit * critMult;
			plusDamage += baseDamage * damageMult;
			plusHP += baseHP * HPMult;
		}

		currentChart2.datasets[0].bars[0].value = baseHP + plusHP;
		currentChart2.datasets[0].bars[1].value = baseCrit + plusCrit;
		currentChart2.datasets[0].bars[2].value = baseDamage + plusDamage;
		currentChart2.update();
	});

	$(document).on("change", "#characterChange", function() {
		$.ajax({
			url: "/Grog-Knight/characters/setCharacter/" + $(this).val()
		}).done(function(data) {
			var obj = JSON.parse(data);
			$("#header .character h1").text(obj.name);
			$("#header .character img").attr("src", "/Grog-Knight/img/Images/" + obj.sprites);
		});
	});
});
