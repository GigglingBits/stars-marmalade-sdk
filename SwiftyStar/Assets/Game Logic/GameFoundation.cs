using UnityEngine;
using System.Collections;

public class GameFoundation
{
	public Vector2 worldSize { get; set; }

	#region singleton
	// singleton
	private static GameFoundation _theGame;

	public static GameFoundation instance {
		get {
			if (null == _theGame) {
				_theGame = new GameFoundation ();
			}
			return _theGame;
		}
	}
	#endregion singleton

	private GameFoundation ()
	{
		worldSize = new Vector2 (12.0f, 8.0f);		
	}

}
