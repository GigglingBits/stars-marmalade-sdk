using UnityEngine;
using System.Collections;
using System.Xml;

public class GameLevel : MonoBehaviour
{

	public string _levelName { get; private set; }

	// Use this for initialization
	void Start ()
	{
		_levelName = "earth01";
		XmlNode level = GameConfiguration.instance.GetLevel (_levelName);
	}
	
	// Update is called once per frame
	void Update ()
	{
	
	}
}
