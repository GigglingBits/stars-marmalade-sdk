using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CloudLogic : MonoBehaviour {

	public float _cloudSpeedMetersPerSeconds = 1.0f; // m/s
	public float _cloudScale = 1.0f; 

	void Start () {
		InitializeTransform();
	}
	
	void Update () {
		Move ();
		ManageLyfecycle ();
	}

	void InitializeTransform() {
		ConfigureDistance ();
		ConfigureLocation ();
	}
	
	void ConfigureDistance() {
		float depthConstant;
		switch (Random.Range (0, 3)) {
		case 0: 
			depthConstant = 1.0f;
			break;
		case 1:
			depthConstant = 0.5f;
			break;
		default:
			depthConstant = 0.133f;
			break;
		}

		_cloudSpeedMetersPerSeconds *= depthConstant;

		transform.localScale = new Vector3 (depthConstant * _cloudScale, depthConstant * _cloudScale, depthConstant * _cloudScale);

		Vector3 pos = transform.localPosition;
		pos.z = -depthConstant;
		transform.localPosition = pos;
	}

	void ConfigureLocation() {
		Vector3 pos = transform.localPosition;
		pos.x = 8.0f;
		pos.y = Random.Range (-8.0f, 8.0f);
		transform.localPosition = pos;
	}

	void Move() {
		var pos = transform.localPosition;
		pos.x -= _cloudSpeedMetersPerSeconds * Time.deltaTime;
		transform.localPosition = pos;
	}

	void ManageLyfecycle() {
		bool outofbounds = transform.localPosition.x < -8.0f;
		if (outofbounds) {
			Destroy(gameObject);
		}
	}
}
