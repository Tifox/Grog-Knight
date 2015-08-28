transparentOn = 0;
currentChart = currentChart2 = 0;

$(document).ready(function() {
	$(".tiles li").on("click", function() {
		transparentOn = 1;
		$.ajax({
			url: "/Grog-Knight/stuffs/getInfoOnWeapon/" + $(this).attr("id")
		}).done(function(data) {
			$(".out-window").html(data);
			$(".out-window #chart").css({"height": "400px", "width": "400px"});
			$(".out-window #chart2").css({"height": "200px", "width": "500px"});
			$(".transparent").fadeIn(300);
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
		//alert($(this).attr("id"));
		if ($(this).attr("id") == 2) {
			currentChart2.datasets[0].bars[2].value = baseDamage + parseInt(currentChart.datasets[0].points[2].value);
		} else if ($(this).attr("id") == 1)
			currentChart2.datasets[0].bars[1].value = baseCrit + parseInt(currentChart.datasets[0].points[1].value);
		currentChart2.update();
	});
});
