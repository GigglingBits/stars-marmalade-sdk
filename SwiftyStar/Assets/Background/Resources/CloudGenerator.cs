using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CloudGenerator : MonoBehaviour
{
	
	private int _cloudCounter = 0;
	private float _countdown = 0;

	public float _interval = 2.0f; 
	public List<Sprite> _spriteList;
	
	public void Start ()
	{
		// if the list wasn't configured through the IDE
		if (null == _spriteList) {
			_spriteList = new List<Sprite> ();
		}
	}
	
	public void Update ()
	{
		_countdown -= Time.deltaTime;
		if (_countdown <= 0.0f) {
			_countdown += _interval;
			CreateNextCloud ();
		}
	}
	
	private Sprite GetRandomSprite ()
	{
		if (_spriteList.Count > 0) {
			return _spriteList [Random.Range (0, _spriteList.Count)];
		}
		return null;
	}

	private void CreateNextCloud ()
	{
		// instantiate the cloud
		var prefab = Resources.Load ("CloudPrefab");
		GameObject cloud = Instantiate (prefab, new Vector3 (0, 0, 0), Quaternion.identity) as GameObject; 

		// assign the visual appearance
		string cloudname = "Cloud";
		var renderer = cloud.GetComponent<SpriteRenderer> ();
		if (renderer != null) {
			Sprite face = GetRandomSprite ();
			if (face != null) {
				cloudname += string.IsNullOrEmpty (face.name) ? "_noname" : ("_" + face.name);
			} else {
				cloudname += "_emptysprite";
			}
			renderer.sprite = face;
		} else {
			cloudname += "_norenderer";
		}
		cloudname += "_" + (++_cloudCounter);
		cloud.name = cloudname;

		// prevent counter overflow
		if (_cloudCounter == int.MaxValue) {
			_cloudCounter = 0;
		}

		// editor cosmetics: oganize it as child of current component
		cloud.transform.SetParent (this.transform);
	}
}
