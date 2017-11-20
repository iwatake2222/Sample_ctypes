<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
		<title>Controller</title>
		<!-- load css files -->
		<!-- <link rel="stylesheet" href="static/css/bootstrap.min.css"> -->
	</head>
	<body>		
		<main>
		<h1>Controller</h1>
		<div>
			<div>
				<h2>LED</h2>
				LED0: <input type="checkbox" id="checkbox-led-0" name="checkbox-led-0" data-on-color="primary" onclick="setLed('0', this.checked)"><br>
				LED1: <input type="checkbox" id="checkbox-led-1" name="checkbox-led-1" data-on-color="primary" onclick="setLed('1', this.checked)">
				<hr>
			</div>
			<div>
				<h2>Button</h2>
				Button0: <input type="checkbox" id="checkbox-button-0" name="checkbox-button-0" data-on-color="primary"><br>
				Button1: <input type="checkbox" id="checkbox-button-1" name="checkbox-button-1" data-on-color="primary">
				<hr>
			</div>
		</div>
		</main>
		<!-- load script files -->
		<script type="text/javascript" src="static/js/caller.js" charset="utf-8"></script>
	</body>
</html>
