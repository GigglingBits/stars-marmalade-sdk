using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections;
using System.Collections.Generic;


public interface ILevelMenuButtonInstantiatedHandler : IEventSystemHandler
{
	void RegisterButton (int id, ILevelMenuFeedbackHandler button);
}

public class LevelMenu : MonoBehaviour, ILevelMenuButtonInstantiatedHandler
{
	private Dictionary<int, ILevelMenuFeedbackHandler> _buttons = new Dictionary<int, ILevelMenuFeedbackHandler> ();	
	
	public int _numberOfLevelPages = 3;
	public int _currentLevelPage = 0;

	private bool _buttonUpdateNeeded;

	void Start ()
	{
		_buttonUpdateNeeded = true;
	}
	
	void Update ()
	{	
		if (_buttonUpdateNeeded) {
			UpdateButtons ();
		}
	}

	public void RegisterButton (int id, ILevelMenuFeedbackHandler button)
	{
		_buttons.Add (id, button);
		_buttonUpdateNeeded = true;
	}	

	private void UpdateButtons ()
	{
		foreach (var id in _buttons.Keys) {
			int level = (_currentLevelPage * _buttons.Count) + id;
			_buttons [id].SetLevelNumber (level);
		}
	}

	public void NextPage ()
	{
		_currentLevelPage = (_currentLevelPage + 1) % _numberOfLevelPages;
		_buttonUpdateNeeded = true;
	}
	
	public void PreviousPage ()
	{
		_currentLevelPage = (_numberOfLevelPages + _currentLevelPage - 1) % _numberOfLevelPages;
		_buttonUpdateNeeded = true;
	}
}
