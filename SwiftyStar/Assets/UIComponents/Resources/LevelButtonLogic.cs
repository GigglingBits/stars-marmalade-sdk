using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections;

public interface ILevelMenuFeedbackHandler : IEventSystemHandler
{
	void SetLevelNumber (int id);
}

public class LevelButtonLogic : MonoBehaviour, ILevelMenuFeedbackHandler
{
	public int _buttonId; 

	public int _levelNumber; 
	private int _levelNumberOnScreen;

	void Start ()
	{
		RegisterEvents ();
		RegisterButton ();
		UpdateButtonText ();
	}
	
	void Update ()
	{
		if (_levelNumber != _levelNumberOnScreen) {
			UpdateButtonText ();
		}
	}

	public void SetLevelNumber (int id)
	{
		_levelNumber = id;
	}

	private void RegisterEvents ()
	{
	
	}

	private void RegisterButton ()
	{
		ExecuteEvents.ExecuteHierarchy<ILevelMenuButtonInstantiatedHandler> (this.gameObject, null, (x,y) => x.RegisterButton (_buttonId, this));
	}

	private void UpdateButtonText ()
	{
		var text = GetComponentInChildren<Text> ();
		if (null != text) {
			text.text = _levelNumber.ToString ();
			_levelNumberOnScreen = _levelNumber;
		} else {
			Debug.LogError ("Unable to find Text component for level button. Cannot display levelnumber");
		}
	}

	public void OnClick ()
	{
		// todo: open level
		//ExecuteEvents.ExecuteHierarchy<ILevelMenuButtonInstantiatedHandler> (this.gameObject, null, (x,y) => x.RegisterButton (_buttonId, this));
	}	
}
