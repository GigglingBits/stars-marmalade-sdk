using UnityEngine;
using System.Collections;

public class CloudGenerator : MonoBehaviour {

	private const float CLOUD_INTERVAL_SEC = 3.0f; 
	private float _countdown;

	public CloudGenerator () {
		_countdown = CLOUD_INTERVAL_SEC;
	}

	public void Start () {
	}
	
	public void Update () {
		_countdown -= Time.deltaTime;
		if (_countdown <= 0.0f) {
			_countdown += CLOUD_INTERVAL_SEC;
			CreateNextCloud();
		}
	}
	
	private void CreateNextCloud() {
		//var cloud = Instantiate<Cloud01>();

	//	Instantiate<Cloud01> ();

/*
 		GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
		cube.AddComponent<Rigidbody>();
		cube.transform.position = new Vector3(0, 0, 0);	
*/
	//	GetComponents

		// create a cloud

	}
}
