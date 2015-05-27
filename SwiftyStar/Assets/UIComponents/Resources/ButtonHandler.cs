using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class ButtonHandler : MonoBehaviour
{

	public string buttonId;

	// Use this for initialization
	void Start ()
	{
		AddClickHandler ();
	}
	
	// Update is called once per frame
	void Update ()
	{
	
	}

	void AddClickHandler ()
	{
		Button b = gameObject.GetComponent<Button> ();
		if (b != null) {
			b.onClick.AddListener (() => OnButtonClicked (gameObject.name, buttonId));
		} else {
			Debug.LogError ("Click handler cannot be registered. Button not found.", this);
		}
	}

	void OnButtonClicked (string buttonName, string buttonId)
	{
		if (buttonId.Equals ("OpenGameModeMenu")) {
			GameSceneManager.instance.OpenGameModeMenuScene ();
		} else if (buttonId.Equals ("OpenLevelMenu")) {
			GameSceneManager.instance.OpenLevelMenuScene ();
		} else if (buttonId.Equals ("OpenTitleMenu")) {
			GameSceneManager.instance.OpenTitleMenuScene ();
		} else if (buttonId.Equals ("OpenLevel")) {
			GameSceneManager.instance.OpenGamePlayScene (0);
		} else if (buttonId.Equals ("OpenCompletionMenu")) {
			GameSceneManager.instance.OpenCompletionMenuScene ();
		} else if (buttonId.Equals ("OpenNextLevel")) {
			GameSceneManager.instance.OpenGamePlayScene (0);
		} else {
			Debug.LogError (string.Format ("No action defined for button {0} with id {1}", buttonName, buttonId), this);
		}
	}
}
