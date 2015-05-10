using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GameLevel : MonoBehaviour
{
	public string _levelName { get; private set; }


	#region commands
	public enum CommadType
	{
		Spawn,
		Sleep
	}
	public struct CommandStruct
	{
		CommadType type;
		float delay;
		float ypos;
	}
	public Queue<CommandStruct> _commandSequence;
	private float _commandTimer = 0.0f;
	#endregion 

	void Start ()
	{
		_levelName = "earth01";
		GameConfiguration.LevelStruct level;
		GameConfiguration.instance.TryGetLevel (_levelName, out level);
	}
	
	void Update ()
	{
		if (_commandTimer <= 0.0f) {
			// execute command, and 
		}
	
	}
}
