<?php
App::uses('AppController', 'Controller');
require "../Lib/Infos.php";

class StuffsController extends AppController {

	public function		weapon() {
		$infos = new Infos("../../Grog-Knight/Resources/");
		$infos->getInfos("Weapons");
		$this->set("infos", $infos->getResult());
		$this->set("obj", $infos);
	}

	public function		getInfoOnWeapon($name) {
		$infos = new Infos("../../Grog-Knight/Resources/");
		$this->set("res", $infos->getStuffInfo("Weapons", $name));
		$this->set("hero", $infos->getHero("Warrior"));
	}
}
?>
