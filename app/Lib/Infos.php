<?php

class Infos {

	private		$_path;

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
		return $result;
	}

	public function		imgHelper($path) {
		return $this->_path.$path;
	}
}

?>
