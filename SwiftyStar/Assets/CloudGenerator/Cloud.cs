using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Cloud : MonoBehaviour {

	private List<Sprite> sprites = new List<Sprite>();

	void Start () {
		sprites.AddRange(Resources.LoadAll<Sprite> ("cloud_01"));
		sprites.AddRange(Resources.LoadAll<Sprite> ("cloud_02"));
		sprites.AddRange(Resources.LoadAll<Sprite> ("cloud_03"));
		sprites.AddRange(Resources.LoadAll<Sprite> ("cloud_04"));

		GetComponent<SpriteRenderer>().sprite = sprites[Random.Range(0, sprites.Count)];
	}
	
	// Update is called once per frame
	void Update () {
		var pos = transform.position;
		pos.x -= 0.2f;
		transform.position = pos;
	}
}
