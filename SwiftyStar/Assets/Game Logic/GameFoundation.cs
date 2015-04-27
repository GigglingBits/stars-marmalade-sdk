using UnityEngine;
using System.Collections;

public class GameFoundation
{
	public Vector2 worldSize { get; set; }

	#region singleton
	private static GameFoundation _theInstance = null;
	
	public static GameFoundation instance {
		get {
			if (null == _theInstance) {
				_theInstance = new GameFoundation ();
			}
			return _theInstance;
		}
	}
	#endregion

	private GameFoundation ()
	{
		worldSize = new Vector2 (12.0f, 8.0f);		
	}
}
