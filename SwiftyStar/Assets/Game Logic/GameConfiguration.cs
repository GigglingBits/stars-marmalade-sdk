using UnityEngine;
using System.Xml;
using System.IO;
using System.Collections;
using System.Collections.Generic;

public class GameConfiguration
{
	#region levels
	public struct LevelObjectSpawnStruct
	{
		public string objectName;
		public float positionY;
	}

	public struct LevelSequenceStruct
	{
		public float movementSpeed;
		public float spawnInterval;
		public List<LevelObjectSpawnStruct> objects;
	}

	public struct LevelStruct
	{
		public string name;
		public int dustRequirement;
		public int numberOfLifes;
		public Vector2 size;
		public List<LevelSequenceStruct> sequences;
	}
	#endregion

	#region singleton
	private static GameConfiguration _theInstance = null;

	public static GameConfiguration instance {
		get {
			if (null == _theInstance) {
				_theInstance = new GameConfiguration ();
			}
			return _theInstance;
		}
	}
	#endregion singleton

	private const string _BODIES_RESOURCE = "bodies_config";
	private const string _LEVELS_RESOURCE = "levels_config";

	private Dictionary<string, LevelStruct> _levels = new Dictionary<string, LevelStruct> ();

	private GameConfiguration ()
	{
		LoadConfiguration ();
	}

	private void LoadConfiguration ()
	{
		var asset = Resources.Load<TextAsset> (_BODIES_RESOURCE);
		if (asset != null) {
			BuildBodies (asset.text);
		} else {
			Debug.LogError ("Unable to load asset:" + _BODIES_RESOURCE);
		}
		asset = Resources.Load<TextAsset> (_LEVELS_RESOURCE);
		if (asset != null) {
			BuildLevels (asset.text);
		} else {
			Debug.LogError ("Unable to load asset:" + _LEVELS_RESOURCE);
		}
	}

	private void BuildBodies (string xml)
	{
	}

	private void BuildLevels (string xml)
	{
		if (xml == null) {
			Debug.LogError ("Levels configuration is NULL. Cannot build levels.");
			return;
		}

		var doc = new XmlDocument ();
		doc.LoadXml (xml);
	
		// some validation
		string ns = doc.DocumentElement.NamespaceURI;
		if (ns == null || ns != "http://www.gigglingbits.com") {
			Debug.LogError ("Levels configuration has wrong namespace (" + ns + "). Cannot build levels. ");
			return;
		}

		// to make XPath work; it will look the namespaces up as "ns"
		var nsmgr = new XmlNamespaceManager (doc.NameTable);
		nsmgr.AddNamespace ("ns", ns);

		// import levels
		XmlNodeList levelnodes = doc.SelectNodes ("/ns:root/ns:levels/ns:level", nsmgr);
		foreach (XmlNode levelnode in levelnodes) {

			LevelStruct level = new LevelStruct ();
			TryGetXmlAttribute (levelnode, "name", out level.name);
			TryGetXmlAttribute (levelnode, "dustrequirement", out level.dustRequirement);
			TryGetXmlAttribute (levelnode, "lives", out level.numberOfLifes);

			float w, h;
			TryGetXmlAttribute (levelnode, "width", out w);
			TryGetXmlAttribute (levelnode, "height", out h);
			level.size = new Vector2 (w, h);
			level.sequences = new List<LevelSequenceStruct> ();

			_levels.Add (level.name, level);
		}
	}

	private bool TryGetXmlAttribute (XmlNode node, string attribute, out string text)
	{
		text = null;
		var a = node.Attributes [attribute];
		if (null != a) {
			text = a.Value;
			return true;
		}
		return false;
	}
	
	private bool TryGetXmlAttribute (XmlNode node, string attribute, out int number)
	{
		number = 0;
		string s;
		if (TryGetXmlAttribute (node, attribute, out s)) {
			if (int.TryParse (s, System.Globalization.NumberStyles.Integer, null, out number)) {
				return true;
			}
		}
		return false;
	}
	
	private bool TryGetXmlAttribute (XmlNode node, string attribute, out float number)
	{
		number = 0.0f;
		string s;
		if (TryGetXmlAttribute (node, attribute, out s)) {
			if (float.TryParse (s, System.Globalization.NumberStyles.Float, null, out number)) {
				return true;
			}
		}
		return false;
	}
	
	public bool TryGetLevel (string levelName, out LevelStruct level)
	{
		return _levels.TryGetValue (levelName, out level);
	}
}
