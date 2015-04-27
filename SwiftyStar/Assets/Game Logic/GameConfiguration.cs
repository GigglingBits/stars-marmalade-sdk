using UnityEngine;
using System.Xml;
using System.Collections;

public class GameConfiguration
{
	#region singleton
	private static GameConfiguration _theInstance = null;

	private const string _BODIES_RESOURCE = "bodies";
	private const string _LEVELS_RESOURCE = "levels";

	private readonly XmlDocument _levels = new XmlDocument ();
	private readonly XmlDocument _bodies = new XmlDocument ();

	public static GameConfiguration instance {
		get {
			if (null == _theInstance) {
				_theInstance = new GameConfiguration ();
			}
			return _theInstance;
		}
	}
	#endregion singleton

	private GameConfiguration ()
	{
		LoadConfiguration ();
	}

	private void LoadConfiguration ()
	{
		var bodies = Resources.Load<TextAsset> (_BODIES_RESOURCE);
		_bodies.LoadXml (bodies.text);

		var levels = Resources.Load<TextAsset> (_LEVELS_RESOURCE);
		_levels.LoadXml (levels.text);
	}

	public XmlNode GetLevel (string leveName)
	{
		return null;
	}
}
