<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
		<title>Controller</title>
		<!-- load css files -->
		<link rel="stylesheet" href="static/css/bootstrap.min.css">
	</head>
	<body>		
		<main class="container">
		<h1>Controller</h1>
		<div class="row">
			<div class="col-md">
				<h2>LED</h2>
				LED0: <input type="checkbox" id="checkbox-led-0" name="checkbox-led-0" data-on-color="primary"><br>
				LED1: <input type="checkbox" id="checkbox-led-1" name="checkbox-led-1" data-on-color="primary">
				<hr>
			</div>
			<div class="col-md">
				<h2>COLOR LED</h2>
				R: <input type="range" id="range-led-r" name="range-led-r" value="0" min="0" max="100" step="1"><br>
				G: <input type="range" id="range-led-g" name="range-led-g" value="0" min="0" max="100" step="1"><br>
				B: <input type="range" id="range-led-b" name="range-led-b" value="0" min="0" max="100" step="1">
				<hr>
			</div>
			<div class="col-md">
				<h2>Other</h2>
				Speaker: <input type="range" id="range-speaker" name="range-speaker" value="0" min="0" max="20000" step="1"><br>
				Motor: <input type="range" id="range-motor" name="range-motor" value="0" min="0" max="100" step="1"><br>
				<!-- Servo: <input type="range" id="range-servo" name="range-servo" value="0" min="0" max="100" step="1"><br> -->
				Text: <input type="text" id="text-oled" name="text-oled" value="hello">
				<hr>
			</div>
		</div>
		<div class="row">
			<div class="col-md">
				<h2>Button</h2>
				<button type="button" class="btn btn-primary" id="button-0" name="button-0">Button0</button><br>
				<button type="button" class="btn btn-primary" id="button-1" name="button-1">Button1</button>
				<hr>
			</div>
			<div class="col-md">
				<h2>Acc</h2>
				X: <span id="text-accel-x" name="text-accel-x">0</span><br>
				Y: <span id="text-accel-y" name="text-accel-y">0</span><br>
				Z: <span id="text-accel-z" name="text-accel-z">0</span><br>
				<!-- TAP: <span id="text-accel-tap" name="text-accel-tap">0</span> -->
				<hr>
			</div>
			<div class="col-md">
				<h2>Camera</h2>
				<img id="img-camera" name="img-camera">
				<hr>
			</div>
		</div>
		</main>
		<!-- load script files -->
		<script type="text/javascript" src="static/js/caller.js" charset="utf-8"></script>
	</body>
</html>
