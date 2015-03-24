using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CloudGenerator : MonoBehaviour {

	private const float CLOUD_INTERVAL_SEC = 2.0f; 
	private int _cloudCounter = 0;

	private float _countdown = CLOUD_INTERVAL_SEC;

	private List<Sprite> _spriteList = new List<Sprite>();

	public void Start () {
		LoadSprites(_spriteList);
	}
	
	public void Update () {
		_countdown -= Time.deltaTime;
		if (_countdown <= 0.0f) {
			_countdown += CLOUD_INTERVAL_SEC;
			CreateNextCloud();
		}
	}

	private void LoadSprites(List<Sprite> list) {
		list.AddRange(Resources.LoadAll<Sprite> ("cloud01"));
		list.AddRange(Resources.LoadAll<Sprite> ("cloud02"));
		list.AddRange(Resources.LoadAll<Sprite> ("cloud03"));
		list.AddRange(Resources.LoadAll<Sprite> ("cloud04"));
	}

	private Sprite GetRandomSprite() {
		return _spriteList[Random.Range(0, _spriteList.Count)];
	}

	private void CreateNextCloud() {
		// instantiate the cloud
		var prefab = Resources.Load ("CloudPrefab");
		GameObject cloud = Instantiate(prefab, new Vector3(0, 0, 0), Quaternion.identity) as GameObject; 

		// assign the visual appearance
		string cloudname = "Cloud";
		var renderer = cloud.GetComponent<SpriteRenderer>();
		if (renderer != null) {
			Sprite face = GetRandomSprite ();
			if (face != null) {
				cloudname += string.IsNullOrEmpty (face.name) ? "_noname" : ("_" + face.name);
			} else {
				cloudname += "_err2";
			}
			renderer.sprite = face;
		} else {
			cloudname += "_err1";
		}
		cloudname += "_" + (++_cloudCounter);
		cloud.name = cloudname;

		// prevent counter overflow
		if (_cloudCounter == int.MaxValue) {
			_cloudCounter = 0;
		}

		// editor cosmetics: oganize it as child of current component
		cloud.transform.SetParent(this.transform);
	}
}
