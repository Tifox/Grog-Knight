<?php

App::uses('AppController', 'Controller');
require "../Lib/Infos.php";

class CharactersController extends AppController {
	public function		setCharacter($name) {
		$infos = new Infos("../../Grog-Knight/Resources/");
		$this->Session->write("character", $infos->getHero($name));
		$return = array(
			"name" => $name,
			"sprites" => $this->Session->read("character")->infos->sprites
		);
		echo json_encode($return);
	}
}

?>
