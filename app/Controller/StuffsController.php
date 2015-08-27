<?php
App::uses('AppController', 'Controller');
require "../Lib/Infos.php";

class StuffsController extends AppController
{
	
	public function weapon() {
		$infos = new Infos("../../Grog-Knight/Resources/");
		$res = $infos->getInfos("Weapons");
		echo "<pre>";
		print_r($res);
		echo "</pre>";
	}
}
?>
