transparentOn = 0;
currentChart = 0;

$(document).ready(function() {
	$(".tiles li").on("click", function() {
		transparentOn = 1;
		$.ajax({
			url: "/Grog-Knight/stuffs/getInfoOnWeapon/" + $(this).attr("id")
		}).done(function(data) {
			$(".out-window").html(data);
			$(".out-window canvas").css({"height": "400px", "width": "400px"});
			$(".transparent").fadeIn(300);
			$(".out-window").fadeIn(300);
			eval(document.getElementById("runscript").innerHTML);
			var ctx = document.getElementById("chart").getContext("2d");
			currentChart = new Chart(ctx).Radar(data);

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
	});
});
