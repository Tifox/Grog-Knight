<?php
App::uses('AppController', 'Controller');
require "../Lib/Infos.php";

/**
 - attack
 - defense
 - specials:
 	- 


*/

class StuffsController extends AppController {

	public function		index($name) {
		$infos = new Infos("../../Grog-Knight/Resources/");
		if ($this->request->is('post')) {
			$result["infos"] = $this->request->data[$name];
			$infos->update($name, $result["infos"]["name"], $result);
		}
		$infos->getInfos($name);
		$this->set("infos", $infos->getResult());
		$this->set("obj", $infos);
		$this->set("name", $name);
	}

	public function		getInfoOn($type, $name) {
		$infos = new Infos("../../Grog-Knight/Resources/");
		$this->set("res", $infos->getStuffInfo($type, $name));
		$this->set("type", $type);
	}
}
?>
