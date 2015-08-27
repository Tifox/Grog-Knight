<?php

class Infos {

	private		$_path;
	private		$_currentResult;

	public function __construct($path) {
		$this->_path = $path;
	}

	public function		getInfos($type) {
		$files = scandir($this->_path."Elements/".$type);
		$result = array();

		foreach ($files as $f) {
			if ($f[0] != ".") {
				$content = file_get_contents($this->_path."Elements/".$type."/".$f);
				$result[] = json_decode($content);
			}
		}
		$this->_currentResult = $result;
		return $result;
	}

	public function		getStuffInfo($type, $name) {
		$content = file_get_contents($this->_path."Elements/".$type."/".$name.".json");
		return json_decode($content);
	}

	public function		imgHelper($path) {
		$result = explode('/', $path);
		return "/Grog-Knight/img/Images/".$result[count($result) - 1];
	}

	public function		getAllImages() {
		$result = array();
		foreach ($this->_currentResult as $r) {
			$result[] = $this->imgHelper($r->infos->sprites);
		}
		return $result;
	}

	public function		getResult() { return $this->_currentResult; }
}

?>
