using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CloudLogic : MonoBehaviour {

	void Start () {
		InitializeTransform();
	}
	
	void Update () {
		Move ();
		ManageLyfecycle ();
	}

	void InitializeTransform() {
		Sprite s = GetComponent<SpriteRenderer> ().sprite;

		transform.localPosition = new Vector3 (8, 0, 0);
		transform.localScale = new Vector3 (1, 1, 1);
	}
	
	void Move() {
		var pos = transform.localPosition;
		pos.x -= 0.03f;
		transform.localPosition = pos;
	}

	void ManageLyfecycle() {
		bool outofbounds = transform.localPosition.x < -8.0f;
		if (outofbounds) {
			Destroy(gameObject);
		}
	}
}
