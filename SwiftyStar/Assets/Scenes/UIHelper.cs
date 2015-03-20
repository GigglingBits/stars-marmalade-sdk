using UnityEngine;
using System.Collections;

public class UIHelper : MonoBehaviour {

	public void OpenGameModeMenuScene() {
		Application.LoadLevel("GameModeMenu");
	}

	public void OpenTitleMenuScene() {
		Application.LoadLevel("TitleMenu");
	}
	
	public void OpenLevelMenuScene() {
		Application.LoadLevel("LevelMenu");
	}
	
	public void OpenGamePlayScene() {
		Application.LoadLevel("GamePlay");
	}
	
	public void OpenCompletionMenuScene() {
		Application.LoadLevel("CompletionMenu");
	}
	
}
