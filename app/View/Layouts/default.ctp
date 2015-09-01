<?php
/**
 * CakePHP(tm) : Rapid Development Framework (http://cakephp.org)
 * Copyright (c) Cake Software Foundation, Inc. (http://cakefoundation.org)
 *
 * Licensed under The MIT License
 * For full copyright and license information, please see the LICENSE.txt
 * Redistributions of files must retain the above copyright notice.
 *
 * @copyright     Copyright (c) Cake Software Foundation, Inc. (http://cakefoundation.org)
 * @link          http://cakephp.org CakePHP(tm) Project
 * @package       app.View.Layouts
 * @since         CakePHP(tm) v 0.10.0.1076
 * @license       http://www.opensource.org/licenses/mit-license.php MIT License
 */

$cakeDescription = __d('cake_dev', 'CakePHP: the rapid development php framework');
$cakeVersion = __d('cake_dev', 'CakePHP %s', Configure::version());
error_reporting(0);
?>
<!DOCTYPE html>
<html>
<head>
	<?php echo $this->Html->charset(); ?>
	<title>
		<?php echo $cakeDescription ?>:
		<?php echo $this->fetch('title'); ?>
	</title>
	<?php
		echo $this->Html->meta('icon');

		echo $this->Html->css(array(
			"main",
			"https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css"
		));

		echo $this->Html->script(array(
			"https://ajax.googleapis.com/ajax/libs/jquery/2.1.4/jquery.min.js",
			"Chart.min",
			"Chart.HorizontalBar",
			"main.js"
		));

		echo $this->fetch('meta');
		echo $this->fetch('css');
		echo $this->fetch('script');
	?>
</head>
<body>
	<div id="container">
		<div class='transparent'></div>
		<div class='out-window'></div>
		<div id="header">
			<div class='logo'>
				<i class='fa fa-linux'></i>
				<h1>Grog-Knight</h1>
			</div>
			<ul>
				<li>Home</li>
				<li>Armors</li>
				<li>Rings</li>
				<li>Weapons</li>
				<li>Git</li>
			</ul>
			<div class='character'>
				<h1><?= $this->Session->read("character")->infos->name ?></h1>
				<img src="/Grog-Knight/img/Images/<?= $this->Session->read("character")->infos->sprites ?>">
				<br />
				<select id="characterChange">
					<option value="Warrior">Warrior</option>
					<option value="Archer">Archer</option>
				</select>
			</div>
		</div>
		<div id="content">
			<?php echo $this->Flash->render(); ?>
			<?php echo $this->fetch('content'); ?>
		</div>
		<div id="footer">
		</div>
	</div>
	<?php echo $this->element('sql_dump'); ?>
</body>
</html>
