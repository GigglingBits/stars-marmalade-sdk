using UnityEngine;
using System.Collections;

public class GameSceneManager
{
	#region singleton
	private static GameSceneManager _theInstance = null;
	
	public static GameSceneManager instance {
		get {
			if (null == _theInstance) {
				_theInstance = new GameSceneManager ();
			}
			return _theInstance;
		}
	}
	#endregion
	
	public int currentLevel { get; set; }

	public void OpenGameModeMenuScene ()
	{
		Application.LoadLevel ("GameModeMenu");
	}
	
	public void OpenTitleMenuScene ()
	{
		Application.LoadLevel ("TitleMenu");
	}
	
	public void OpenLevelMenuScene ()
	{
		Application.LoadLevel ("LevelMenu");
	}
	
	public void OpenGamePlayScene (int level)
	{
		currentLevel = level;
		Application.LoadLevel ("GamePlay");
	}
	
	public void OpenCompletionMenuScene ()
	{
		Application.LoadLevel ("CompletionMenu");
	}
}
